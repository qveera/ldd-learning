/* uniApp.c
** Version: 1.0
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h> //For singal
#include <fcntl.h> //For fcntl

#include "uniCharDev.h"
int fd;
char uni_buf[400]="Hello Universe";

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
}

void uni_signal_handler(int signal) {
	printf("%s[%d]: Got Signal %d\n",__func__,__LINE__,signal);

	printf("Trying to read from the device\n");
	uni_read(fd, uni_buf, 333);
}

int main()
{
	int i, flags;
	ssize_t ret;
	unsigned int size;
	char dummy[400];

	bzero(uni_buf, 4000);

	printf("Opening the device node %s\n", DEV_NODE);
	fd = open( DEV_NODE, O_RDWR );
	if (fd < 0)
		printf("failed acquiring file descriptor return status %d\n",fd);


	printf("Device opend with fd %d\n",fd);
	fcntl(fd, F_SETOWN, getpid()); /* set this process as owner  to get calls for signal handler*/
	flags = fcntl(fd, F_GETFL); /* get file flags now */
	fcntl(fd, F_SETFL, flags| FASYNC); /* enable async notice */
	signal(SIGIO, uni_signal_handler); /* install signal handler */

	printf("Type CTRL+C to exit, this is just for while loop\n");
	while(1);
	close(fd);
}
