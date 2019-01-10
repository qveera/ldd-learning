/* uniApp.c
** Version: 1.0
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "uniCharDev.h"

#define DEV_NODE "/dev/uniCharDevOpsIOCtlPerm"

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
}

int main()
{
	int fd, i;
	ssize_t ret;
	unsigned int size;
	char c, uni_buf[400]="Hello Universe";

	printf("Opening the device node %s\n", DEV_NODE);
	fd = open( DEV_NODE, O_RDWR );
	if (fd < 0)
		printf("failed acquiring file descriptor return status %d\n",fd);


	/* write the contents into the device */
	printf("Writing into the device 1\n");
	ret = uni_write(fd, uni_buf, 15);
	printf("Writing into the device 2\n");
	ret = uni_write(fd, uni_buf, 15);

	printf("Seeking the fd\n");
	lseek(fd,0,0);

	printf("Reading from the device\n");
	uni_read(fd, uni_buf, 24);

	ioctl(fd, UNI_GET_SIZE, &size);
	printf("The size of the buffer in driver %d\n", size);

	printf("UNI_FILL_ZERO\n");
	ioctl(fd, UNI_FILL_ZERO);
	uni_read(fd, uni_buf, 24);

	printf("UNI_SET_SIZE to 24\n");
	size = 24;
	ioctl(fd, UNI_SET_SIZE, &size);
	ioctl(fd, UNI_GET_SIZE, &size);
	printf("The size of the buffer in driver %d\n", size);

	printf("Filling the buffer with 9\n");
	c = '9';
	ioctl(fd, UNI_FILL_CHAR, &c);
	uni_read(fd, uni_buf, 24);

	close(fd);
}
