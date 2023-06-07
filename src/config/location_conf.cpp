#include "location_conf.hpp"
#include "../utils/utils.hpp"
const location_conf location_conf::error;

location_conf::location_conf():is_faile(true), _root(), _index(),_cgi(),_autoindex(true){return ;}
location_conf::location_conf(const location_conf &src):is_faile(src.is_faile), _root(src._root), _index(src._index),_cgi(src._cgi),_autoindex(src._autoindex){return ;}
const location_conf &location_conf::operator=(const location_conf &src){
	this->is_faile = src.is_faile;
	this->_root = src._root;
	this->_index = src._index;
	this->_autoindex = src._autoindex;
	return (*this);
}

bool location_conf::faile() const{return (this->is_faile);}

std::string location_conf::root() const{return (this->_root);}
std::string location_conf::index() const{return (this->_index);}
std::string location_conf::cgi() const{return (this->_cgi);}
bool location_conf::autoindex() const{return (this->_autoindex);}

location_conf::~location_conf(){return ;}

location_conf::location_conf(std::string src):is_faile(false), _root(), _index(),_cgi(),_autoindex(true){
	src = utils::trim_sp(src);
	if (!src.length()){this->is_faile = true; return;}
	bool autoi = false;
	while (src.length()){
		std::string buf = utils::new_token(src, this->is_faile);
		if (this->is_faile) {return ;}
		if (!buf.length() && !src.length()){break;}
		if(buf == "root"){
			std::string value = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (this->_root.length()) {this->is_faile = true; return;}
			this->_root = value;
		}else if(buf == "index"){
			std::string value = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (this->_index.length()) {this->is_faile = true; return;}
			this->_index = value;
			autoi = true;
			this->_cgi = std::string();
		}else if(buf == "cgi"){
			std::string value = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (this->_cgi.length()) {this->is_faile = true; return;}
			this->_cgi = value;
		}else if(buf == "autoindex"){
			std::string value = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (autoi) {this->is_faile = true; return;}
			autoi = true;
			if (value == "off")
				this->_cgi = std::string();
			else if (value != "on")
				{this->is_faile = true; return;}
		}else{
			this->is_faile = true; return ;}
	}
	if (!this->_root.length()) {this->is_faile = true; return;}
	return ;
}

