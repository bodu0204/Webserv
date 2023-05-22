#include "handler.hpp"

handler::handler(){}//not call

handler::handler(int descriptor, unsigned rw):desc(descriptor),_rw(rw){}

handler::handler(const handler& src):desc(src.desc),_rw(src._rw), add(src.add), del(src.del){}

const handler &handler::operator=(const handler& src){
	this->desc = src.desc;
	this->_rw = src._rw;
	this->add = src.add;
	this->del = src.del;
	return *this;
}

const std::vector<handler *> handler::get_add_handler(){
	std::vector<handler *> ret = this->add;
	this->add.clear();
	return (ret);
}

const std::vector<handler *> handler::get_del_handler(){
	std::vector<handler *> ret = this->del;
	this->del.clear();
	return (ret);
}

int handler::descriptor() const{
	return (this->desc);
}

unsigned handler::descriptor_status() const{
	return (this->_rw);
}
