#pragma once
#include <string>
namespace utils{
	std::string read_file(const char *);
	std::string new_token(std::string &, bool &, bool trim_bracket = false, bool trim_last = false, bool no_edit = false);
	std::string trim_sp(const std::string &);
	std::string trim_meta(const std::string &);
	bool is_numstring(const char *);
}
