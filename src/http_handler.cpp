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
	if (this->_req[KEY_METHOD] != "GET" \
		&& this->_req[KEY_METHOD] != "POST" \
		&& this->_req[KEY_METHOD] != "PUT" \
		&& this->_req[KEY_METHOD] != "DELETE" \
		&& this->_req[KEY_METHOD] != "HEAD")
		{this->Method_Not_Allowed(); return ;}
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
	{
//KEY_TARGETに関する処理 PATH_INFO, PATH_TRANSLATED, SCRIPT_NAME, QUERY_STRING
		std::string buff = this->_req[KEY_TARGET];
		size_t l = buff.find("?");
		if (l != UINT64_MAX){
			env.push_back(strdup(("QUERY_STRING=" + buff.substr(l + 1, buff.length())).c_str()));
			buff = buff.substr(0, l);
		}
		env.push_back(strdup(("SCRIPT_NAME=" + buff).c_str()));
	}
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
		if(i->first[0] != ':'){
			std::string meta = i->first;
			for (size_t i = 0; i < meta.length(); i++){
				if (meta[i] >= 'a' && meta[i] <= 'z')
					meta[i] += 'A' - 'a';
				else if (meta[i] == '-')
					meta[i] = '_';
			}
			env.push_back(strdup(("HTTP_" + meta + "=" + i->second).c_str()));
		}
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
			s = e;
			for (; reqline[e] && reqline[e] != ' '; e++);
			this->_req[KEY_TARGET] = reqline.substr(s, e);
			e++;
			s = e;
			for (; reqline[e] && reqline[e] != '\r' && reqline[e] != '\n'; e++);
			this->_req[KEY_VERSION] = reqline.substr(s, e);
			if (!this->_req[KEY_METHOD].length() || !this->_req[KEY_TARGET].length() || !this->_req[KEY_VERSION].length())
				{this->Bad_Request(); return ;}
		}
		while (header.length()){
			size_t s = 0,e = 0;
			for (; header[e] != '\n' && header[e + 1] != ' ' && header[e + 1] != '\t' ; e++);
			std::string field = header.substr(0, e + 1);
			header = header.substr(field.length(), header.length());
			e = field.find(":");
			if (e == UINT64_MAX){this->Bad_Request(); return;}
			std::string	field_name = field.substr(0, e);
			if (!field_name.length()){this->Bad_Request(); return;}
			for (size_t i = 0; i < field_name.length(); i++){
				if (field_name[i] >= 'A' && field_name[i] <= 'Z')
					field_name[i] += 'a' - 'A';
				else if (!(field_name[i] >= 'a' && field_name[i] <= 'z') \
					&& !(field_name[i] >= '0' && field_name[i] <= '9') \
					&& !memchr(F_NAME_ABLE,field_name[i],strlen(F_NAME_ABLE)))
				{this->Bad_Request(); return;}
			}
			s = e + 1;
			e = field.length() - 1;
			if (field[e] == '\r')
				e--;
			for (; s < e && (field[s] == ' ' || field[s] == '\t'); s++);
			if (s >= e){this->Bad_Request(); return;}
			for (; field[e] == ' ' || field[e] == '\t'; e--);
			std::string field_value = field.substr(s, e);
			std::string p_field_value;
			for (size_t i = 0; i < field_value.length(); i++)
			{
				s = i;
				if (!isprint(field_value[i]) && !(field_value[i] >= 0x80))
					{this->Bad_Request(); return;}
				else if (p_field_value.length())
					p_field_value += " ";
				i++;
				for (; i < field_value.length() && (isprint(field_value[i]) || field_value[i] >= 0x80 || field_value[i] == ' ' || field_value[i] == '\t'); i++);
				if (field_value[i - 1] == ' ' || field_value[i - 1] == '\t') {this->Bad_Request(); return;}
				p_field_value += field_value.substr(s, i);
				if (i >= field_value.length())
					break;
				if (field_value.find('\n', i) <= 0)
					i++;
				else if (field_value.find("\r\n", i) <= 0)
					i += 2;
				else
					{this->Bad_Request(); return;}
				if (field_value[i] != ' ' && field_value[i] != '\t'){this->Bad_Request(); return;}
				i++;
				for (; i < field_value.length() && (field_value[i] == ' ' || field_value[i] == '\t'); i++);
			}
			if (this->_req.find(field_name) != this->_req.end())
				this->_req[field_name] += ",";
			this->_req[field_name] += p_field_value;
		}
	}
	if (this->_req.find(KEY_BODY) != this->_req.end()){
		if (this->_body < 0){
			if (this->_req.find("transfer-encoding") != this->_req.end()){
				if(this->_req["transfer-encoding"] != "chunked")
					{this->Not_Acceptable(); return;}
				else
					this->_body = 0;
			}
			if (this->_req.find("content-length") != this->_req.end()){
				if (!utils::is_numstring(this->_req["content-length"].c_str())){this->Bad_Request(); return;}
				if (this->_req.find("transfer-encoding") != this->_req.end())
					this->_req.erase("content-length");
				else
					this->_body = atoi(this->_req["content-length"].c_str());
			}
			if ((this->_req[KEY_METHOD] == "POST" || this->_req[KEY_METHOD] == "PUT") \
				&& this->_body < 0)
				{this->Length_Required(); return;}
		}
		//bodyのパース
		if (this->_body < 0)
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

