#include <iostream>
#include <string>
#include <vector>
#include <map>
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
	std::vector<struct pollfd>pollfds;
	for (std::map<int, handler *>::iterator i = handlers.begin(); i != handlers.end(); i++){
		struct pollfd p = {0};
		p.fd = i->first;
		p.events = i->second->events;
		pollfds.push_back(p);
	}
	while (true)
	{
		handler *shortest;
		{
			time_t now = time(NULL);
			long t = LONG_MAX;
			for (std::map<int, handler *>::iterator i = handlers.begin(); i != handlers.end(); i++){}
		}
	}
	
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