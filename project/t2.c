
/** This is a temporary solution */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

void display(int *d, int n);
void update_stats(int *d, int n);
int  writer(int fd);
int scan_columns(int fd, int ncols, int *buffer);
void *reader(void *arg);

#define N_LINES 100   // 100 lines
#define N_COLMS 50    // the number of the actual data
#define N_THREADS 4   // 4 threads

// Global initializations
long int global_sum = 0;
int curr_line = 0;
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER; // Initialize mutex

int main () {
    // Define working file and Initialize random seed
    srand(time(NULL));
    char *file = "temp.txt";

    // Define the Semaphore and Mutex
    const char *sem_name = "my_sem01";
    sem_t *my_sem = sem_open(sem_name, O_CREAT, 0600, 1);
    if (my_sem == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_init(&my_mutex, NULL);    // Initialize mutex


    pid_t pid = fork();

    if (pid != 0) {
        // Parent process for Writing
        printf("Hello from parent , id:%d\n", getpid());
        sem_wait(my_sem); // ->down
        int fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) exit(1);
        writer(fd);
        sleep(3);
        sem_post(my_sem); // <-up

        // Make sure that parent will wait for the child to finish
        int status;
        waitpid(pid, &status, 0);
        printf("Parent i will close the dore of execution!, sum=%ld\n",global_sum);
        close(fd);
        sem_close(my_sem);
        sem_unlink(sem_name);
        pthread_mutex_destroy(&my_mutex); // Destroy mutex
    }
    else {
        // Child process for Reading
        printf("Hello from child , id:%d\n", getpid());
        sem_wait(my_sem); // <-down
        pthread_t threads[N_THREADS];
        int fd = open(file, O_RDONLY, 0644);
        if (fd < 0) exit(1);

        for (int i = 0; i < N_THREADS; ++i) { pthread_create(&threads[i], NULL, reader, (void *)&fd);}
        for (int i = 0; i < N_THREADS; ++i) { pthread_join(threads[i], NULL);}
       // sleep(3);
        sem_post(my_sem); // <-up
        close(fd);
    }

    printf("\n->[\n(line:%d)-> global sum:%ld\n]\n", curr_line, global_sum);
    return 0;
}

/** Write random data to the file with csv format */
int writer (int fd) {
    int wrb, num;
    char space = ' ';
    printf("(*)-> I am the writer ;)\n");

    for (int i = 0; i < N_LINES; i++) {
        for (int j = 0; j < N_COLMS; j++) {
            num = rand() % (100 + 1);
            // print string for testing: printf("  -> %d\n",num);
            wrb = write(fd, &num, sizeof(int));
            if (wrb < 0) return -1;
            wrb = write(fd, &space, sizeof(char)); // add the "" as the columns separator
            if (wrb < 0) return -1;
        }
    }
    close(fd);
    return 0;
}

/** Reads each line */
void *reader(void *arg) {
    printf("(*)-> I am the reader ;)\n");
    int *fd = (int *)arg;
    int count = 0, sc;

    while (curr_line < N_LINES) {
        // scan a line
        int *line = (int *)malloc(N_COLMS * sizeof(int));
        sc = scan_columns(*fd, N_COLMS, line);
        if (sc < 0) perror("Scanner Error\n");
        else      update_stats(line, N_COLMS);

        printf("* (line:%d)-> global sum:%ld\n", curr_line, global_sum);

        curr_line++;
        count++;

        free(line);
    }
    return NULL;
}

/** Perform Scan by columns And Update the global sum */
int scan_columns(int fd, int ncols, int *buffer) {
    int rbr, num;
    char space;

    for (int c = 0; c < ncols; c++) {
        rbr = read(fd, &num, sizeof(int));
        if (rbr < 0) return -1;
        buffer[c] = num;
        rbr = read(fd, &space, sizeof(char));
        if (rbr < 0) return -1;
    }

    return 0;
}

void update_stats(int *d, int n) {
    int i;
    long s=0;
    for (i = 0; i < n; i++) s += d[i];
    // printf("%d elements scanned and the sum of them is : %ld\n", i, s);

    // Update global sum
    pthread_mutex_lock(&my_mutex);
    global_sum += s;
    pthread_mutex_unlock(&my_mutex);
}
