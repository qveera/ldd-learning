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

int main()
{
	int fd, i;
	ssize_t ret;
	char uni_buf[24]="Hello Universe";

	fd = open( "/dev/uniCharDevOps", O_RDWR );
	if (fd < 0)
		printf("failed acquiring file descriptor return status %d\n",fd);


	/* write the contents into the device */
	ret = write(fd, uni_buf, 15);
	lseek(fd,0,0);
	ret= read(fd, uni_buf, 24);
	if(ret<0)
		printf("read operation failed with return status %ld\n",ret);
	else {
		printf("buffer[%ld] = %s\n",ret,uni_buf);
	}
	close(fd);
}
