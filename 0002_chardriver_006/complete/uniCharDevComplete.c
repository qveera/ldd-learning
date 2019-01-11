#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h> //For copy_to_user
#include <linux/cdev.h>
#include <linux/slab.h> //For Kmalloc
#include <linux/completion.h> //For Complete API

#include "uniCharDev.h"



#define MAX_LENGTH 400
#define SUCCESS 0

static char *char_device_buf;
struct cdev *uni_cdev;
dev_t uni_dev;
int count=1,inuse_count=0,cur_buf_size=MAX_LENGTH;
static struct class *uni_class;

static DECLARE_COMPLETION(uni_complete);

static int print_buf(void) {
	//int i = 0;
	printk("%s[%d]: %s\n",__func__,__LINE__,char_device_buf);
	//for(i = 0; i < cur_buf_size; printk("%c",char_device_buf[i++]));
	return 0;
}

ssize_t uni_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos) {
	int maxbytes; //Number of bytes from ppos to MAX_LENGTH
	int bytes_to_read; //Number of bytes to read
	int nbytes; //Number of bytes actually ready

	printk(KERN_INFO "%s[%d]: lbuf=%lu ppos=%lld\n",__func__,__LINE__,lbuf,*ppos);
	//1) Validate the read request
	maxbytes = MAX_LENGTH - *ppos;
	//Truncate the request if it is more than maxbytes
	if(maxbytes < lbuf)
		bytes_to_read = maxbytes;
	else
		bytes_to_read = lbuf;

	if(bytes_to_read == 0) {
		printk("%s[%d]: Reached end of the device",__func__,__LINE__);
		return -ENOSPC; //Causes read() to return EOF
	}

	printk (KERN_INFO "%s[%d]: process %i going to wait state\n",__func__,__LINE__,current->pid);
	wait_for_completion(&uni_complete);
	printk (KERN_INFO "%s[%d]: process %i (%s) resuming\n",__func__,__LINE__,current->pid,current->comm);

	nbytes = bytes_to_read - copy_to_user(buf, /*To*/
				char_device_buf + *ppos, /*From*/
				bytes_to_read); /*how many bytes*/

	*ppos += nbytes;
	return nbytes;
}

ssize_t uni_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos) {
	int nbytes; //Number of bytes to written
	int bytes_to_write; //Number of bytest to Write
	int maxbytes; //Max number of bytes that can be written

	printk(KERN_INFO "%s[%d]: lbuf=%lu ppos=%lld\n",__func__,__LINE__,lbuf,*ppos);

	maxbytes = MAX_LENGTH - *ppos;

	if(maxbytes > lbuf)
		bytes_to_write = lbuf;
	else
		bytes_to_write = maxbytes;

	if(bytes_to_write == 0) {
		printk("%s[%d]: Reached end of the device",__func__,__LINE__);
		return -ENOSPC; //Returns EOF at write
	}

	nbytes = bytes_to_write - copy_from_user(char_device_buf + *ppos /*To*/,
					buf /*From*/,
					bytes_to_write/*how many bytes*/);

	*ppos += nbytes;
	print_buf();
	printk (KERN_INFO "%s[%d]: process %i (%s) wake up readers\n",__func__,__LINE__,current->pid, current->comm);
	complete(&uni_complete);
	return nbytes;
}

loff_t uni_llseek(struct file *file, loff_t offset, int orig) {
	loff_t testpos;
	printk("%s[%d]: offset=%lld orig=%d:",__func__,__LINE__,offset,orig);
	switch (orig) {
		case 0/* SEEK_SET */:
			testpos = offset;
			break;
		case 1/* SEEK_CUR */:
			testpos = file->f_pos + offset;
			break;
		case 2/* SEEK_END */: //This is not possible in our case
			testpos = MAX_LENGTH + offset;
			break;
		default:
			return -EINVAL;
	}

	//Some basic checkings
	testpos = testpos < MAX_LENGTH ? testpos : MAX_LENGTH;
	testpos = testpos >= 0 ? testpos : 0;
	file->f_pos = testpos;
	return testpos;
}

int uni_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "%s[%d]: ",__func__,__LINE__);
	printk(KERN_INFO "%s[%d]: current process id %d",__func__,__LINE__,current->pid);
	printk(KERN_INFO "%s[%d]: major_no=%d minor_no=%d from inode",__func__,__LINE__,imajor(inode),iminor(inode));
	printk(KERN_INFO "%s[%d]: module_refcount %d",__func__,__LINE__,module_refcount(THIS_MODULE));
	inuse_count++;
	printk(KERN_INFO "%s[%d]: Open operation invoked inuse_count %d\n",__func__,__LINE__,inuse_count);
	printk("%s[%d]: UNI_FILL_ZERO = %u",__func__,__LINE__,UNI_FILL_ZERO);
	printk("%s[%d]: UNI_FILL_CHAR = %lu",__func__,__LINE__,UNI_FILL_CHAR);
	printk("%s[%d]: UNI_SET_SIZE  = %lu",__func__,__LINE__,UNI_SET_SIZE);
	printk("%s[%d]: UNI_GET_SIZE  = %lu",__func__,__LINE__,UNI_GET_SIZE);
	return SUCCESS;
}

int uni_release(struct inode *inode, struct file *file) {
	inuse_count--;
	printk(KERN_INFO "%s[%d]: Close/release operation invoked inuse_count %d\n",__func__,__LINE__,inuse_count);
	return SUCCESS;
}


struct file_operations uni_char_dev_fops = {
	.owner   = THIS_MODULE,
	.llseek  = uni_llseek,
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

	//Create the class and device
	uni_class = class_create(THIS_MODULE, "UNI_VIRTUAL");
	device_create(uni_class, NULL, uni_dev, NULL, "%s", DEV_NAME);

	//Memory ALlocation
	char_device_buf = (char *)kmalloc(MAX_LENGTH,GFP_KERNEL);
	memset(char_device_buf, '\0', cur_buf_size);

	return 0;
}

static void __exit  uni_char_dev_exit(void) {

	//Remove the device and device class
	device_destroy(uni_class, uni_dev);
	class_destroy(uni_class);

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
