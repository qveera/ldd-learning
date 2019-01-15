Interrupts Example
------------------

Implementation:
1) Include the Interrupts headerfile
#include <linux/interrupt.h>

2) Register your ISR with the interrupts subsystem
request_irq(irq, uniInterrupt, IRQF_SHARED, "uniInterrupt", &uniDevId);

3) Freeing the IRQ
synchronize_irq(irq);
free_irq(irq, &uniDevId);

4) Implement the ISR
static irqreturn_t uniInterrupt(int irq, void *data) {
	count++;
	printk("%s[%d]: count = %d",__func__,__LINE__,count);
	return IRQ_NONE;
}


see linux/interrupt.h for more info about the flags, etc

Testing:
insmod uniModuleInterrupt.ko irq=num //Give irq number of the input(1) or network card
check the interrupts in cat /proc/interrupts


Some info:
---------
do_irq is the default response routine registered in the interrupt vector table.
It is responsible for calling our ISR.
do_irq will create the interrupt stack for the ISR.
do_irq is the function of process 0 ie kernel.
process 0 is responsible for interrupts
process 1(init) is responsible for managing the system, io, resources etc

Linux Interrupts:
----------------
1) linux kernel configures do_irq routine as the default response function for all the external interrupts
2) do_irq is a routine of the process 0 which is responsible for allocation of the interrupt stack and 
invoking appropriate interrupt service functions

Steps Executed by do_irq:
-------------------------
1) Find the interrupt req line on which the interrupt signal was triggered
2) Look up irq descriptor table for address of the registered interrupt service routines
3) Invoke registered ISRs 
4) Enable IRQ line
5) Execute other priority work
6) Invoke process scheduler

Interrupt Latency:
------------------
Interrupt Latency is the total amount of the time spent by the system in response to the hardware interrupt

Factors contributing to interrupt Latency:
1) Hardware Latency: The amount of the time spent by the cpu
2) Kernel Latency: The amount of the time spent to look for the ISR.
3) ISR Latency
4) Soft Interrupt routine Latency
5) scheduler Latency: The amount of the time taken by the scheduler to give the processor time to some process

Designing Interrupt service routine:
-----------------------------------
Example for ethernet driver
1) Allocate buffer
2) Copy package from hardware
3) process physical header
4) queue packet for protocol layers

After considering the below Tips
1) schedule the buttom half / deffered function

To reduce the latenc below are the guidelines:
---------------------------------------------
1) Avoid calling dynamic memory allocation routines because they will take variable time to allocate memory
2) Avoid transferring data between two buffer blocks synchronously
3) Avoid contending for access on global data structures because they need aquire locks to access
4) Avoid operations on user space address spaces
5) Avoid calls to scheduler
6) Avoid calls to which are non atomic

Tips:
1) Use pre-allocated buffers
2) Consider using DMA whenever data needs to be transferred between device memory
3) Consider using per cpu data whenever needed
4) Identify non critical work and use appropriate deffered routines to execute when system is ideal

Links:
https://www.ibm.com/developerworks/library/l-tasklets/index.html
https://notes.shichao.io/lkd/ch8/
