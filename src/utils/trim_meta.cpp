#include "utils.hpp"

std::string utils::trim_meta(const std::string &src){
	std::string ret;
	size_t l = 0;
	while (l < src.length()){
		switch (src[l])
		{
		case '\\':
			l++;
			if (l < src.length()){
				ret += src[l];
				l++;
			}
			break;
		case '\'':
			l++;
			break;
		default:
			size_t i = 0;
			for (; l + i< src.length() && src[l + i] != '\'' && src[l + i] != '\\'; i++);
			ret += src.substr(l, i);
			l += i;
			break;
		}
	}
	return (ret);
}
