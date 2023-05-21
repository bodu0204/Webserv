#include "config.hpp"
#include "../utils/utils.hpp"

config::config():is_faile(true),ports(){}//not call
config::config(const config &src):is_faile(src.is_faile),ports(src.ports){}
const config &config::operator=(const config &src){this->is_faile = src.is_faile;this->ports;}
config::~config(){this->ports.clear();}
const std::vector<port_conf>& config::port_confs() const{return (this->ports);}
bool config::faile() const{return (this->is_faile);}

config::config(std::string src):is_faile(false),ports(){
    while (true){
	    std::string buf = utils::new_token(src, this->is_faile);
        if (this->is_faile){return ;}
        if (!buf.length() && !src.length()) {return ;}

        if (buf == "server"){
            std::string cont = utils::new_token(src, this->is_faile, true);
            if (this->is_faile) {return ;}

            port_conf pc(cont);
            if (pc.faile()){this->is_faile = true; return ;}
            for (std::vector<port_conf>::iterator i = this->ports.begin(); i !=  this->ports.end(); i++){
                if (i->port() == pc.port()){
                    i->marge(pc);
                    if (i->faile())
                        this->is_faile = true;
                    return ;
                }
            }
            this->ports.push_back(cont);
        }else{
            this->is_faile = true; return ;}
    }
    return ;
}
