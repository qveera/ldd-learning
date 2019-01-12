Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor
   numbers and registers chardev with vfs and uses class_create and device_create
   to create the device inode.
   The driver should also Implemente ioctl.
	cat /proc/devices

complete API:
-----------
1) Inlcude the header file
#include <linux/completion.h>
2) Declare a completion struct
static DECLARE_COMPLETION(uni_complete);
3) In read wait for the complete
wait_for_completion(&uni_complete);
4) In write wake up the readers by complete
complete(&uni_complete);

Test:
-----

1) Write a driver application reader and writer that initiates operations on the driver through
vfs common file api
