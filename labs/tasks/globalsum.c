
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>  

# define NUM_THREADS 4
# define TOTAL 2500000000

// Initilize global sum with 0
long int sum=0;
// Define the runtime of each thread 
long int limit=(NUM_THREADS%2==0) ? (TOTAL/NUM_THREADS) : (TOTAL/NUM_THREADS +(1-NUM_THREADS%2));

// Define mutex in global space
pthread_mutex_t mutex;

void *task (void *arg) {
    // Initialize a local sum variable
    long int localsum=0;
    int *x =(int *)arg;
    // Increese the local sum
    for (int i=0;i<limit;i++) localsum++;   
    // Add local sum to the global sum   
    pthread_mutex_lock(&mutex);
    sum+=localsum;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


void main () {

    printf("*** Main process start ***\n");
    // Create multiple threads 
    int count[NUM_THREADS];

    // Initialize global mutex
    pthread_mutex_init(&mutex, NULL);

    // Define attributes that e
    pthread_t threads[NUM_THREADS];
    for (int i=0; i<NUM_THREADS;i++) {count[i] = i; pthread_create(&threads[i], NULL, task, &count[i]);}
    for (int j=0; j<NUM_THREADS;j++)  pthread_join(threads[j], NULL);
    printf("Total sum is: %ld , it should be: %ld .\n", sum, TOTAL);
    printf("*** Main process stop ***\n");
}