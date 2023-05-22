#include <iostream>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <vector>
#include <map>
#include "utils/utils.hpp"
#include "config/config.hpp"
#include "config/port_conf.hpp"
#include "handler.hpp"
#include "accept_handker.hpp"

#define EXTENSION ".conf"

static inline int is_argerror(int argc, char *argv[]){
	return (argc != 2 \
		|| strlen(argv[1]) < strlen(EXTENSION) \
		|| memcmp(argv[1] + strlen(argv[1]) - strlen(EXTENSION), EXTENSION, sizeof(EXTENSION)));
}

void run(std::map<int, handler *>&);

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
		int disc;

		//socketを確立

		accept_handker *ah = new accept_handker(disc, *i);
		handlers[i->port()] = ah;
	}
	run(handlers);
	return (0);
}
