#pragma once
#include <string>
#include <vector>
#include "location_conf.hpp"

class location_conf
{
private:
	bool is_faile;
	std::string root;
	std::string index;
	location_conf();
public:
	location_conf(std::string);
	location_conf(const location_conf &);
	const location_conf &operator=(const location_conf &);
	bool faile();
	~location_conf();
};
