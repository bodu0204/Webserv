#include "debug.h"
#include <fcntl.h>
int main (void)
{
	char buf[64];
	Tn(fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK));
	while (1)
	{
		Tn(read(STDIN_FILENO, buf, 64));
	}
	
}