timer:
----------
This example is on the latest 4.15 Kernel

Implementation of work queue:
----------------------------
1) Include header file
    #include <linux/timer.h>
2) Create instance of timer_list structure
    static struct timer_list uni_timer;
3) Initialize and assign the function, expiry and data to the timer
    timer_setup(&uni_timer, uni_timer_f, 0);
    uni_timer.expires = jiffies + HZ; //One second dealy
3) Add timer so that the timer function will be called when time expires
	add_timer(&uni_timer);

