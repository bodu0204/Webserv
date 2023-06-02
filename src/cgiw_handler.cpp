#include <string>
#include <poll.h>
#include <unistd.h>
#include "cgiw_handler.hpp"
#define BUFFERSIZE 1024

#include "debug.h"

void cgiw_handler::_action(short event){
	if (this->buf.length())
	{
		ssize_t r = write(this->descriptor, this->buf.c_str(), this->buf.length());
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		this->buf = this->buf.substr(r, this->buf.length() - r);
	}
	return ;
}

void cgiw_handler::set_write(std::string req){
	bool doit = !this->buf.length();
	this->buf += req;
	this->_action(POLL_OUT);
	return ;
}

cgiw_handler::cgiw_handler(handler *parent, int descriptor):buf(),handler(parent, descriptor, POLL_OUT){}

cgiw_handler::~cgiw_handler(){}

cgiw_handler::cgiw_handler():buf(),handler(NULL, 0, 0){}//not use

cgiw_handler::cgiw_handler(const cgiw_handler&):buf(),handler(NULL, 0, 0){}//not use

const cgiw_handler &cgiw_handler::operator=(const cgiw_handler&){return (*this);}//not use

