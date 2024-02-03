/** 
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Work with two threads who run the same task
 * */

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  


# define MAX_ITERATIONS 5

void *task (void *arg) {

    int *cp = (int *)(arg);
    int count = *cp;

    printf("Thread woke up with value -> %d\n", *cp);

    // Now that check is not nessecary for this specific test
    // if (*cp > MAX_ITERATIONS) .. .
    
    while (count++ < MAX_ITERATIONS) {
        printf(" working .. (%d) , ", count);    
        // Apply a pseudo check to find wich process runs 
        (*cp == 0) ? printf("(t1)\n") : printf("(t2)\n"); 
    }

    return NULL;
}


void main () {

    printf("*** Main process statrt ***\n");
    
    pthread_t thread0;
    pthread_t thread1;

    int val0=0;
    int val1=1; 

    pthread_create(&thread0, NULL, task,(void *)&val0);     // Call thread 1
    pthread_create(&thread1, NULL, task,(void *)&val1);     // Call threas 2

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);

    printf("*** Main process stop ***\n");


}