#include "handler.hpp"

handler::handler(){}//not call

handler::handler(int descriptor):desc(descriptor){}

handler::handler(const handler& src):desc(src.desc), add(src.add), del(src.del){}

const handler &handler::operator=(const handler& src){
	this->desc = src.desc;
	this->add = src.add;
	this->del = src.del;
	return *this;
}

std::vector<handler &> handler::get_add_handler(){
	std::vector<handler &> ret = this->add;
	this->add.clear();
	return (ret);
}

std::vector<handler &> handler::get_del_handler(){
	std::vector<handler &> ret = this->del;
	this->del.clear();
	return (ret);
}

int handler::get_descriptor() const{
	return (desc);
}
