#pragma once
#include <string>
#include <map>
#include "server_conf.hpp"

enum protocol{
	tcp_echo,
	http,
	https,
	undfind_protocol,
};

class port_conf{
private:
	bool is_faile;
	unsigned short _port;
	protocol _proto;
	std::string _default;
	std::map<std::string, server_conf>_servers;
	port_conf();
public:
	port_conf(std::string);
	port_conf(const port_conf &);
	const port_conf &operator=(const port_conf &);
	void marge(const port_conf &);
	unsigned short port() const;
	protocol protocol() const;
	const server_conf &server(std::string, std::string& ret_name = port_conf::buff) const;
	bool faile() const;
	~port_conf();

	static const port_conf error;
	static std::string buff;
};
