
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
 
# define N_READERS 4  

/* prototypes */
int access_database(int);     
void write_database();
 
void* reader(void*);
void* writer(void*);

/* global initializations */
 
sem_t mutex;    // A semaphore to control access to the rc (reader count) variable;

sem_t db;       // A semaphore to control access to the database. . It is used to
                // ensure that only one thread (either reader or writer) can access
                // the database at a time;

sem_t q;        // A semaphore used to create a queue, allowing readers and writers 
                // to take turns accessing the shared resource;

int rc = 0;     // A counter that keeps track of the number of readers currently 
                // accessing or waiting to access the database.

int my_db[N_READERS]; // My database 

void main () {

  
    pthread_t readers[N_READERS],writerTh;
    int index, ids[N_READERS]; 

    //srand(time(NULL));

    sem_init(&mutex,0,1);
    sem_init(&db,0,1);
    sem_init (&q,0,1);

    // Create readers and check for possible errors
    for(index = 0; index < N_READERS; index ++) {
        ids[index]=index+1;  
        if(pthread_create(&readers[index],0,reader,&ids[index])!=0) {  
            perror("Reader error"); 
            exit(1);                       
        }
    }

    // Create one thread for the writer
    if(pthread_create(&writerTh,0,writer,0)!=0){perror("Writer error");exit(1);}
    pthread_join(writerTh,0);   

    sem_destroy(&mutex);
    sem_destroy(&db);
    sem_destroy (&q);

}
 
void* reader(void *arg) {

    // Cast argument value to integer
    int index = *(int*)arg;
    int value;

    while (1) {

        sem_wait(&q);     // <- Down the queue (push new thread)
        sem_wait(&mutex); // <- Down the workers 
        rc++;             // + 1 reader wants to work 
       
        /* If reader is the first block the writer */
        if(rc == 1) sem_wait(&db);   
        sem_post(&mutex); // -> Up the workers 
        
        // ...
        value = access_database(index-1);
        sem_post (&q);    // -> Up the queue  (pop that thread)
        printf("Thread %d read %d\n",index, value);
        sleep(index);
        // ...

        sem_wait(&mutex); // <- Down the workers 
        rc--;             // - 1 reader finished its task

        /* If readers finished their job , then wake up writer */
        if(rc == 0) sem_post(&db);
        sem_post(&mutex);  // -> Up the workers 

    }

    return 0;
}

void* writer(void*arg) {
  
    while (1) {
        //non_access_database();
        sem_wait (&q); // <- push the writer thread
        sem_wait(&db); // <- sign for access to database
        
        // ..
        write_database();
        // ..

        sem_post (&q); // -> pop the writer
        printf("Writer is now writing...Number of readers: %d\n",rc);
        sleep(1);
        sem_post(&db); // -> sign up, that its done!
    }
    return 0;
}
 

int access_database (int index) { return my_db[index]; }
 
 

void write_database () { 
    for(int i=0; i<N_READERS; i++) {
        my_db[i] = rand()%1000;
        printf("...writing: my_db[%d] = %d\n", i, my_db[i]);
    }
  return;
}