/**
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * How to return value from a single thread
 * */
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  

# define MAX_ITERATIONS 5

void *task (void *arg) {

    int *cp = (int *)(arg);
    // Allocate memory for the result into funtion
    int *local = malloc(sizeof(int)); 
    // Do the computation
    int count = *cp;
    printf("Thread woke up with value -> %d\n", *cp);    
    while (count++ < MAX_ITERATIONS) 
    // Store the value to the result
    *local = count;
    // Return the result
    return (void *)local;
}


void main () {

    printf("*** Main process statrt ***\n");

    // Create a pointer that will store the result of the task
    int *result;

    pthread_t thread0;
    int val0=0;

    if (pthread_create(&thread0, NULL, task,(void *)&val0) !=0) exit(1);
    if (pthread_join(thread0, (void *)&result) != 0)             exit(1);
    
    printf("Result -> %d\n",*result);
    printf("*** Main process stop ***\n");

    // Clean up allocated space
    free(result);

}