#include "http_handler.hpp"
#include "utils/utils.hpp"
#include <vector>
#include <sstream>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#define BUFFERSIZE 1024

void http_handler::_action(short event){
	if (event & POLL_IN){
		std::string buff;
		ssize_t r = this->_read(buff);
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
Ts(buff.c_str())
		this->_req_buff += buff;
		if (!this->_cgi_pid){
			this->_to_req();
		}
	}
	if (event & POLL_OUT && this->_res_buff.length()){
		ssize_t r = this->_write(this->_res_buff);
		if (r < 0){
			this->set_del(this->all_child());
			this->set_del(this);
			return ;
		}
Ts(this->_res_buff.substr(0,r).c_str())
		this->_res_buff = this->_res_buff.substr(r);
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
			if (WEXITSTATUS(wstatus) == 42)
				{this->Internal_Server_Error(); return;}
		}
		bool doit = !this->_res_buff.length();
		if (this->_res.find(KEY_STATUS) != this->_res.end())
			this->_res_buff += "HTTP/1.1 " + this->_res[KEY_STATUS];
		else
			this->_res_buff += STATUS_200;
		this->_res_buff += this->_res[KEY_CGIHEAD];
		if (this->_res.find(KEY_CGIBODY) != this->_res.end()){
			std::stringstream ss;
			ss << this->_res[KEY_CGIBODY].length();
			this->_res_buff += "Content-Length: " + ss.str() + CRLF;
			this->_res_buff += CRLF;
			this->_res_buff += this->_res[KEY_CGIBODY];
		}else{
			this->_res_buff += CONTENT_LENGTH_ZERO;
			this->_res_buff += CRLF;
		}
		this->_req.clear();
		this->_res.clear();
		if (doit)
			this->_action(POLL_OUT);
	}
	handler::callback_end(target);
}

void http_handler::callback(std::string str){
	if (this->_res.find(KEY_CGIBODY) == this->_res.end())
	{
		str = this->_res[KEY_CGIBUFF] + str;
		this->_res[KEY_CGIBUFF].clear();
		while (str.length())
		{
			size_t l = str.find(CRLF);
			if (l == UINT64_MAX){
				l = str.find("\n");
				if (l == UINT64_MAX)
					{this->_res[KEY_CGIBUFF] = str; return ;}
				l++;
			}
			else
				l += 2;
			std::string h = str.substr(0,l);
			str = str.substr(l);
			if (h == CRLF || h == "\n")
				{this->_res[KEY_CGIBODY] = str; return;}
			else if (!memcmp(h.c_str(), "Status:", 7))
				this->_res[KEY_STATUS] = h.substr(7);
			else
				this->_res[KEY_CGIHEAD] += h;
		}
	}
	this->_res[KEY_CGIBODY] += str;
	return ;
}

void http_handler::exec(){
	const server_conf &sc = this->_conf.server(this->_req["host"]);
	if (sc.faile()){
		if (this->_req["host"] == "teapot" && this->_req[KEY_TARGET] == "/coffee")
			this->I_m_a_teapot();
		else
			this->Bad_Request();
		return ;
	}
	std::string pattern;
	const location_conf &lc =sc.location(this->_req[KEY_TARGET], pattern);
	if (lc.faile() || this->_req[KEY_TARGET].find("/..") != UINT64_MAX){
		this->Not_Found(); return ;
	}
	if (this->_req[KEY_TARGET] == pattern)
		this->_req[KEY_TARGET] += lc.index();
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
		env.push_back(strdup(("REMOTE_USER=" + authorization.substr(i)).c_str()));
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
		std::string buff = this->_req[KEY_TARGET];
		size_t l = buff.find("?");
		if (l != UINT64_MAX){
			env.push_back(strdup(("QUERY_STRING=" + buff.substr(l + 1)).c_str()));
			buff = buff.substr(0, l);
		}
		env.push_back(strdup(("PATH_INFO=" + buff).c_str()));
		env.push_back(strdup(("PATH_TRANSLATED=" + lc.root() + buff).c_str()));
		env.push_back(strdup(("SCRIPT_NAME=" + lc.root() + buff).c_str()));
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
	env.push_back(strdup("REDIRECT_STATUS="));
	env.push_back(NULL);
	int fds[2];
	pipe(fds);
	this->_cgi_pid = fork();
	if(!this->_cgi_pid){
		char *args[2];
		args[0] = strdup(lc.cgi().c_str());
		args[1] = NULL;
		dup2(fds[STDIN_FILENO], STDIN_FILENO);
		dup2(fds[STDOUT_FILENO], STDOUT_FILENO);
		close(fds[STDIN_FILENO]);
		close(fds[STDOUT_FILENO]);
		execve(args[0], args, env.data());
		exit(42);
	}
	for (std::vector<char *>::iterator i = env.begin(); i != env.end(); i++){
		free(*i);
	}
	if (this->_cgi_pid < 0){
		close(fds[0]);
		close(fds[1]);
		this->_cgi_pid = 0;
		return ;
	}
	this->_cgi_r = new cgir_handler(this, fds[0]);
	this->_cgi_w = new cgiw_handler(this, fds[1]);
	this->set_add(this->_cgi_r);
	this->set_add(this->_cgi_w);
	if (!this->_cgi_w->set_write(this->_req[KEY_BODY], true))
		{this->set_del(this->_cgi_w);}
}

void http_handler::exec_std(const location_conf &lc){
	this->_res["Server"] = this->_req["host"];
	this->_res["Connection"] = "keep-alive";
	if (this->_req[KEY_METHOD] == "PUT"){
		this->_res[KEY_STATUS] = "201 Created";
		this->_res["Content-Location"] = this->_req[KEY_TARGET];
		this->_res["Content-Length"] = "0";
		unlink((lc.root() + this->_req[KEY_TARGET]).c_str());
		int fd = open((lc.root() + this->_req[KEY_TARGET]).c_str(), O_WRONLY | O_CREAT, S_IREAD | S_IWRITE);
		if (fd < 0)
			{this->Not_Found(); return ;}
		if(write(fd, this->_req[KEY_BODY].c_str(),this->_req[KEY_BODY].length()) < this->_req[KEY_BODY].length())
			{unlink((lc.root() + this->_req[KEY_TARGET]).c_str()); this->Internal_Server_Error(); return ;}
	}else{
	struct stat sb;
	if (stat((lc.root() + this->_req[KEY_TARGET]).c_str(), &sb) < 0)
		{this->Not_Found(); return ;}
	if (this->_req[KEY_METHOD] == "GET" || this->_req[KEY_METHOD] == "HEAD"){
		this->_res[KEY_STATUS] = "200 OK";
		if ((sb.st_mode & S_IFMT) == S_IFDIR && lc.autoindex()){
			this->_res["Content-Type"] = "text/html";
			if (this->_req[KEY_METHOD] == "GET"){
				DIR *dir;
				struct dirent *dent;
				dir = opendir((lc.root() + this->_req[KEY_TARGET]).c_str());
				if (dir == NULL)
					{this->Internal_Server_Error(); return ;}
				this->_res[KEY_BODY] = AUTO_INDEX_FMT_0 + this->_req[KEY_TARGET] + AUTO_INDEX_FMT_1 \
					+ this->_req[KEY_TARGET] + AUTO_INDEX_FMT_2;
				while ((dent = readdir(dir)) != NULL) {
					std::string name = dent->d_name;
					if (name == ".")
						continue;
					if (dent->d_type == DT_DIR)
						name += "/";
					this->_res[KEY_BODY] += AUTO_INDEX_FMT_3 + name + AUTO_INDEX_FMT_4 + name + AUTO_INDEX_FMT_5;
				}
				this->_res[KEY_BODY] += AUTO_INDEX_FMT_6;
				closedir(dir);
				{
					std::stringstream ss;
					ss<<this->_res[KEY_BODY].length();
					this->_res["Content-Length"] = ss.str();
				}
			}
		}else if((sb.st_mode & S_IFMT) == S_IFREG){
			{
				size_t m = this->_req[KEY_TARGET].rfind(".");
				if (m == UINT64_MAX)
					{m = this->_req[KEY_TARGET].length();}
				else
					m++;
				std::map<std::string,std::string>::const_iterator mi = http_handler::mime.find(this->_req[KEY_TARGET].substr(m));
				if (mi != http_handler::mime.end())
					this->_res["Content-Type"] = mi->second;
			}
			{
				std::stringstream ss;
				ss<<static_cast<long>(sb.st_size);
				this->_res["Content-Length"] = ss.str();
			}
			if (this->_req[KEY_METHOD] == "GET"){
				int fd = open((lc.root() + this->_req[KEY_TARGET]).c_str(), O_RDONLY);
				if (fd < 0)
					{this->Internal_Server_Error(); return ;}
				char buff[BUFFERSIZE + 1];
				ssize_t r = 1;
				while (r){
					r = read(fd, buff, BUFFERSIZE);
					if (fd < 0)
						{this->Internal_Server_Error(); return ;}
					buff[r] = '\0';
					this->_res[KEY_BODY] += buff;
				}
				close(fd);
			}
		}else
			{this->Not_Found(); return ;}
	}else if (this->_req[KEY_METHOD] == "DELETE"){
		this->_res[KEY_STATUS] = "200 OK";
		this->_res["Content-Length"] = "0";
		if((sb.st_mode & S_IFMT) == S_IFDIR || (sb.st_mode & S_IFMT) == S_IFREG){
			if (unlink((lc.root() + this->_req[KEY_TARGET]).c_str()) < 0)
				{this->Internal_Server_Error(); return ;}
		}else
			{this->Not_Found(); return ;}
	}else
		{this->Method_Not_Allowed(); return ;}}
	bool doit = !this->_res_buff.length();
	this->_res_buff += "HTTP/1.1 " + this->_res[KEY_STATUS] + CRLF;
	for (std::map<std::string, std::string>::iterator i = this->_res.begin(); i != this->_res.end(); i++){
		if (i->first[0] != ':')
			this->_res_buff += i->first + ": " + i->second + CRLF;
	}
	this->_res_buff += CRLF;
	if (this->_res[KEY_BODY].length())
		this->_res_buff += this->_res[KEY_BODY];
	this->_req.clear();
	this->_res.clear();
	if (doit)
		this->_action(POLL_OUT);
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
			else if (this->_req_buff.find("\n") == 0 || this->_req_buff.find("\r\n") == 0)
				crlf = 0;
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
		this->_req_buff = this->_req_buff.substr(header.length());
		if (this->_req_buff.find("\n") <= 1)
			this->_req_buff = this->_req_buff.substr(this->_req_buff.find("\n") + 1);
		if (this->_req.find(KEY_TARGET) == this->_req.end())
		{
			std::string reqline = header.substr(0, header.find("\n") + 1);
			header = header.substr(reqline.length());
			if (reqline[0] == ' ' || reqline[0] == '\t') {this->Bad_Request(); return ;}
			size_t s = 0, e = 0;
			for (; reqline[e] && reqline[e] != ' '; e++);
			this->_req[KEY_METHOD] = reqline.substr(s, e - s);
			e++;
			if (e > reqline.length()){this->Bad_Request(); return ;}
			s = e;
			for (; reqline[e] && reqline[e] != ' '; e++);
			this->_req[KEY_TARGET] = reqline.substr(s, e - s);
			e++;
			if (e > reqline.length()){this->Bad_Request(); return ;}
			s = e;
			for (; reqline[e] && reqline[e] != '\r' && reqline[e] != '\n'; e++);
			this->_req[KEY_VERSION] = reqline.substr(s, e - s);
			if (!this->_req[KEY_METHOD].length() || !this->_req[KEY_TARGET].length() || !this->_req[KEY_VERSION].length())
				{this->Bad_Request(); return ;}
		}
		while (header.length()){
			size_t s = 0,e = 0;
			for (; header[e] != '\n' || (header[e] == '\n' && (header[e + 1] == ' ' || header[e + 1] == '\t')) ; e++);
			std::string field = header.substr(0, e + 1);
			header = header.substr(field.length());
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
			if (e && field[e - 1] == '\r')
				e--;
			for (; s < e && (field[s] == ' ' || field[s] == '\t'); s++);
			if (s >= e){this->Bad_Request(); return;}
			for (; field[e] == ' ' || field[e] == '\t'; e--);
			std::string field_value = field.substr(s, e - s);
			std::string p_field_value;
			for (size_t i = 0; i < field_value.length(); i++)
			{
				s = i;
				if (!isprint(field_value[i]) && !(field_value[i] >= static_cast<char>(0x80)))
					{this->Bad_Request(); return;}
				else if (p_field_value.length())
					p_field_value += " ";
				i++;
				for (; i < field_value.length() && (isprint(field_value[i]) || field_value[i] >= static_cast<char>(0x80) || field_value[i] == ' ' || field_value[i] == '\t'); i++);
				if (field_value[i - 1] == ' ' || field_value[i - 1] == '\t') {this->Bad_Request(); return;}
				p_field_value += field_value.substr(s, i - s);
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
		if (this->_body >= 0){
			if (this->_req.find("transfer-encoding") != this->_req.end()){
				while (this->_body >= 0){
					size_t l = this->_body < this->_req_buff.length() ? this->_body : this->_req_buff.length();
					this->_req[KEY_BODY] += this->_req_buff.substr(0, l);
					this->_req_buff = this->_req_buff.substr(l);
					this->_body -= l;
					if (!this->_req_buff.length())
						return;
					l = this->_req_buff.find("\n");
					if (l == UINT64_MAX)
						return;
					l++;
					std::stringstream ss;
					ss << std::hex << this->_req_buff.substr(0, l);
					this->_req_buff = this->_req_buff.substr(l);
					ss >> this->_body;
					if (this->_body == 0){
						this->_body = -1;
					}
				}
			}
			else if (this->_req.find("content-length") != this->_req.end()){
				size_t l = this->_body < this->_req_buff.length() ? this->_body : this->_req_buff.length();
				this->_req[KEY_BODY] += this->_req_buff.substr(0, l);
				this->_req_buff = this->_req_buff.substr(l);
				this->_body -= l;
				if (this->_body == 0){
					this->_body = -1;
				}
			}
		}
		if (this->_body < 0)
			{this->exec();}
	}
	return ;
}

ssize_t http_handler::_write(const std::string &target){
	return (send(this->descriptor, target.c_str(), target.length(), 0));
}

ssize_t http_handler::_read(std::string &target){
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

http_handler::~http_handler(){
	if (this->_cgi_pid){
		kill(this->_cgi_pid, SIGINT);
		int wstatus;
    	waitpid(this->_cgi_pid, &wstatus, 0);
	}
}

void http_handler::Bad_Request(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_400;
	this->_req_buff.clear();
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::Not_Found(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_404;
	{
		std::map<std::string,std::string>::iterator it= this->_req.find("host");
		if (it != this->_req.end())
			this->_res_buff += "Server: " + it->second + CRLF;
	}
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CONTENT_LENGTH_ZERO;
	this->_res_buff += CRLF;
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::Method_Not_Allowed(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_405;
	{
		std::map<std::string,std::string>::iterator it= this->_req.find("host");
		if (it != this->_req.end())
			this->_res_buff += "Server: " + it->second + CRLF;
	}
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CONTENT_LENGTH_ZERO;
	this->_res_buff += CRLF;
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::Not_Acceptable(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_406;
	{
		std::map<std::string,std::string>::iterator it= this->_req.find("host");
		if (it != this->_req.end())
			this->_res_buff += "Server: " + it->second + CRLF;
	}
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CONTENT_LENGTH_ZERO;
	this->_res_buff += CRLF;
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::Length_Required(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_411;
	{
		std::map<std::string,std::string>::iterator it= this->_req.find("host");
		if (it != this->_req.end())
			this->_res_buff += "Server: " + it->second + CRLF;
	}
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CONTENT_LENGTH_ZERO;
	this->_res_buff += CRLF;
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::I_m_a_teapot(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_418;
	this->_res_buff += SERVER_TEAPOT;
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CONTENT_LENGTH_ZERO;
	this->_res_buff += CRLF;
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

void http_handler::Internal_Server_Error(){
	bool do_it = !this->_res_buff.length();
	this->_res_buff += STATUS_500;
	{
		std::map<std::string,std::string>::iterator it= this->_req.find("host");
		if (it != this->_req.end())
			this->_res_buff += "Server: " + it->second + CRLF;
	}
	this->_res_buff += CONNECTION_KEEP_ALIVE;
	this->_res_buff += CONTENT_LENGTH_ZERO;
	this->_res_buff += CRLF;
	this->_req.clear();
	this->_res.clear();
	if (do_it)
		this->_action(POLL_OUT);
	return ;
}

http_handler::http_handler():handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(),_body(-1){}//not use
http_handler::http_handler(const http_handler&):handler(NULL, 0, 0),_conf(port_conf::error),_req_buff(),_req(),_cgi_pid(0),_cgi_w(),_cgi_r(),_res(),_res_buff(),_info(),_body(-1){}//not use
const http_handler &http_handler::operator=(const http_handler&){return (*this);}//not use

