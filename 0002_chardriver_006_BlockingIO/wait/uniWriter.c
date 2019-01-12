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
	char c, uni_buf[400]="Hello Universe, Welcome to the Quantum World! We are the Unions\n";

	printf("Opening the device node %s\n", DEV_NODE);
	fd = open( DEV_NODE, O_RDWR );
	if (fd < 0)
		printf("failed acquiring file descriptor return status %d\n",fd);


	printf("Device opened with fd %d\n",fd);
	/* write the contents into the device */
	printf("Writing into the device Buf %s\n",uni_buf);
	ret = uni_write(fd, uni_buf, 200);
	printf("Bytes written %ld\n",ret);
	close(fd);
}
