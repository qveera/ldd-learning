//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//


void exportedFunc(void); //Exported functions should have the prototype

EXPORT_SYMBOL(exportedFunc); //EXPORT_SYMBOL Macro is from the module.h; Function is exported to the kernel symbol table
//EXPORT_SYMBOL_GPL(exportedFunc); //Make it available to only GPL licensed modules

void exportedFunc(void) {
	printk("%s was invoked",__func__);
}

void nonExportedFunc(void) {
	printk("%s was invoked",__func__);
}

static int __init uniInit(void) {
	printk("%s[%d]\n",__func__,__LINE__);
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
