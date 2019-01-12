This folder contains the examples of the blocking IO ie to read from the buffer
first we need to have data in the buffer.

When there is no data in the buffer the read will go into the wait state
using various methods and we will be discussing thoese methods here.

1)semaphore: This contains how we  can implement the above scenario using the
semaphores.
2)complete: This contains how we can implement the above scenario using the
complete api
3) Wait Queue: This contains how we can use the wait queue to implement the above
scenario using the wait API
