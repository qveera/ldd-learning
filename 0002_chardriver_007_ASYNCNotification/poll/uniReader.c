/* uniApp.c
** Version: 1.0
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>

#include "uniCharDev.h"

/*Implemented  own write fucntion so that we can use ioctl here to Write
in later examples of the driver */
int uni_write(int fd, char *buf, int size) {
	return write(fd,buf,size);
}

/*Implemented  own read fucntion so that we can use ioctl here to read
in later examples of the driver */
int uni_read(int fd, char *buf, int size) {
	ssize_t ret;
	ret = read(fd,buf,size);
	if(ret < 0) {
		printf("read operation failed with return status %ld\n",ret);
	} else {
		//Since our first string ends with \0 %s only shows till that
		printf("buffer[%ld] = %s\n",ret,buf);
	}
	lseek(fd,0,0);
	return ret;
}

int main()
{
	int fd, i;
	struct pollfd fds[1];
	int timeout = 1000; //timeout for poll
	ssize_t ret;
	unsigned int size;
	char c, uni_buf[400]="Hello Universe";

	printf("Opening the device node %s\n", DEV_NODE);
	fd = open( DEV_NODE, O_RDWR );
	if (fd < 0)
		printf("failed acquiring file descriptor return status %d\n",fd);


	printf("Device opend with fd %d\n",fd);
	fds[0].fd = fd;
	fds[0].events = POLLIN;

	for(;;) {
		if((ret = poll(fds, 1, timeout)) < 0) {
			perror("Failed in poll");
			exit(EXIT_FAILURE);
		}
		if(ret > 0) {
			printf("Poll returned %ld revents = 0x%03x\n",ret, fds[0].revents);
			if(fds[0].revents & POLLIN) {
				ret = uni_read(fd, uni_buf, 333);
			} else {
				printf("POLLIN not set\n");
			}
		}  else {
            printf("poll timed out in %d milliseconds on %s.\n",
                    timeout, DEV_NODE);
        }
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
