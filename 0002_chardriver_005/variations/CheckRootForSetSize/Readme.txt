Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor
   numbers and registers chardev with vfs and uses class_create and device_create
   to create the device inode.
   The driver should also Implemente ioctl.
	cat /proc/devices

IOCTL:
------------
Before implementing ioctl request commands one should understand all the special
operations supported by the deivce
1) Identify the special operations possible on your devices
2) Create request commands for the special operations
3) Add support for these request commands in your driver

Refer uniCharDev.h for complete Implementation of the above steps

Implementation of the ioctl function:
1) Verify the arguments
2) Implement the switch case for the commands

Test:
-----

1) Write a driver application that initiates operations on the driver through
vfs common file api
