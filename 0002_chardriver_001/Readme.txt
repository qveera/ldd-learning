Implementation Steps:
-------------------------

1) Create a device node file
	check for the free major number(cat /proc/devices) and use the mknod to create the node
	$ sudo mknod /dev/uniCharDev c 333 0
	$ ls -l /dev/uniCharDev 
	crw-r--r-- 1 root root 333, 0 Jan  8 09:51 /dev/uniCharDev

2) Implement the driver operations and register it with VFS Layer
	refer uniCharDevSkel.c
	
	for file operations structure refer /usr/src/linux-headers-4.15.0-43-generic/include/linux/fs.h

3) Insert the driver using a kernel module

Test:
-----

1) Write a driver application that initiates operations on the driver through vfs common file api
