#include "port_conf.hpp"
#include "../utils/utils.hpp"
#include <sstream>

const port_conf port_conf::error;

port_conf::port_conf():is_faile(true),_port(0), _proto(undfind_protocol),_default(""),_body_len(UINT64_MAX),_servers(){}

port_conf::port_conf(const port_conf &src):is_faile(src.is_faile), _port(src._port), _proto(src._proto),_default(src._default),_body_len(src._body_len), _servers(src._servers){}

const port_conf &port_conf::operator=(const port_conf &src){
	this->is_faile = src.is_faile;
	this->_port = src._port;
	this->_proto = src._proto;
	this->_servers = src._servers;
	return (*this);
}

port_conf::~port_conf(){this->_servers.clear();}

unsigned short port_conf::port() const{return(this->_port);}
protocol port_conf::protocol() const{return(this->_proto);}
size_t port_conf::body_length() const{return(this->_body_len);}
const server_conf &port_conf::server(std::string name, std::string& ret_name) const{
	ret_name = name;
	if (this->_servers.find(name) != this->_servers.end())
		return(this->_servers.at(name));
	ret_name = this->_default;
	return (this->_servers.at(this->_default));
}

bool port_conf::faile() const{return(this->is_faile);}

port_conf::port_conf(std::string src):is_faile(false), _port(0), _proto(undfind_protocol),_default(""),_body_len(UINT64_MAX), _servers(){
	src = utils::trim_sp(src);
	if (!src.length()){this->is_faile = true; return;}
	std::string server_name;
	std::string location;
	bool bl = false;
	while (src.length()){
		std::string buf = utils::new_token(src, this->is_faile);
		if (this->is_faile) {return ;}
		if (!buf.length() && !src.length()){break;}
		if (buf == "port"){
			if (this->_port){this->is_faile = true; return;}
			std::string cont = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (cont.length() > 4 || !utils::is_numstring(cont.c_str())){this->is_faile = true; return;}
			int p = atoi(cont.c_str());
			if (p <= 0 || p > 65535){this->is_faile = true; return;}
			this->_port = (unsigned short)p;
		}else if(buf == "protocol"){
			if (this->_proto != undfind_protocol){this->is_faile = true; return;}
			std::string cont = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
#ifndef NO_ECHO
			if (cont == "echo"){
				this->_proto = tcp_echo;}else
#endif
			if(cont == "http"){
				this->_proto = http;
			}/* else if (cont == "https"){
				this->_proto = https;
			} */else{
				this->is_faile = true; return ;}
		}else if(buf == "name"){
			if (server_name.length()){this->is_faile = true; return;}
			std::string cont = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (this->_servers.find(cont) != this->_servers.end()){this->is_faile = true; return;}
			server_name = cont;
		}else if(buf == "body-length"){
			if (bl){this->is_faile = true; return;}
			bl = true;
			std::string cont = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile || !utils::is_numstring(cont.c_str())) {this->is_faile = true; return;}
			std::stringstream ss;
			ss << cont;
			ss >> this->_body_len;
		}else if(buf == "location"){
			if (location.length())
				location += "\n";
			location += "location ";
			location += utils::new_token(src, this->is_faile, false, false, true);
			location += utils::new_token(src, this->is_faile, false, false, true);
			if (this->is_faile) {return ;}
		}else{
			this->is_faile = true; return ;}
	}
	if (this->_port == 0){this->is_faile = true; return;}
	if (this->_proto == undfind_protocol){this->is_faile = true; return;}
	if (this->_proto == tcp_echo){
		if (server_name.length() || location.length()){this->is_faile = true;}
		return;
	}
	if (!server_name.length()){this->is_faile = true; return;}
	server_conf sc(location);
	if (sc.faile()){this->is_faile = true; return;}
	this->_servers.insert(std::pair<std::string, server_conf>(server_name, sc));
	this->_default = server_name;
	return ;
}

void port_conf::marge(const port_conf &src){
	for (std::map<std::string, server_conf>::const_iterator i = src._servers.begin(); i != src._servers.end(); i++){
		if (this->_servers.find(i->first) == this->_servers.end()){
			this->_servers.insert(std::pair<std::string, server_conf>(i->first, i->second));
		}else{this->is_faile = true; return;}
	}
	return ;
}

