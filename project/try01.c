
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


void writer (char *fn) {

    int fd = open(fn, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd<0) return -1;

    int wrb;
    int num;
    int space = "";

    for (int i=0; i<N_LINES; i++) {
        for (int j=0; j<N_COLMS; j++) {
            num = rand() % (100+1);               // create new random number 
            wrb = write(fd,&num, sizeof(int));    // write new random number to the file
            if (wrb<0) return -1;    
            wrb = write(fd,&space, sizeof(char)); // add the "" as the columns separator
            if (wrb<0) return -1;    
        }
    }

    close(fd);

}




// *
// : read 1 line 
// lock mutex
// sum ++;
// unlock mutex