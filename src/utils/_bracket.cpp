#include "utils.hpp"
static inline ssize_t strfind(const char *, char);

bool utils::meta::_bracket(const char *src, const char *brac, char ignor, size_t &len){
	bool flat = true;
	if (src[len] == brac[0]){
		len++;
		flat = false;
	}
	while (1){
		ssize_t b = strfind(brac, src[len]);
		while (src[len] && b < 0){
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
		}else if(flat && (!b || !src[len]))
			return (true);
		if (!src[len] || b % 2)
			return (false);
		bool r = utils::meta::_bracket(src, brac + b, ignor, len);
		if (!r)
			return (false);
	}
	return (false);
}

static inline ssize_t strfind(const char *str, char c){
	size_t i = 0;
	for (; str[i] && str[i] != c; i++);
	if (str[i] && str[i] == c)
		return (i);
	return (-1);
}
