/** 
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Pass attribute to the function 
 * */

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  


# define MAX_ITERATIONS 5

void *task (void *arg) {
    int *cp = (int *)(arg);
    printf("Thread woke up with value -> %d\n", *cp);
    if (*cp > MAX_ITERATIONS)
    printf("Thread took the nap, %d value is out of his responsibilities\n", *cp);
    while ((*cp)++ < MAX_ITERATIONS) 
    printf(" working .. (%d)\n", *cp);    
    return NULL;
}


void main () {

    printf("*** Main process statrt ***\n");
    
    pthread_t thread;
    int val=6; 
    pthread_create(&thread, NULL, task,(void *)&val);

    pthread_join(thread, NULL);
    printf("*** Main process stop ***\n");


}