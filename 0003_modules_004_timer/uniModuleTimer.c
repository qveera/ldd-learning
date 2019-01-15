//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//
#include <linux/uaccess.h> //For current->comm
#include <linux/timer.h>


static struct timer_list uni_timer;

void uni_timer_f(struct timer_list *tl) {
	printk("%s[%d]: jiffies = %ld\n",__func__,__LINE__,jiffies);
	printk("%s[%d]: current task pid %d\n",__func__,__LINE__,current->pid);
}
static int __init uniInit(void) {
	printk("%s[%d]: uniInit @ 0x%p\n",__func__,__LINE__,uniInit);
	timer_setup(&uni_timer, uni_timer_f, 0);
	uni_timer.expires = jiffies + HZ; //One second dealy
	printk("%s[%d]: Adding timer @ jiffies %ld\n",__func__,__LINE__,jiffies);
	add_timer(&uni_timer);
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
