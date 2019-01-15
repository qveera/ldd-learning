//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//
#include <linux/interrupt.h>


struct uni_struct {
	int i;
	int j;
} t_data;

void f_uni_tasklet(unsigned long int arg) {
	struct uni_struct *data = (struct uni_struct *)arg;
	printk("%s[%d]: jiffies = %ld data.i %d data.j %d\n",__func__,__LINE__,jiffies,data->i,data->j);
}

DECLARE_TASKLET(uni_tasklet, f_uni_tasklet, (unsigned long)&t_data);

static int __init uniInit(void) {
	printk("%s[%d]: uniInit @ 0x%p\n",__func__,__LINE__,uniInit);
	t_data.i = 999;
	t_data.j = 888;
	printk("%s[%d]: Scheduling the tasklet, jiffies = %ld\n",__func__,__LINE__,jiffies);
	tasklet_schedule(&uni_tasklet);
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
