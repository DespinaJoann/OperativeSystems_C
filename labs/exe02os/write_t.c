
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <semaphore.h>
//# include <pthread.h>
# include <string.h>
# include <time.h>
# include <sys/wait.h>

// Records structure
struct rec_column { int v; char s; };
struct rec_line { int length; struct rec_column *columns; };

int writer (int fd);   // 0->success , -1->error

# define N_LINES 10   // 10 lines
# define N_COLMS 5    // 5 columns
# define N_THREADS 4   // 4 threads

// Global Initializations
long int global_sum = 0;
int curr_line = 0;

void main () {

    // Define Working file and Initialize random seed
    char * f = "./demo_data.txt"; srand(time(NULL));

    // Define Semaphore and Mutex
    const char *sem_name = "myIt2022112";
    sem_t *my_sem = sem_open(sem_name, O_CREAT,0600, 1);

    // Open a common shared file descriptor
    int fd  = open(f, O_RDWR | O_CREAT | O_APPEND , 00600);
    
    int w = writer(fd);
    if (w==-1) printf("Unsuccesfully try for writing\n"); 
    else   printf("Writing task steped with success!\n");
      
}



int writer (int fd) {

    if (fd<0) return -1;
    printf("Parent writes ...\n");
    char n='\n'; 
    int  x=0, wrb;

    struct rec_column col; col.s = ' ';

    for (int i=0; i<N_LINES; i++) {
        for (int j=0; j<N_COLMS; j++) {
            col.v = rand() % 101;
            wrb = write(fd,&col,sizeof(struct rec_column));  if (wrb<0) return -1 ;
        }
        wrb = write(fd,&n,sizeof(char));
        if (wrb<0) return -1 ;

        // Synchronize the file descriptor
        if (fdatasync(fd) < 0)
            return -1;
    }

    return 0;
}