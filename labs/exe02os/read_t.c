
/** 
 * This is a temporary solution 
 * */

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

void print_line (struct rec_line line);
int reader (int fd);   // 0->success , -1->error
int scan_columns(int fd, int ncols, int *buffer);

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
    int r = reader(fd);
    if (r==-1) printf("Unsuccesfully try for reading\n"); 
    else   printf("Reading task steped with success!\n");  
}



int reader(int fd) {
    if (fd < 0)
        return -1;

    printf("Child reads ...\n");

    int reb;
    struct rec_line line;
    line.length = N_COLMS;

    for (int i = 0; i < N_LINES; i++) {
        // Allocate memory for line.columns dynamically
        line.columns = (struct rec_column *)malloc(N_COLMS * sizeof(struct rec_column));

        // Check for allocation failure
        if (line.columns == NULL) {
            perror("Failed to allocate memory");
            return -1;
        }

        // Read data into the allocated memory
        reb = read(fd, line.columns, N_COLMS * sizeof(struct rec_column));

        // Check for read error
        if (reb < 0) {
            perror("Read error");
            free(line.columns);
            return -1;
        }

        // Read the newline character separately
        char newline;
        reb = read(fd, &newline, sizeof(char));
        if (reb < 0) {
            perror("Read error");
            free(line.columns);
            return -1;
        }

        // Print the line
        print_line(line);

        // Free the allocated memory
        free(line.columns);
    }

    return 0;
}


void print_line (struct rec_line line) {
    printf("[ ");
    for (int i=0;i<line.length;i++) printf(" %d,",line.columns[i].v);
    printf(" ]\n");
}

int scan_columns(int fd, int ncols, int *buffer) {
    int reb, x[2]; 
    for (int c = 0; c < ncols; c++) {
        reb = read(fd, x, sizeof(int)*2); if (reb<0) return -1;
        printf("-> %d\n",x[0]);
        buffer[c] = x[0];
    }
    return 0;
}




