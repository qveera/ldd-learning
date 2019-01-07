//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//
#include <linux/moduleparam.h> //for MOUDLE_PARAM Macros

static int vvn = 0;
module_param(vvn, int, S_IRUGO); //READ_ONLY
//module_param(vvn, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(vvn, "This should be something which will describe the param");
MODULE_SUPPORTED_DEVICE("NULL");


void uniFunc(void) {
	printk("%s was invoked",__func__);
	printk("vvn = %d",vvn);
}



static int __init uniInit(void) {
	printk("%s[%d]\n",__func__,__LINE__);
	uniFunc();
	return 0;
}

static void __exit  uniExit(void) {
	printk("%s[%d]\n",__func__,__LINE__);
}



module_init(uniInit);
module_exit(uniExit);


/*kernel module comments*/
MODULE_AUTHOR("UNIONS");
MODULE_DESCRIPTION("Module Version 1");
MODULE_LICENSE("GPL");
