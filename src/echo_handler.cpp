#include "echo_handler.hpp"
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#define BUFFERSIZE 1024

#include "debug.h"

void echo_handler::_action(short event){
	if (!this->_cgi_pid){
		int fds[2];
		pipe(fds);
		this->_cgi_pid = fork();
		if(!this->_cgi_pid){
			dup2(fds[STDIN_FILENO], STDIN_FILENO);
			dup2(fds[STDOUT_FILENO], STDOUT_FILENO);
			close(fds[0]);
			close(fds[1]);
			char cmd[] = "/bin/cat";
			char *args[2];
			char *env[1];
			args[0] = cmd;
			args[1] = NULL;
			env[0] = NULL;
			execve(args[0], args, env);
			exit(1);
		}
		if (this->_cgi_pid < 0){
			close(fds[0]);
			close(fds[1]);
			this->set_del(this);
			this->_cgi_pid = 0;
			return ;
		}
		this->_cgi_r = new cgir_handler(this, fds[0]);
		this->_cgi_w = new cgiw_handler(this, fds[1]);
		this->set_add(this->_cgi_r);
		this->set_add(this->_cgi_w);
	}
	if (event & POLL_IN){
		char buff[BUFFERSIZE + 1];
		ssize_t r = recv(this->descriptor, buff, BUFFERSIZE, 0);
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		buff[r] = '\0';
		this->_cgi_w->set_write(buff);
	}
	if (event & POLL_OUT && this->_res_buff.length()){
		ssize_t r = send(this->descriptor, this->_res_buff.c_str(), this->_res_buff.length(), 0);
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
		this->_res_buff = this->_res_buff.substr(r);
	}
	return ;
}

void echo_handler::callback_end(handler *target){
	if (this->_cgi_pid){
		kill(this->_cgi_pid, SIGINT);
		int wstatus;
     	waitpid(this->_cgi_pid, &wstatus, 0);
		this->_cgi_pid = 0;
	}
	if (target == this->_cgi_w){
		this->_cgi_w = NULL;
	}
	if (target == this->_cgi_r){
		this->_cgi_r = NULL;
	}
	handler::callback_end(target);
}

void echo_handler::callback(std::string set){
	bool doit= !this->_res_buff.length();
	this->_res_buff += set;
	if (doit)
		this->action(POLL_OUT);
}

echo_handler::echo_handler(handler *parent,int descriptor):handler(parent, descriptor, POLL_IN|POLL_OUT, 40),_res_buff(),_cgi_w(NULL),_cgi_r(NULL),_cgi_pid(0){}

echo_handler::~echo_handler(){
	if (this->_cgi_pid){
		kill(this->_cgi_pid, SIGINT);
		int wstatus;
     	waitpid(this->_cgi_pid, &wstatus, 0);
		this->_cgi_pid = 0;
	}
}


echo_handler::echo_handler():handler(NULL, 0, 0),_res_buff(),_cgi_w(NULL),_cgi_r(NULL),_cgi_pid(0){}//not use
echo_handler::echo_handler(const echo_handler&):handler(NULL, 0, 0),_res_buff(),_cgi_w(NULL),_cgi_r(NULL),_cgi_pid(0){}//not use
const echo_handler &echo_handler::operator=(const echo_handler&){return (*this);}//not use
