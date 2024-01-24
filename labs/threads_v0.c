# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  // import possix library


# define MAX_ITERATIONS 5

void *task () {
    printf("-> Thread start\n");
    int c=0;
    // Run the same 5 iteration's task each time
    // that the thread is called by the main
    while (c++ < MAX_ITERATIONS) 
       printf(" working .. (%d)\n", c);    
    printf("-> Thread stop\n");
    return NULL;
}


void main () {

    printf("*** Main process statrt ***\n");
    // Create a thread
    pthread_t thread;
    // Add task to the thread
    pthread_create(&thread, NULL, task, NULL);
    // Make sure that the thread has fineshed it's tasl
    // before the main exits.
    pthread_join(thread, NULL);
    printf("*** Main process stop ***\n");


}