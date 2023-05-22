
#include "port_conf.hpp"
#include "../utils/utils.hpp"

const port_conf port_conf::error;

port_conf::port_conf():is_faile(true),_port(-1), _proto(undfind_protocol), _servers(){}

port_conf::port_conf(const port_conf &src):is_faile(src.is_faile), _port(src.is_faile), _proto(src._proto), _servers(src._servers){}

const port_conf &port_conf::operator=(const port_conf &src){
	this->is_faile = src.is_faile;
	this->_port = src.is_faile;
	this->_proto = src._proto;
	this->_servers = src._servers;
	return (*this);
}

port_conf::~port_conf(){this->_servers.clear();}

int port_conf::port() const{return(this->_port);}
protocol port_conf::protocol() const{return(this->_proto);}
const server_conf &port_conf::server(std::string name) const{
	if (this->_servers.find(name) != this->_servers.end())
		return(this->_servers.at(name));
	return (server_conf::error);
}

bool port_conf::faile() const{return(this->is_faile);}

port_conf::port_conf(std::string src):is_faile(false), _port(-1), _proto(undfind_protocol), _servers(){
	std::string server_name;
	std::string location;
    while (true){
	    std::string buf = utils::new_token(src, this->is_faile);
	    if (this->is_faile) {return ;}
		if (!buf.length() && !src.length()){break;}

        if (buf == "port"){
			if (this->_port != -1){this->is_faile = true; return;}
            std::string cont = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (cont.length() > 4 || !utils::is_numstring(cont.c_str())){this->is_faile = true; return;}
			int p = atoi(cont.c_str());
			if (p <= 0 || p > 65535){this->is_faile = true; return;}
			this->_port = p;
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
			}else if (cont == "https"){
				this->_proto = https;
			}else{
				this->is_faile = true; return ;}
        }else if(buf == "name"){
			if (server_name.length()){this->is_faile = true; return;}
            std::string cont = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (this->_servers.find(cont) != this->_servers.end()){this->is_faile = true; return;}
			server_name = cont;
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
	if (this->_proto == undfind_protocol){this->is_faile = true; return;}
	if (this->_proto == tcp_echo){
		if (server_name.length() || location.length()){this->is_faile = true; return;}
//catを呼び出すようなserver_conf sc("?????????????");
		return;
	}
	server_conf sc(location);
	if (sc.faile()){this->is_faile = true; return;}
	this->_servers[server_name] = sc;
    return ;
}

void port_conf::marge(const port_conf &src){
	for (std::map<std::string, server_conf>::const_iterator i = src._servers.begin(); i != src._servers.end(); i++){
		if (this->_servers.find(i->first) == this->_servers.end()){
			this->_servers[i->first] = i->second;
		}else{this->is_faile = true; return;}
	}
	return ;
}

