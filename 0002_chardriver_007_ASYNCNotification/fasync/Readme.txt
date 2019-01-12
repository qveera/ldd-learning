Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor
   numbers and registers chardev with vfs and uses class_create and device_create
   to create the device inode.
   The driver should also Implemente ioctl.
	cat /proc/devices

NOTE: semaphore are no more in kernl, they were replaced by mutex
If you want to run this use the old kernel 2.6
*I did not tested this*

fasync API:
-----------
1) Declare a completion struct
    static struct fasync_struct *async_struct;
2) Implement fasync function and assign it to fops
	.fasync  = uni_fasync,

int uni_fasync(int fd, struct file *filep, int mode) {
	printk(KERN_INFO "%s[%d]: In Driver",__func__,__LINE__);
	fasync_helper(fd, filep, mode, &async_struct);
	return 0;
}

4) In write wake up the readers by using the killasync
/* infor the asynchronous readers */
if(async_struct) {
	printk("%s[%d]: Sending SIGIO\n",__func__,__LINE__);
	kill_fasync(&async_struct, SIGIO, POLL_IN);
}

Test:
-----

1) Write a driver application reader and writer that initiates operations on the driver through
vfs common file api

In Reader application Implement the following for async
i) Include the headerfiles
#include <signal.h> //For singal
#include <fcntl.h> //For fcntl

ii) Implement the signal handler
void uni_signal_handler(int signal) {
	printf("%s[%d]: Got Signal %d\n",__func__,__LINE__,signal);

	printf("Trying to read from the device\n");
	uni_read(fd, uni_buf, 333);
}

iii) And add the following in the main after the open function
	fcntl(fd, F_SETOWN, getpid()); /* set this process as owner */
	flags = fcntl(fd, F_GETFL); /* get file flags now */
	fcntl(fd, F_SETFL, flags| FASYNC); /* enable async notice */
	signal(SIGIO, uni_signal_handler); /* install signal handler */

