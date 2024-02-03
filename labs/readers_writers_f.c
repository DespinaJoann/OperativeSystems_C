
/** 
 * A readers writers Problem solution
 * with semaphores.
*/

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <semaphore.h>
# include <pthread.h>
# include <string.h>
# include <time.h>
# include <sys/wait.h>

pthread_mutex_init r_lock;
sem_t rw_schedualer;
int readcount = 0;


void writer () {
    do {
        // Writer requests for critical section
        wait(rw_schedualer);
        // Performs the write
        // ...
        // Leave the critical section
        signal(rw_schedualer); 
    }
}

void reader () {
    wait(r_lock);
    // The number of readers has now increased by 1
    readcount++;

    if (readcount==1) {
        // Ensure that none writer can enter if there is even 1 reader
        wait(rw_schedualer);
        // Other readers can enter while the current reader is inside
        // the critical section
        signal(r_lock);
    }

    // Current reader performs reading here ...
    // ...
    wait(r_lock);
    readcount--;    // a reader wants to leave;

    if (readcount==0) {
        // then ..no readers left in the critical section
        signal(rw_schedualer);  // Inform the writer that can enter
        signal(r_lock);         // Reader leaves
    }
}
void main () {


}
