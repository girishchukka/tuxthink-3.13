#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "ioctl_basic.h"

int main()	{
	int fd;
	fd = open ("/dev/temp", O_RDWR);
	if(fd==-1)	{
		printf("Error in opening file\n");
		exit(-1);
	}
	ioctl(fd, IOCTL_HELLO);
	close(fd);
}