workqueue:
----------
WorkQueue is a process context bottom half(tasklet and softirqs are interrupt context bh)
Which means we can schedule or deffer the long operations by using the workqueue

Below is an example of very simple work queue, refer the links mentioned below for more info

Implementation of work queue:
----------------------------
1) Include header file
    #include<linux/workqueue.h>
2) Create instance of work structure
    DECLARE_WORK(uni_wq,(work_func_t)uni_wq_func);
3) Scheduling the work on the kernel global worke queue(we did not create any work queue struct,
we only created the work struct. To schedule work we need work queue and we are using the linux
global work queue)
    schedule_work(&uni_wq);
Refer the Link [1] for more info.

Links:
1] https://www.ibm.com/developerworks/library/l-tasklets/index.html
2] https://notes.shichao.io/lkd/ch8/

TODO: Create your own work queue and implement the above example with it
