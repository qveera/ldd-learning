Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor numbers and registers chardev with vfs
   And uses class_create and device_create to create the device inode.
	cat /proc/devices

Test:
-----

1) Write a driver application that initiates operations on the driver through vfs common file api



Dynamic Registration of the major and minor numbers and inode creation
---------------------------------------------------

Used APIs for registration
--------------------------

	//Aquire the major and minor number dynamically
	alloc_chrdev_region(&uni_dev, 0, count, DEV_NAME);

	//Alloc the cdev
	uni_cdev = cdev_alloc();

	//Initialize the cdev
	cdev_init(uni_cdev, &uni_char_dev_fops);

	//Add the cdev to the vfs
	ret = cdev_add(uni_cdev, uni_dev, count);

	//Create the class and device
	uni_class = class_create(THIS_MODULE, "UNI_VIRTUAL");
    device_create(uni_class, NULL, uni_dev, NULL, "%s", DEV_NAME);


Used APIs for de-registration
-----------------------------

	//Remove the device and device class
	device_destroy(uni_class, uni_dev);
    class_destroy(uni_class);

	//Delete the char dev
	cdev_del(uni_cdev);

	//Release the major and minor numbers
	unregister_chrdev_region(uni_dev, count);
