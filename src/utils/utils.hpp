#pragma once
#include <string>
namespace utils{
	std::string read_file(const char *);
	std::string get_next_token(std::string &);
	std::string trim_sp(const std::string &);
	std::string trim_meta(const std::string &);
	bool is_numstring(const char *);
}
