Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor numbers and registers chardev with vfs
2) Finding out what major and minor numbers were aquired
	cat /proc/devices
3) Create mknod using the dynamic major and minor numbers

Test:
-----

1) Write a driver application that initiates operations on the driver through vfs common file api




Dynamic Registration of the major and minor numbers
---------------------------------------------------
Used APIs for registration
---------------------------

	//Aquire the major and minor number dynamically
	alloc_chrdev_region(&uni_dev, 0, count, DEV_NAME);

	//Alloc the cdev
	uni_cdev = cdev_alloc();

	//Initialize the cdev
	cdev_init(uni_cdev, &uni_char_dev_fops);

	//Add the cdev to the vfs
	ret = cdev_add(uni_cdev, uni_dev, count);


Used APIs for de-registration
---------------------------

	//Delete the char dev
	cdev_del(uni_cdev);

	//Release the major and minor numbers
	unregister_chrdev_region(uni_dev, count);
