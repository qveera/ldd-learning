//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//

extern void exportedFunc(void); //To Tell the compiler that this function is external to us

static int __init uniInit(void) {
	printk("%s[%d] calling exportedFunc\n",__func__,__LINE__);
	exportedFunc();
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
