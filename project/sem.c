
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <semaphore.h>
# include <pthread.h>  
# include <string.h>
# include <time.h>


void display (int *d, int n);
void update_stats (int *d, int n);
int writer (int fd);
int scan_columns (int fd, int ncols, int *buffer);
int scan_lines (int fd, int nl, int nc);
void* reader (void *argv);


# define N_LINES    100   // 100 lines
# define N_COLMS     50   // the number of the actuall data
# define N_THREADS    4   // 100 lines


// Global initializations
long int global_sum =  0; 
int curr_line = 0;
pthread_mutex_t my_mutex;


void main () {

    // Define working file and Initialize random seed 
    srand(time(NULL));
    char *file = "temp.txt";

    // Define the Semaphore and Mutex
    const char *sem_name = "my_sem01";
    sem_t *my_sem;

    my_sem = sem_open(sem_name, O_CREAT,0600, 1);
    pid_t pid = fork();

    if (pid!=0) {
        // Parent process for Writing
        sem_wait(my_sem); //->down    
        int fd = open(file, O_RDWR | O_APPEND | O_CREAT, 0644);
        if (fd<0) exit(1);
        writer(fd);     
        sleep(3);
        sem_post(my_sem);  //<-up

        // Make sure that parent will went to bed after the child
        int status;
        waitpid(pid,&status,0);
        close(fd);
        sem_close(my_sem);
        sem_unlink(sem_name);
    }
    else {
        pthread_mutex_init(&my_mutex, NULL);
        sem_wait(my_sem); // <-down
        pthread_t threads[N_THREADS];
        // Create threads, add them tasks and then join them
        for (int i = 0; i < N_THREADS; ++i) { pthread_create(&threads[i], NULL, reader, (void*)&file);}
        for (int i = 0; i < N_THREADS; ++i) { pthread_join(threads[i], NULL); }
        sleep(3);
        sem_post(my_sem); //<-up
        pthread_mutex_destroy(&my_mutex);
    }

    printf("\n->[\n(line:%d)-> global sum:%ld\n]\n", curr_line, global_sum);
}

/** Write random data to the file with csv format */
int writer (int fd) {

    int wrb, num;
    char space = ' ';

    for (int i=0; i<N_LINES; i++) {
        for (int j=0; j<N_COLMS; j++) {
            num = rand() % (100+1);       
            wrb = write(fd,&num, sizeof(int));    
            if (wrb<0) return -1;    
            wrb = write(fd,&space, sizeof(char)); // add the "" as the columns separator
            if (wrb<0) return -1;    
        }
    }

    close(fd);
    return 0;
}


/** Reads each line */
void* reader (void *arg) {
    // Cast void arg type to string
    char *fn = (char *)arg;

    // Open file to Reading mode
    int fd = open(fn, O_RDONLY , 0644);
    if (fd<0) exit(4); 

    int count = 0,sc;

    while (curr_line < N_LINES) {

        // scan a line 
        int *line = (int *)malloc(N_COLMS*sizeof(int));
        sc = scan_columns(fd, N_COLMS, line);
        if (sc<0)  perror("Scanner Error\n"); 
        else update_stats(line,N_COLMS);

        curr_line ++;
        if (count%10==10)
            printf("* (line:%d)-> global sum:%ld\n", curr_line, global_sum);
        free(line);
    } 
    close(fd);
    return NULL;
}



/** Perform Scann by columns And Update the global sum */
int scan_columns (int fd, int ncols, int *buffer) {
    
    int rbr,num; char space;

    for (int c=0; c<ncols; c++) {
        rbr = read(fd, &num, sizeof(int));
        if (rbr<0)  return -1;
        buffer[c] = num;
        rbr = read(fd,&space, sizeof(char));
        if (rbr<0)  return -1;
    }

    return 0;
}

void update_stats (int *d, int n) {   
    int i,s=0;
    for(i=0; i<n; i++) s+=d[i];
    printf("%d elements scanned and the sum of them is : %d\n",i,s);

    // Update global sum
    pthread_mutex_lock(&my_mutex);
    global_sum += s;
    pthread_mutex_unlock(&my_mutex);
}



/** Display the column's values */
void display (int *d, int n) {
   
    int i, c=0, s=0;
    printf("Data:\n -> [\n");

    for(i=0; i<n; i++) {
        printf("%d ,",d[i]); s+=d[i]; c++;
        if (c==10) { c=0;  printf("\n"); }
    }

    printf(" ]\n");
    printf("%d elements scanned and the sum of them is : %d\n",i,s);
}