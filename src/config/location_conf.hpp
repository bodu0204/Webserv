#pragma once
#include <string>
#include <vector>

class location_conf{
private:
	bool is_faile;
	std::string _root;
	std::string _index;
	location_conf();
public:
	location_conf(std::string, std::string);
	location_conf(const location_conf &);
	const location_conf &operator=(const location_conf &);
	bool faile();
	~location_conf();

	static const location_conf error;
};
