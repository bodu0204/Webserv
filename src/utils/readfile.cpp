#include <string>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

std::string read_file(const char *file_name){
	std::string ret;
	int fd = open(file_name, O_RDONLY);

	if (fd < 0)
		return(std::string(""));
	char buf[BUFFER_SIZE];
	ssize_t red = read(fd, buf, BUFFER_SIZE);
	while (red > 0)
	{
		ret += buf;
		red = read(fd, buf, BUFFER_SIZE);
	}
	if (red < 0)
		return(std::string(""));
	return (ret);
}
