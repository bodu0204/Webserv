#include "http_handler.hpp"
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#define BUFFERSIZE 1024

void http_handler::_action(short event){
	if (event & POLL_IN){
		std::string buff;
		ssize_t r = this->read(buff);
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		this->_req_buff += buff;
		if (!this->_cgi_pid){
			this->_to_req();
		}
	}
	if (event & POLL_OUT && this->_res_buff.length()){
		ssize_t r = this->write(this->_res_buff);
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		this->_res_buff = this->_res_buff.substr(r, this->_res_buff.length() - r);
	}
	return ;
}

void http_handler::callback_end(handler *target){
	if (target == this->_cgi_w){
		this->_cgi_w = NULL;
	}
	if (target == this->_cgi_r){
		this->_cgi_r = NULL;
		if (this->_cgi_pid){
			kill(this->_cgi_pid, SIGINT);
			int wstatus;
    	 	waitpid(this->_cgi_pid, &wstatus, 0);
			this->_cgi_pid = 0;
		}
	}
	handler::callback_end(target);
}

void http_handler::callback(std::string str){}

void http_handler::exec(){}


void http_handler::_to_req(){
	if (this->_req.find(KEY_BODY) == this->_req.end()){
		size_t crlf = this->_req_buff.find("\n\n");
		if (crlf != UINT64_MAX)
			crlf += 1;
		else{
			crlf = this->_req_buff.find("\r\n\r\n");
			if (crlf != UINT64_MAX)
				crlf += 2;
		}
		if (crlf != UINT64_MAX){
			this->_req[KEY_BODY] = std::string();
		}
		else{
			crlf = this->_req_buff.rfind("\n");
			if (crlf == UINT64_MAX)
				return ;
			else
				crlf += 1;
		}
		std::string header = this->_req_buff.substr(0, crlf);
		this->_req_buff = this->_req_buff.substr(header.length(), this->_req_buff.length());
		if (this->_req_buff.find("\n") <= 1)
			this->_req_buff = this->_req_buff.substr(this->_req_buff.find("\n") + 1, this->_req_buff.length());
		{
			std::string reqline = header.substr(0, header.find("\n") + 1);
			header = header.substr(reqline.length(), header.length());
			if (reqline[0] == ' ' || reqline[0] == '\t') {this->Bad_Request(); return ;}
			size_t s = 0, e = 0;
			for (; reqline[e] && reqline[e] != ' '; e++);
			this->_req[KEY_METHOD] = reqline.substr(s, e);
			e++;
		}
	}
	//body
}

ssize_t http_handler::write(const std::string &target){
	return (send(this->descriptor, target.c_str(), target.length(), 0));
}

ssize_t http_handler::read(std::string &target){
	char buff[BUFFERSIZE + 1];
	ssize_t r = recv(this->descriptor, buff, BUFFERSIZE, 0);
	if (r >= 0){
		buff[r] = '\0';
		target = buff;
	}
	return (r);
}

http_handler::http_handler(handler *parent, int descriptor, const port_conf &conf, struct sockaddr_in info):\
	handler(parent, descriptor, POLL_IN|POLL_OUT, 40),_conf(conf),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(info){}

http_handler::~http_handler(){}

void http_handler::Bad_Request(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += RES_400;
	this->_req_buff.clear();
	this->_req.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

http_handler::http_handler():handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(){}//not use
http_handler::http_handler(const http_handler&):handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(){}//not use
const http_handler &http_handler::operator=(const http_handler&){return (*this);}//not use


