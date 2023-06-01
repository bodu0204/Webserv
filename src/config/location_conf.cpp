#include "location_conf.hpp"

const location_conf location_conf::error;

location_conf::location_conf():is_faile(true), _root(), _index(),_cgi(){return ;}
location_conf::location_conf(const location_conf &src):is_faile(src.is_faile), _root(src._root), _index(src._index),_cgi(src._cgi){return ;}
const location_conf &location_conf::operator=(const location_conf &src){
	this->is_faile = src.is_faile;
	this->_root = src._root;
	this->_index = src._index;
	return (*this);
}

bool location_conf::faile() const{return (this->is_faile);}

location_conf::~location_conf(){return ;}

location_conf::location_conf(std::string src):is_faile(false), _root(), _index(),_cgi(){
	printf("[(%s/%d) %s ]%s\n", __FILE__, __LINE__, __func__, ((char *)"NOT_MAKE")); fflush(stdin);
	return ;
}

