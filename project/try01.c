
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <pthread.h>  

// define semaphore
// define mutex
// define sum variable

void main () {
    // Create data file
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
// : read 1 line 
// lock mutex
// sum ++;
// unlock mutex