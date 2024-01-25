/**
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Deatched option in thread attributes
 * >
 * Attributes are a way to specify behavior that 
 * is different from the default.
 * 
 * !? DETACHED .
 *  
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

    // Define attributes that e
    pthread_t threads[NUM_THREADS];
    for (int i=0; i<NUM_THREADS;i++) {count[i] = i; pthread_create(&threads[i], NULL, task, &count[i]);}
    for (int j=0; j<NUM_THREADS;j++)  pthread_join(threads[j], NULL);

    printf("*** Main process stop ***\n");
    // TODO: ..
}