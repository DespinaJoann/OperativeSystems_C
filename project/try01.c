
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <semaphore.h>
# include <pthread.h>  
# include <fcntl.h>
# include <time.h>


# define N_LINES  100   // 100 lines
# define N_COLMS   50   // the number of the actuall data



// define semaphore
// define mutex
// define sum variable
sem_t sem; 
int globalCount = 0;


// Error Handling functions
void err (int errc) {
    printf("Error code:%d(%s)\n", errc, strerror(errc));
    exit(1);
}

// Process Signature
void reader (char *fn) ;
void writer () ;

void main () {

    const char *newfile = "data.txt";
    srand(time(NULL));

    pid_t pid = fork();

    if (pid==0) {
        // child's code
        // down senaphore
        // uses function read() *
        // up semaphore
    }
    else {
        // parent's code
        // down semaphore 
        // uses function write()
        // up semaphore 
    }

}

// *
// : use 4 threads
// each of the threads reads 1 line at the time until the file ends 
// lock mutex
// sum ++;
// unlock mutex