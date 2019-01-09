Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor numbers and registers chardev with vfs
   And uses class_create and device_create to create the device inode.
	cat /proc/devices

Test:
-----

1) Write a driver application that initiates operations on the driver through vfs common file api





Operations Call flow
--------------------
driver open function:
	open --> sys_open --> driver_open
	driver_open function is the entry point for the application to access the driver functions
	This is the process context routine
	driver_open function should take care of the policy and permissions related to the open
		1) Verify the caller application request
		2) Enforce the policy of the driver
		3) Verify target hardwares is ready for the operations to perform(push to wakeup if needed)
		4) Allocate resources if needed and initialize them

Driver release function:
	close -> sys_release -> driver_release
	whatever we done in the open call we need to undo that.
	All the destroctor operations will be done here
	This is the process context routine

Driver read function:
	read -> sys_read -> driver_read
	process context
	ssize_t uni_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
	args:	file = file descriptor
		buf  = buffer address to where the driver needs to update
		lbuf = size of the buffer
		ppos = file offset position; this is the position from where we start reading

	1) We need to verify the read request whether we have the data or not based on the current offset(ppos)
	2) Read data from the device (In our case read the data from the buffer)
	3) Process data (Like processing the data to make the application understand the data)
	4) Transfer to app buffer
	5) Increment *ppos
	6) return number of bytes transferred

Driver write function:
	write -> sys_write -> driver_write
	process context
	ssize_t uni_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos) {
	args:	file = file descriptor
		buf  = buffer address from where the driver needs to read and update the driver buffer
		lbuf = size of the buffer
		ppos = file offset position; this is the position from where we start writing

	1) Validate the write request
	2) copy data from the application
	3) Process data
	4) Write the data to the device
	5) Increment *ppos
	6) return number of bytes written

Driver llseek:
	fseek/seek -> sys_lseek -> driver_llseek
	process context
	man lseek for more info
	loff_t uni_llseek(struct file *file, loff_t offset, int orig);
	args:	file   = file descriptor
		offest = offset that we need to add
		orig   = from where we need to add the offset





-----------------
Used APIs for registration

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

	//Remove the device and device class
	device_destroy(uni_class, uni_dev);
    class_destroy(uni_class);

	//Delete the char dev
	cdev_del(uni_cdev);

	//Release the major and minor numbers
	unregister_chrdev_region(uni_dev, count);
