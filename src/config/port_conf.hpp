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
	int _port;
	protocol _proto;
	std::map<std::string, server_conf>_servers;
	port_conf();
public:
	port_conf(std::string);
	port_conf(const port_conf &);
	const port_conf &operator=(const port_conf &);
	void marge(port_conf);
	int port() const;
	protocol protocol() const;
	const server_conf &server(std::string) const;
	bool faile() const;
	~port_conf();

	static const port_conf error;
};
