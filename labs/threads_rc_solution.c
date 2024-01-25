/**
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : How to solve a race condition issue
 * 
 * */

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  

# define MAX_ITERATIONS 5

// initialize a globally affected variable
int global_sum = 0;
pthread_mutex_t mutex;

void *task (void *arg) {
    int *cp = (int *)(arg);    
    for (int i=0; i<*cp; i++) {
        pthread_mutex_lock(&mutex);
        global_sum ++;
        pthread_mutex_unlock(&mutex);
    }
}


void main () {

    printf("*** Main process statrt ***\n");

    pthread_t t0,t1;
    pthread_mutex_init(&mutex, NULL);
    int val0=10000000;

    if (pthread_create(&t0, NULL, task,(void *)&val0) !=0) exit(1);
    if (pthread_create(&t1, NULL, task,(void *)&val0) !=0) exit(1);
    
    
    if (pthread_join(t0,NULL) != 0) exit(1);
    if (pthread_join(t1,NULL) != 0) exit(1);

    printf("Global sum computation from 2 working threads: %d\n", global_sum);

    pthread_mutex_destroy(&mutex);
    printf("*** Main process stop ***\n");

}