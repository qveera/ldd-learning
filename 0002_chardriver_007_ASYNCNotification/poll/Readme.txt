Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor
   numbers and registers chardev with vfs and uses class_create and device_create
   to create the device inode.
   The driver should also Implemente ioctl.
	cat /proc/devices

poll API:
-----------
1) Inlcude the header file
#include <linux/wait.h>
#include <linux/poll.h>
2) Declare a wait q struct
static DECLARE_WAIT_QUEUE_HEAD(uni_waitQ);
static atomic_t data_ready;
3) In the read driver function Implement the following logic
	wait_event_interruptible(uni_waitQ, (atomic_read (&data_ready)));
	atomic_set (&data_ready, 0);
4) In write driver function set the data_ready and wake up the Q
	atomic_set(&data_ready, 1);
	wake_up_interruptible(&uni_waitQ);
5) Implement the poll function and assign it to the file ops
unsigned int uni_poll(struct file *filep, poll_table *wait) {
	poll_wait(filep, &uni_waitQ, wait);
	printk("%s[%d]: In poll at jiffies=%ld",__func__,__LINE__,jiffies);
	if(atomic_read(&data_ready)) {
		return POLLIN | POLLRDNORM;
	}
	return 0;
}

struct file_operations uni_char_dev_fops = {
    ...
	.poll    = uni_poll,
}

Test:
-----

1) Write a driver application reader and writer that initiates operations on the driver through
vfs common file api

In Reader application Implement the following for async
i) Include the headerfiles
#include <poll.h>

ii) Declare pollfd and initialize it
	struct pollfd fds[1];
	fds[0].fd = fd;
	fds[0].events = POLLIN;

iii) Implement the following logic in main
	for(;;) {
		if((ret = poll(fds, 1, timeout)) < 0) {
			perror("Failed in poll");
			exit(EXIT_FAILURE);
		}
		if(ret > 0) {
			printf("Poll returned %d revents = 0x%03x\n",ret, fds[0].revents);
			if(fds[0].revents & POLLIN) {
				ret = uni_read(fd, uni_buf, 333);
			} else {
				printf("POLLIN not set\n");
			}
		}
	}

