#include <iostream>
#include <string>
#include <string.h>
#include "utils/utils.hpp"
#define EXTENSION ".conf"

class config;

static inline int is_argerror(int argc, char *argv[]){
	return (argc != 2 \
		|| strlen(argv[1]) < strlen(EXTENSION) \
		|| memcmp(argv[1] + strlen(argv[1]) - strlen(EXTENSION), EXTENSION, sizeof(EXTENSION)));
}

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
	



	return (0);
}
