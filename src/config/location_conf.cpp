#include "location_conf.hpp"
#include "../utils/utils.hpp"
const location_conf location_conf::error;

location_conf::location_conf():is_faile(true),_redirect(),_root(),_index(),_cgi(),_methods(),_autoindex(true){return ;}
location_conf::location_conf(const location_conf &src):is_faile(src.is_faile),_redirect(src._redirect),_root(src._root),_index(src._index),_cgi(src._cgi),_methods(src._methods),_autoindex(src._autoindex){return ;}
const location_conf &location_conf::operator=(const location_conf &src){
	this->is_faile = src.is_faile;
	this->_root = src._root;
	this->_index = src._index;
	this->_autoindex = src._autoindex;
	return (*this);
}

bool location_conf::faile() const{return (this->is_faile);}

std::string location_conf::redirect() const{return (this->_redirect);}
std::string location_conf::root() const{return (this->_root);}
std::string location_conf::index() const{return (this->_index);}
bool location_conf::cgi() const{return (!!this->_cgi.size());}
char** location_conf::cgi_arg() const{
	size_t l = this->_cgi.size();
	char **r = static_cast<char **>(malloc(sizeof(char *) * (l + 1)));
	for (size_t i = 0; i < l; i++){
		r[i] = strdup(this->_cgi.at(i).c_str());
	}
	r[l] = NULL;
	return (r);
}

bool location_conf::method(const std::string& target) const{
	return (this->_methods.find(target) != this->_methods.end());
}

bool location_conf::autoindex() const{return (this->_autoindex);}

location_conf::~location_conf(){return ;}

location_conf::location_conf(std::string src):is_faile(false),_redirect(),_root(),_index(),_cgi(),_methods(),_autoindex(true){
	src = utils::trim_sp(src);
	if (!src.length()){this->is_faile = true; return;}
	bool autoi = false;
	bool meth = false;
	while (src.length()){
		std::string buf = utils::new_token(src, this->is_faile);
		if (this->is_faile) {return ;}
		if (!buf.length() && !src.length()){break;}
		if(buf == "redirect"){
			std::string value = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (this->_root.length()) {this->is_faile = true; return;}
			this->_redirect = value;
		}else if(buf == "root"){
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
		}else if(buf == "cgi"){
			if (this->_cgi.size()) {this->is_faile = true; return;}
			while (true){
				std::string srcbuff = src;
				std::string value = utils::new_token(src, this->is_faile, false, true);
				if (this->is_faile){
					src = srcbuff;
					this->is_faile = false;
					value = utils::new_token(src, this->is_faile);
					if (this->is_faile) {return ;}
					this->_cgi.push_back(value);
				}else{
					if (this->is_faile) {return ;}
					this->_cgi.push_back(value);
					break ;
				}
			}
			if (!this->_cgi.size()) {this->is_faile = true; return;}
		}else if(buf == "method"){
			if (meth) {this->is_faile = true; return;}
			meth = true;
			while (true){
				std::string srcbuff = src;
				std::string value = utils::new_token(src, this->is_faile, false, true);
				if (this->is_faile){
					src = srcbuff;
					this->is_faile = false;
					value = utils::new_token(src, this->is_faile);
					if (this->is_faile || this->_methods.find(value) != this->_methods.end())
						{this->is_faile = true; return ;}
					this->_methods.insert(value);
				}else{
					if (this->is_faile || this->_methods.find(value) != this->_methods.end())
						{this->is_faile = true; return ;}
					this->_methods.insert(value);
					break ;
				}
			}
		}else if(buf == "autoindex"){
			std::string value = utils::new_token(src, this->is_faile, false, true);
			if (this->is_faile) {return ;}
			if (autoi) {this->is_faile = true; return;}
			autoi = true;
			if (value == "off")
				this->_autoindex = false;
			else if (value != "on")
				{this->is_faile = true; return;}
		}else{
			this->is_faile = true; return ;}
	}
	if (meth){
		for (std::set<std::string>::iterator i = this->_methods.begin(); i != this->_methods.end(); i++){
			if (*i != "GET" && *i != "HEAD" \
			&& !(*i == "PUT" && !this->_redirect.length()) \
			&& !(*i == "DELETE" && !this->_redirect.length()) \
			&& !(*i == "POST" && !this->_redirect.length() && this->_cgi.size()))
				{this->is_faile = true; return;}
		}
	}else{
		this->_methods.insert("GET");
		this->_methods.insert("HEAD");
		if (!this->_redirect.length()){
			this->_methods.insert("PUT");
			this->_methods.insert("DELETE");
			if (this->_cgi.size())
				this->_methods.insert("POST");
		}
	}
	if (this->_redirect.length()){
		if (this->_root.length() || autoi || this->_cgi.size()) {this->is_faile = true; return;}
		return ;
	}
	if (!this->_root.length()) {this->is_faile = true; return;}
	return ;
}

