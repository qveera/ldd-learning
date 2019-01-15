//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//
#include <linux/interrupt.h>
#include<linux/workqueue.h>
#include <linux/uaccess.h> //For current->comm

int count=0;
void uni_wq_func(void *arg);

//Simple work struct
DECLARE_WORK(uni_wq, (work_func_t)uni_wq_func);

void uni_wq_func(void *arg) {
	printk("%s[%d]: current->comm %s\n",__func__,__LINE__,current->comm);
	if(count < 10) {
		count++;
		schedule_work(&uni_wq);
	}
}

static int __init uniInit(void) {
	printk("%s[%d]: uniInit @ 0x%p\n",__func__,__LINE__,uniInit);
	printk("%s[%d]: Scheduling the work on the kernel global worke queue\n",__func__,__LINE__);
	schedule_work(&uni_wq);
	return 0;
}

static void __exit  uniExit(void) {
	printk("%s[%d]: uniExit @ 0x%p\n",__func__,__LINE__,uniExit);
}

module_init(uniInit);
module_exit(uniExit);


/*kernel module comments*/
MODULE_AUTHOR("UNIONS");
MODULE_DESCRIPTION("Module Version 1");
MODULE_LICENSE("GPL");
