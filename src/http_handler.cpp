#include "http_handler.hpp"
#include "utils/utils.hpp"
#include <vector>
#include <sstream>
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

void http_handler::exec(){
	const server_conf &sc = this->_conf.server(this->_req.at("server"));
	if (sc.faile()){
		if (this->_req.at("server") == "teapot" && this->_req.at(KEY_TARGET) == "/coffee")
			this->I_m_a_teapot();
		else
			this->Bad_Request();
		return ;
	}
	const location_conf &lc =sc.location(this->_req.at(KEY_TARGET));
	if (lc.faile() || this->_req[KEY_TARGET].find("/..") != UINT64_MAX){
		this->Not_Found(); return ;
	}
	if (this->_req[KEY_TARGET] != "GET" \
		&& this->_req[KEY_TARGET] != "POST" \
		&& this->_req[KEY_TARGET] != "PUT" \
		&& this->_req[KEY_TARGET] != "DELETE" \
		&& this->_req[KEY_TARGET] != "HEAD"){
		this->Not_Implemented(); return ;
	}
	if (lc.cgi().length())
		this->exec_CGI(lc);
	else
		this->exec_std(lc);
	return ;
}

void http_handler::exec_CGI(const location_conf &lc){
	std::vector<char*>env;
	if (this->_req.find("authorization") != this->_req.end()){
		std::string authorization;
		authorization.swap(this->_req["authorization"]);
		size_t i = 0;
		for (; authorization[i] && authorization[i] != ' ' && authorization[i] != '\t';i++);
		env.push_back(strdup(("AUTH_TYPE=" + authorization.substr(0,i)).c_str()));
		for (; authorization[i] && authorization[i] == ' ' && authorization[i] == '\t';i++);
		env.push_back(strdup(("REMOTE_USER=" + authorization.substr(i,authorization.length())).c_str()));
	}
	this->_req.erase("authorization");
	if (this->_req[KEY_METHOD] == "POST" || (this->_req[KEY_METHOD] == "DELETE" && (this->_req.find("content-length") != this->_req.end() || this->_req["transfer-encoding"] == "chunked"))){
		std::stringstream ss;
		ss << this->_req[KEY_BODY].length();
		std::string length;
		ss >> length;
		env.push_back(strdup(("CONTENT_LENGTH=" + length).c_str()));
		env.push_back(strdup(("CONTENT_TYPE=" + this->_req["content-type"]).c_str()));
	}
	else
		this->_req[KEY_BODY] = std::string();
	this->_req.erase("content-length");
	this->_req.erase("transfer-encoding");
	this->_req.erase("content-type");
	env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
	//KEY_TARGETに関する処理 PATH_INFO, PATH_TRANSLATED, SCRIPT_NAME, QUERY_STRING
	{
		unsigned addr = ntohl(this->_info.sin_addr.s_addr);
		std::stringstream ss;
		ss << ((addr & 0xff000000) >> 24) << "." << ((addr & 0xff0000) >> 16) << "." << ((addr & 0xff00) >> 8) << "." << (addr & 0xff);
		env.push_back(strdup(("REMOTE_ADDR=" + ss.str()).c_str()));
	}
	if (this->_req.find("user-agent") != this->_req.end()){
		env.push_back(strdup(("REMOTE_IDENT=" + this->_req["user-agent"]).c_str()));
	}
	this->_req.erase("user-agent");
	env.push_back(strdup(("REQUEST_METHOD=" + this->_req[KEY_METHOD]).c_str()));
	env.push_back(strdup(("SERVER_NAME=" + this->_req["server"]).c_str()));
	{
		std::stringstream ss;
		ss << this->_conf.port();
		env.push_back(strdup(("SERVER_PORT=" + ss.str()).c_str()));
	}
	env.push_back(strdup("SERVER_PROTOCOL=HTTP/1.1"));
	env.push_back(strdup("SERVER_SOFTWARE=Webserv/1.0"));
	for (std::map<std::string, std::string>::iterator i = this->_req.begin(); i != this->_req.end(); i++){
		if(i->first[0] != ':')
			env.push_back(strdup((http_handler::to_meta_var(i->first) + "=" + i->second).c_str()));
	}


}

void http_handler::exec_std(const location_conf &lc){

}

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
	if (this->_req.find(KEY_BODY) != this->_req.end()){
		//
		std::string method = this->_req[KEY_METHOD];
		if (method == "GET" || method == "HEAD" \
		|| (method == "DELETE" && this->_body < 0) \
		|| (method == "POST" && this->_body < 0))
			this->exec();
	}
	return ;
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
	handler(parent, descriptor, POLL_IN|POLL_OUT, 40),_conf(conf),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(info),_body(-1){}

http_handler::~http_handler(){}

void http_handler::Bad_Request(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_400;
	this->_res_buff += CRLF;
	this->_req_buff.clear();
	this->_req.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::Not_Found(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_404;
	this->_res_buff += CRLF;
	this->_res_buff += "Server: " + this->_res.at("server") + CRLF;
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CRLF;
	this->_req.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::I_m_a_teapot(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_418;
	this->_res_buff += SERVER_TEAPOT;
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CRLF;
	this->_req.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

http_handler::http_handler():handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(),_body(-1){}//not use
http_handler::http_handler(const http_handler&):handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(),_body(-1){}//not use
const http_handler &http_handler::operator=(const http_handler&){return (*this);}//not use

std::string http_handler::to_meta_var(const std::string &src){
	std::string ret = "HTTP_" + src;
	for (size_t i = 0; i < ret.length(); i++){
		if (ret[i] >= 'a' && ret[i] <= 'z')
			ret[i] += 'A' - 'a';
		else if (ret[i] == '-')
			ret[i] = '_';
	}
}

