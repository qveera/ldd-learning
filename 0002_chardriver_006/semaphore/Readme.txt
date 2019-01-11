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

semaphores:
-----------
1) Inlcude the header file
#include <linux/semaphore.h>
2) Declare a semaphore
static struct semaphore uni_sem;
3) Initialize the semaphore in the module init
init_MUTEX_LOCKED(&uni_sem);
4) In read wait for the semaphore
down_interruptible (&uni_sem);
5) In write up the semaphore
up(&uni_sem);

Test:
-----

1) Write a driver application reader and writer that initiates operations on the driver through
vfs common file api
