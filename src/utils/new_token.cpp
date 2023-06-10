#include "utils.hpp"

static size_t _token(const char *);
static std::string get_next_token(std::string &);

std::string utils::new_token(std::string &src, bool &error, bool trim_bracket, bool trim_last, bool no_edit){
	if (!src.length()){error = true; return (std::string(""));}
	std::string buf = get_next_token(src);
    if (!buf.length()) {error = true; return (std::string(""));}
	if (no_edit){return (buf);}
    buf = utils::trim_sp(buf);
	if (trim_last){
		if (buf.length() <= 1 || buf[buf.length() - 1] != ';') {error = true; return (std::string(""));}
		buf = buf.substr(0, buf.length() - 1);
    	buf = utils::trim_sp(buf);
    	if (!buf.length()) {error = true; return (std::string(""));}
	}
	if (trim_bracket){
        if (buf.length() <= 2 || buf[0] != '{' || buf[buf.length() - 1] != '}') {error = true; return (std::string(""));}
        buf = buf.substr(1, buf.length() - 2);
    	buf = utils::trim_sp(buf);
    	if (!buf.length()) {error = true; return (std::string(""));}
	}
    if (!buf.length()) {error = true; return (std::string(""));}
    buf = utils::trim_meta(buf);
    if (!buf.length()) {error = true; return (std::string(""));}
	return (buf);
}

static std::string get_next_token(std::string &str){
	size_t len = 0;
	bool r = utils::meta::_bracket(str.c_str(), "{}''", '\\', len);
	if (r){
		std::string ret;
		if (str[0] == '{')
			ret = str.substr(0,len);
		else
			ret = str.substr(0,_token(str.substr(0, len).c_str()));
		str = str.substr(ret.length(), str.length() - ret.length());
		return(ret);
	}
	return(std::string(""));
}

static size_t _token(const char *src){
	size_t len = 0;
	for (; src[len] && isspace(src[len]); len++);
	while (src[len] && !isspace(src[len]))
	{
		if (src[len] == '\'')
			utils::meta::_bracket(src, "''", '\\', len);
		for (; src[len] && src[len] != '\'' &&!isspace(src[len]); len++);
	}
	for (; src[len] && isspace(src[len]); len++);
	if (src[len] == ';'){
		len++;
		for (; src[len] && isspace(src[len]); len++);
	}
	return (len);
}
