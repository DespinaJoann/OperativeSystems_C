/**
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : How to create multiple threads with for loop
 * */

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  

# define NUM_THREADS 10

void *task (void *arg) {
    int *var=(int *)arg;
    sleep(5);
    printf("This is the (%d) thread\n",*var);
}


void main () {

    printf("*** Main process start ***\n");
    // Create multiple threads 
    int count[NUM_THREADS];

    pthread_t threads[NUM_THREADS];
    for (int i=0; i<NUM_THREADS;i++) { count[i] = i; pthread_create(&threads[i], NULL, task, &count[i]);}
    for (int j=0; j<NUM_THREADS;j++)  pthread_join(threads[j], NULL);
    printf("*** Main process stop ***\n");

}