#include <string>
#include <poll.h>
#include <unistd.h>
#include "cgir_handler.hpp"
#define BUFFERSIZE 1024

#include "debug.h"

void cgir_handler::_action(short event){
    if (event & ~this->events){
		this->set_del(this->all_child());
		this->set_del(this);
		return ;
	}
    //if (/* event & POLL_OUT &&  */this->buf.length())
	char buff[BUFFERSIZE + 1];
	ssize_t r = read(this->descriptor, buff, BUFFERSIZE);
	if (r < 0){
		this->set_del(this->all_child());
		this->set_del(this);
		return ;
	}
	buff[r] = '\0';
Tn(r)
Ts(buff)
	if (this->parent)
		this->parent->callback(buff);
    return ;
}

cgir_handler::cgir_handler(handler *parent,int descriptor):handler(parent, descriptor, POLL_IN){}

cgir_handler::~cgir_handler(){}

cgir_handler::cgir_handler():handler(NULL, 0, 0){}//not use
cgir_handler::cgir_handler(const cgir_handler&):handler(NULL, 0, 0){}//not use
const cgir_handler &cgir_handler::operator=(const cgir_handler&){return (*this);}//not use
