#include "config.hpp"
#include "../utils/utils.hpp"

config::config(){}//not call
config::config(const config &src):is_faile(src.is_faile),ports(src.ports){}
const config &config::operator=(const config &src){this->is_faile = src.is_faile;this->ports;}
config::~config(){this->ports.clear();}
std::vector<port_conf> config::port_confs() const{return (this->ports);}
bool config::faile() const{return (this->is_faile);}

config::config(std::string src):is_faile(false),ports(){
    while (true){
	    std::string buf = utils::get_next_token(src);
	    if (!buf.length()) {this->is_faile = true; return ;}
        buf = utils::trim_sp(buf);
        if (!buf.length() && !src.length()) {return ;}
	    if (!buf.length()) {this->is_faile = true; return ;}
        buf = utils::trim_meta(buf);
	    if (!buf.length()) {this->is_faile = true; return ;}
        if (buf == "server"){
            std::string cont = utils::get_next_token(src);
            if (!cont.length() || cont[0] != '{' || cont[cont.length() - 1] != '}') {this->is_faile = true; return ;}
            cont = cont.substr(1, cont.length());
            port_conf pc(cont);
            if (pc.faile()){this->is_faile = true; return ;}
            for (std::vector<port_conf>::iterator i = this->ports.begin(); i !=  this->ports.end(); i++){
                if (i->port() == pc.port()){this->is_faile = true; return ;}
            }
            this->ports.push_back(cont);
        }else
            {this->is_faile = true; return ;}
    }
    return ;
}
