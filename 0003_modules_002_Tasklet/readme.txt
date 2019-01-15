Tasklets:
---------
1) Built on top of softirqs
2) Represented by two softirqs (HI_SOFTIRQ, TASKLET_SOFTIRQ)
3) Each tasklet has a tasklet_struct structure
4) Created statically or dynamically
5) Less restrictive synchronization requirements
6) Scheduled with tasklet_schedule
7) Executed with other softirqs

Implementations:
---------------
1) Include the header file
#include <linux/interrupt.h>
2) Implementation of the tasklet
#define DECLARE_TASKLET(name, func, data);
	or
struct tasklet_struct uni_tasklet;
tasklet_init(uni_tasklet, func, data);
3) Implemente BH routine
void (*func)(unsigned long);
4) Schedule the tasklet
tasklet_schedule(&uni_tasklet);
	or
tasklet_hi_schedule(&uni_tasklet);


Execution Policy:
----------------
Tasklets are executed using the same policy ie applied for softirqs since interrupt subsystem of kernel
views a tasklet either an instance of type HI_SOFTIRQ or TASKLET_SOFTIRQ

Other info:
-----------
/* Tasklets --- multithreaded analogue of BHs.

   Main feature differing them of generic softirqs: tasklet
   is running only on one CPU simultaneously.

   Main feature differing them of BHs: different tasklets
   may be run simultaneously on different CPUs.

   Properties:
   * If tasklet_schedule() is called, then tasklet is guaranteed
     to be executed on some cpu at least once after this.
   * If the tasklet is already scheduled, but its execution is still not
     started, it will be executed only once.
   * If this tasklet is already running on another CPU (or schedule is called
     from tasklet itself), it is rescheduled for later.
   * Tasklet is strictly serialized wrt itself, but not
     wrt another tasklets. If client needs some intertask synchronization,
     he makes it with spinlocks.
 */

struct tasklet_struct
{
	struct tasklet_struct *next;
	unsigned long state;
	atomic_t count;
	void (*func)(unsigned long);
	unsigned long data;
};

Links:
https://www.ibm.com/developerworks/library/l-tasklets/index.html
https://notes.shichao.io/lkd/ch8/