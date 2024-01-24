/** 
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Work with multaple threads andf check for errors 
 * */

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  

# define MAX_ITERATIONS 5

void *task (void *arg) {
    int *cp = (int *)(arg);
    int count = *cp;
    printf("Thread woke up with value -> %d\n", *cp);    
    while (count++ < MAX_ITERATIONS) 
        printf(" working .. (%d) , from -> %d\n", count, *cp);    
    return NULL;
}


void main () {

    printf("*** Main process statrt ***\n");
    
    pthread_t thread0;
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;


    int val0=0;
    int val1=1; 
    int val2=2; 
    int val3=3; 


    if (pthread_create(&thread0, NULL, task,(void *)&val0) !=0) {
        printf("No memmory space availiable for threads, exit ..");
        exit(1);
    }

    if (pthread_create(&thread1, NULL, task,(void *)&val1) !=0) {
        printf("No memmory space availiable for threads, exit ..");
        exit(1);
    }    
         
    if (pthread_create(&thread2, NULL, task,(void *)&val2) !=0) {
        printf("No memmory space availiable for threads, exit ..");
        exit(1);
    }    
    if (pthread_create(&thread3, NULL, task,(void *)&val3) !=0) {
        printf("No memmory space availiable for threads, exit ..");
        exit(1);
    }    
    
    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("*** Main process stop ***\n");


}