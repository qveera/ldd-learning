Implementation Steps:
-------------------------

1) Writing and Insert the driver module which will aquire the major and minor
   numbers and registers chardev with vfs and uses class_create and device_create
   to create the device inode.
   The driver should also Implemente ioctl.
	cat /proc/devices

NOTE: semaphore are no more in kernl, they were replaced by mutex
If you want to run this use the old kernel 2.6
*I did not tested this*

Wait Queue:
-----------
1) Inlcude the header file
#include <linux/wait.h>
2) Declare a waitQueue
static DECLARE_WAIT_QUEUE_HEAD(uni_waitQ); //This will create a driver specific wait Q
static atomic_t data_ready; //This is for the condition in wait_event
3) In read wait for the event
wait_event_interruptible(uni_waitQ, atomic_read(&data_ready));
atomic_set(&data_ready, 0); //Reset the data_ready so that another process dont wake up
4) In write wake up the readers by wake up and setting the condition
wake_up_interrup(&uni_complete);

Test:
-----

1) Write a driver application reader and writer that initiates operations on the driver through
vfs common file api

wake_up(wait_queue_head_t *queue);
wake_up_interruptible(wait_queue_head_t *queue);
------------------------------------------------

wake_up awakens every process on the queue that is not in an exclusive wait, and exactly one exclusive waiter, if it exists.
wake_up_interruptible does the same, with the exception that it skips over processes in an uninterruptible sleep.
These functions can, before returning, cause one or more of the processes awakened to be scheduled
(although this does not happen if they are called from an atomic context).



wake_up_nr(wait_queue_head_t *queue, int nr);
wake_up_interruptible_nr(wait_queue_head_t *queue, int nr);
------------------------------------------------

These functions perform similarly to wake_up, except they can awaken up to nr exclusive waiters, instead of just one.
Note that passing 0 is interpreted as asking for all of the exclusive waiters to be awakened, rather than none of them.




wake_up_all(wait_queue_head_t *queue);
wake_up_interruptible_all(wait_queue_head_t *queue);
------------------------------------------------

This form of wake_up awakens all processes whether they are performing an exclusive wait or not
(though the interruptible form still skips processes doing uninterruptible waits).




wake_up_interruptible_sync(wait_queue_head_t *queue);
------------------------------------------------

Normally, a process that is awakened may preempt the current process and be scheduled into the processor before wake_up returns.
In other words, a call to wake_up may not be atomic. If the process calling wake_up is running in an atomic context
(it holds a spinlock, for example, or is an interrupt handler), this rescheduling does not happen.
Normally, that protection is adequate. If, however, you need to explicitly ask to not be scheduled out of the processor at this time,
you can use the "sync" variant of wake_up_interruptible. This function is most often used when the caller is about to reschedule anyway,
and it is more efficient to simply finish what little work remains first.


