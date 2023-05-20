#include "utils.hpp"
static bool _bracket(const char *, const char *, char , size_t &);
static size_t _token(const char *);
static inline ssize_t strfind(const char *, char);

std::string utils::get_next_token(std::string &str){
	size_t len = 0;
	bool r = _bracket(str.c_str(), "{}''", '\\', len);
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

static bool _bracket(const char *src, const char *brac, char ignor, size_t &len){
	bool flat = true;
	if (src[len] == brac[0]){
		len++;
		flat = false;
	}
	while (1){
		ssize_t b = strfind(brac, src[len]);
		while (src[len] && src[len] != brac[1] && b < 0){
			if(src[len] == ignor){
				if (src[len + 1])
					len++;
				else
					return (false);
			}
			len++;
			b = strfind(brac, src[len]);
		}
		if (src[len] == brac[1]){
			len++;
			return (true);
		}else if (!src[len] || b % 2)
			return (false);
		if(flat && !b)
			return (true);
		bool r = _bracket(src, brac + b, ignor, len);
		if (!r)
			return (false);
	}
	return (false);
}

static size_t _token(const char *src){
	size_t len = 0;
	for (; src[len] && isspace(src[len]); len++);
	while (src[len] && !isspace(src[len]))
	{
		if (src[len] == '\'')
			_bracket(src, "''", '\\', len);
		for (; src[len] && src[len] != '\'' &&!isspace(src[len]); len++);
	}
	for (; src[len] && isspace(src[len]); len++);
	return (len);
}

static inline ssize_t strfind(const char *str, char c){
	size_t i = 0;
	for (; str[i] && str[i] != c; i++);
	if (str[i] && str[i] == c)
		return (i);
	return (-1);
}
