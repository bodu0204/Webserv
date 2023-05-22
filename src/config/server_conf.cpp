#include "server_conf.hpp"

const server_conf server_conf::error;

server_conf::server_conf():is_faile(true),_locations(){}

server_conf::server_conf(const server_conf &src):is_faile(src.is_faile),_locations(src._locations){}

const server_conf &server_conf::operator=(const server_conf &src){
	this->is_faile = src.is_faile;
	this->_locations = src._locations;
	return (*this);
}

server_conf::~server_conf(){this->_locations.clear();}

bool server_conf::faile() const{return (this->is_faile);}

const location_conf &server_conf::location(std::string src) const{
	printf("[(%s/%d) %s ]%s\n", __FILE__, __LINE__, __func__, ((char *)"NOT_MAKE")); fflush(stdin);
	std::map<std::string,location_conf>::const_iterator loc = this->_locations.end();
	size_t len = 0;
		for (std::map<std::string,location_conf>::const_iterator i = this->_locations.begin();i != this->_locations.end(); i++){
		if (false/* i->first のパターンがマッチ*/ && i->first.length() > len){
			loc = i;
			len = i->first.length();
		}
	}
	if (loc == this->_locations.end())
		return (location_conf::error);
	return (loc->second);
}

server_conf::server_conf(std::string str):is_faile(false),_locations(){
	(void)str;
	printf("[(%s/%d) %s ]%s\n", __FILE__, __LINE__, __func__, ((char *)"NOT_MAKE")); fflush(stdin);
}
