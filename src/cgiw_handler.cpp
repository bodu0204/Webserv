#include <string>
#include <poll.h>
#include <unistd.h>
#include "cgiw_handler.hpp"
#define BUFFERSIZE 1024

void cgiw_handler::_action(short){
	if (this->buf.length())
	{
		ssize_t r = write(this->descriptor, this->buf.c_str(), this->buf.length());
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		this->buf = this->buf.substr(r);
	}
	if (!this->buf.length() && this->_end)
		this->set_del(this);
	return ;
}

size_t cgiw_handler::set_write(std::string req, bool end){
	bool doit = !this->buf.length();
	this->buf += req;
	if (doit)
		this->_action(POLL_OUT);
	size_t r = this->buf.length();
	if (r && end)
		this->_end = true;
	return (r);
}

cgiw_handler::cgiw_handler(handler *parent, int descriptor):handler(parent, descriptor, POLL_OUT),buf(),_end(false){}

cgiw_handler::~cgiw_handler(){}

cgiw_handler::cgiw_handler():handler(NULL, 0, 0),buf(),_end(false){}//not use

cgiw_handler::cgiw_handler(const cgiw_handler&):handler(NULL, 0, 0),buf(),_end(false){}//not use

const cgiw_handler &cgiw_handler::operator=(const cgiw_handler&){return (*this);}//not use

