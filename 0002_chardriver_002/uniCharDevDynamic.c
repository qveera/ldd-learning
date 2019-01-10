#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
//#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h> //For Kmalloc


#define DEV_NAME "uniCharDevDynamic" //This will be shown in cat /proc/devices
#define MAJOR_NO 333
#define MINOR_NO 0
#define MAX_LENGTH 4000
#define SUCCESS 0

static char *char_device_buf;
struct cdev *uni_cdev;
dev_t uni_dev;
int count=1,inuse=0;


ssize_t uni_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos) {
	printk(KERN_INFO "%s[%d]: buf size %lu\n",__func__,__LINE__,lbuf);
	return lbuf;
}

ssize_t uni_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos) {
	printk(KERN_INFO "%s[%d]: buf size %lu\n",__func__,__LINE__,lbuf);
	return lbuf;
}

int uni_open(struct inode *inode, struct file *file) {
	if(inuse)
	{
		printk(KERN_INFO "%s[%d]: Device busy %s\n",__func__,__LINE__,DEV_NAME);
		return -EBUSY;
	}
	inuse=1;
	printk(KERN_INFO "%s[%d]: Open operation invoked\n",__func__,__LINE__);
	return SUCCESS;
}

int uni_release(struct inode *inode, struct file *file) {
	inuse=0;
	printk(KERN_INFO "%s[%d]: Close/release operation invoked\n",__func__,__LINE__);
	return SUCCESS;
}

struct file_operations uni_char_dev_fops = {
	.owner   = THIS_MODULE,
	.read    = uni_read,
	.write   = uni_write,
	.open    = uni_open,
	.release = uni_release,
};

static int __init uni_char_dev_init(void) {
	int ret = 0;

	//Aquire the major and minor number dynamically
	if(alloc_chrdev_region(&uni_dev, 0, count, DEV_NAME) < 0) {
		printk (KERN_ERR "%s[%d]: Failed to reserve major/minor range\n",__func__,__LINE__);
		return -1;
	}

	if(!(uni_cdev = cdev_alloc())) {
		printk (KERN_ERR "%s[%d]: cdev_alloc() failed\n",__func__,__LINE__);
		unregister_chrdev_region(uni_dev, count);
		return -1;
 	}

	//Initialize the cdev
	cdev_init(uni_cdev, &uni_char_dev_fops);

	//Add the cdev to the vfs
	ret = cdev_add(uni_cdev, uni_dev, count);
	if(ret < 0) {
		printk("%s[%d]: Eror while registering the device\n",__func__,__LINE__);
		return ret;
	}
	printk(KERN_INFO "%s[%d]: Successfully registered the device\n",__func__,__LINE__);
	printk(KERN_INFO "%s[%d]: Major number = %d, Minor number = %d\n",__func__,__LINE__,MAJOR (uni_dev),MINOR (uni_dev));

	//Memory ALlocation
	char_device_buf = (char *)kmalloc(MAX_LENGTH,GFP_KERNEL);
	return 0;
}

static void __exit  uni_char_dev_exit(void) {
	//Delete the char dev
	cdev_del(uni_cdev);

	//Release the major and minor numbers
	unregister_chrdev_region(uni_dev, count);
	kfree(char_device_buf);

	printk("%s[%d]: Successfully unregistered\n",__func__,__LINE__);
}



module_init(uni_char_dev_init);
module_exit(uni_char_dev_exit);


/*kernel module comments*/
MODULE_AUTHOR("UNIONS");
MODULE_DESCRIPTION("CharDevDriver Version 1");
MODULE_LICENSE("GPL");
