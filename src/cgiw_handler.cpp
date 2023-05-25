#include <string>
#include <poll.h>
#include <unistd.h>
#include "cgiw_handler.hpp"
#define BUFFERSIZE 1024

void cgiw_handler::_action(short event){
	if (event & ~this->events){
		this->set_del(this->all_child());
		this->set_del(this);
		return ;
	}
	if (/* event & POLL_OUT &&  */this->buf.length())
	{
		ssize_t r = write(this->descriptor, this->buf.c_str(), this->buf.length());
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		this->buf.substr(r, this->buf.length() - r);
	}
	return ;
}

void cgiw_handler::set_write(std::string req){
	this->buf += req;
	return ;
}

cgiw_handler::cgiw_handler(handler *parent, int descriptor):buf(),handler(parent, descriptor, POLL_OUT, 40){}

cgiw_handler::~cgiw_handler(){handler::~handler();}

cgiw_handler::cgiw_handler():buf(),handler(NULL, 0, 0){}//not use

cgiw_handler::cgiw_handler(const cgiw_handler&):buf(),handler(NULL, 0, 0){}//not use

const cgiw_handler &cgiw_handler::operator=(const cgiw_handler&){return (*this);}//not use

