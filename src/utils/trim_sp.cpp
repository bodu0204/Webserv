#include "utils.hpp"

static bool _trim_sp(const char *, size_t &, size_t &);

std::string utils::trim_sp(const std::string &src){
	size_t start = 0;
	size_t len = 0;
	if (_trim_sp(src.c_str(), start, len))
		return (src.substr(start ,len));
	return (std::string(""));
}

static bool _trim_sp(const char *src, size_t &start, size_t &len){
	start = 0;
	len = 0;
	for (; src[start] && isspace(src[start]); start++);
	for (; src[start + len]; len++);
	for (; len && isspace(src[start + len - 1]); len--);
	if (src[start + len - 1] == '\\' && src[start + len - 1]) len++;
	return (true);
}
