//These are the basic header files which we need to include in our modules
#include <linux/module.h> 	//Kmod interface
#include <linux/version.h>	//kernel version stamp
#include <linux/kernel.h>	//Resolve kernel symbol calls
#include <linux/init.h>		//
#include <linux/moduleparam.h> //for MOUDLE_PARAM Macros
#include <linux/interrupt.h>


static int irq = 0, count, uniDevId;
//module_param(irq, int, S_IRUGO); //READ_ONLY
module_param(irq, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(vvn, "Input the IRQ number");
MODULE_SUPPORTED_DEVICE("NULL");


static irqreturn_t uniInterrupt(int irq, void *data) {
	count++;
	printk("%s[%d]: count = %d",__func__,__LINE__,count);
	return IRQ_NONE; //Because we are just observing irq
}



static int __init uniInit(void) {
	printk("%s[%d]: Registering the ISR on IRQ %d\n",__func__,__LINE__,irq);
	if(request_irq(irq, uniInterrupt, IRQF_SHARED, "uniInterrupt", &uniDevId)) {
		printk("%s[%d]: Failed to register the irq\n",__func__,__LINE__);
		return -1;
	}
	return 0;
}

static void __exit  uniExit(void) {
	printk("%s[%d]: Unregistering the IRQ\n",__func__,__LINE__);
	synchronize_irq(irq);
	free_irq(irq, &uniDevId);
}



module_init(uniInit);
module_exit(uniExit);


/*kernel module comments*/
MODULE_AUTHOR("UNIONS");
MODULE_DESCRIPTION("Module Version 1");
MODULE_LICENSE("GPL");
