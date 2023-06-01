#include "server_conf.hpp"
#include "../utils/utils.hpp"

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
	std::map<std::string,location_conf>::const_iterator loc = this->_locations.end();
	size_t len = 0;
		for (std::map<std::string,location_conf>::const_iterator i = this->_locations.begin();i != this->_locations.end(); i++){
		if (i->first.length() > len && src.substr(0, i->first.length()) == i->first){
			loc = i;
			len = i->first.length();
		}
	}
	if (loc == this->_locations.end())
		return (location_conf::error);
	return (loc->second);
}

server_conf::server_conf(std::string src):is_faile(false),_locations(){
	src = utils::trim_sp(src);
	if (!src.length()){this->is_faile = true; return;}
	std::string server_name;
	std::string location;
	while (src.length()){
		std::string buf = utils::new_token(src, this->is_faile);
		if (this->is_faile) {return ;}
		if (!buf.length() && !src.length()){break;}
		if(buf == "location"){
			std::string pattern = utils::new_token(src, this->is_faile);
			if (this->is_faile) {return ;}
			if (this->_locations.find(pattern) == this->_locations.end()) {this->is_faile = true; return;}
			std::string value = utils::new_token(src, this->is_faile, true);
			if (this->is_faile) {return ;}
			location_conf lf(value);
			if (lf.faile()) {this->is_faile = true; return;}
			this->_locations.insert(std::pair<std::string, location_conf>(pattern, lf));
		}else{
			this->is_faile = true; return ;}
	}
	if (!this->_locations.size()) {this->is_faile = true; return;}
	return ;
}
