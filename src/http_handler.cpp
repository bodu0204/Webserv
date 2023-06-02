#include "http_handler.hpp"
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#define BUFFERSIZE 1024

void http_handler::_action(short event){}

void http_handler::callback_end(handler *){}

void http_handler::callback(std::string){}

void http_handler::exec(){}

void http_handler::_to_req(){}

void http_handler::_to_res(std::string){}

http_handler::http_handler(handler *parent, int descriptor, const port_conf &conf):\
	handler(parent, descriptor, POLL_IN|POLL_OUT, 40),_conf(conf),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(){
	
}

http_handler::~http_handler(){}


http_handler::http_handler():handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(){}//not use
http_handler::http_handler(const http_handler&):handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(){}//not use
const http_handler &http_handler::operator=(const http_handler&){return (*this);}//not use



