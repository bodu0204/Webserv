#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <poll.h>
#include "utils/utils.hpp"
#include "config/config.hpp"
#include "config/port_conf.hpp"
#include "handler.hpp"
#include "accept_handker.hpp"

#define EXTENSION ".conf"

std::map<int, handler *> handlers;

static inline int is_argerror(int argc, char *argv[]);
static void run();
static void close_handler();

int main(int argc, char *argv[]){
	if (is_argerror(argc, argv)){
		std::cerr << "COMAND [config-file]" << std::endl;
		return (1);
	}
	std::string file = utils::read_file(argv[1]);
	if (file.length() == 0){
		std::cerr << "ERROR can not read " << argv[1] << " !" << std::endl;
		return (1);
	}
	config conf(file);
	if (conf.faile()){
		std::cerr << "ERROR config-file have error !" << std::endl;
		return (1);
	}
	const std::vector<port_conf>& ports = conf.port_confs();
	std::map<int, handler *>handlers;
	for (std::vector<port_conf>::const_iterator i = ports.begin(); i != ports.end(); i++){
		int disc = socket(AF_INET, SOCK_STREAM, 0);
		if (disc < 0){
			std::cerr << "ERROR can not make socket !" << std::endl;
			close_handler();
			return (1);
		}
		{
			bool optval = true;
			setsockopt(disc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		}
		{
			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htonl(static_cast<uint32_t>(i->port()));
			addr.sin_addr.s_addr = INADDR_ANY;
			if (bind(disc, (const struct sockaddr *)&addr, sizeof(addr)) == -1){
				std::cerr << "ERROR socket can not bind !" << std::endl;
				close_handler();
				close(disc);
				return (1);
			}
		}
		if (listen(disc, SOMAXCONN) == -1){
			std::cerr << "ERROR socket can not listen !" << std::endl;
			close_handler();
			close(disc);
			return (1);
		}
		accept_handker *ah  = new accept_handker(disc, *i);
		handlers[i->port()] = ah;
	}
	run();
	return (0);
}

static void run(){
	std::map<time_t, std::set<int>>schedule;
	for (std::map<int, handler *>::iterator i = handlers.begin(); i != handlers.end(); i++){
		schedule[i->second->limit()].insert(i->first);
	}
	while (true)
	{
		size_t polllen = handlers.size();
		struct pollfd pollfds[polllen];
		{
			size_t l = 0;
			std::map<int, handler *>::iterator i = handlers.begin();
			for (; i != handlers.end(); i++, l++){
				pollfds[l].fd = i->first;
				pollfds[l].events = i->second->events;
				pollfds[l].revents = 0;
			}
		}
		int timeout = schedule.begin()->first - time(NULL);
		if (timeout < 0)
			timeout = 0;
		int ev = poll(pollfds, polllen, timeout);
		std::set<handler *> add;
		std::set<handler *> del;
		for (size_t i = 0; ev > 0; i++){
			if (pollfds[i].revents){
				std::set<handler *> buf;
				handler *h = handlers[pollfds[i].fd];
				h->action(pollfds[i].revents);
				schedule[h->limit()].insert(h->descriptor);
				buf = h->get_add_handler();
				add.insert(buf.begin(), buf.end());
				buf = h->get_add_handler();
				del.insert(buf.begin(), buf.end());
				ev--;
			}
		}
		{
			time_t t = schedule.begin()->first;
			std::set<int> &buf = schedule.begin()->second;
			for (std::set<int>::iterator i = buf.begin(); i != buf.end(); i++)
			{
				handler *h = handlers[*i];
				if (h->limit() == t)
				{
					std::set<handler *> ch = h->all_child();
					del.insert(ch.begin(), ch.end());
					del.insert(h);
				}
			}
			schedule.erase(t);
		}
		for (std::set<handler *>::iterator i = add.begin(); i != add.end(); i++)
		{
			schedule[(*i)->limit()].insert((*i)->descriptor);
			handlers[(*i)->descriptor] = *i;
		}
		for (std::set<handler *>::iterator i = del.begin(); i != del.end(); i++)
		{
			handler *h = *i;
			handlers.erase(h->descriptor);
			if (schedule.find(h->limit()) != schedule.end())
				schedule[h->limit()].erase(h->descriptor);
			close(h->descriptor);
			delete h;
		}
	}
	return ;
}

static inline int is_argerror(int argc, char *argv[]){
	return (argc != 2 \
		|| strlen(argv[1]) < strlen(EXTENSION) \
		|| memcmp(argv[1] + strlen(argv[1]) - strlen(EXTENSION), EXTENSION, sizeof(EXTENSION)));
}

static void rm_handler(handler* target){
}

static void close_handler(){
	for (std::map<int ,handler*>::iterator i = handlers.begin(); i != handlers.end(); i++){
		delete i->second;
		close(i->first);
	}
	handlers.clear();
	return ;
}