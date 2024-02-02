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
#include <sys/wait.h>
#include <sys/mman.h>

void display(int *d, int n);
void update_stats(int *d, int n);
int  writer(int fd, sem_t *my_sem);
int scan_columns(int fd, int ncols, int *buffer, sem_t *my_sem);
void *reader(void *arg);

#define N_LINES 100   // 100 lines
#define N_COLMS 50    // the number of the actual data
#define N_THREADS 4   // 100 lines

// Global initializations
int *global_sum_ptr;
pthread_mutex_t *my_mutex_ptr;

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

    // Create shared memory for global_sum and mutex
    int shm_fd = shm_open("/global_sum_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int) + sizeof(pthread_mutex_t));
    global_sum_ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    my_mutex_ptr = (pthread_mutex_t *)(global_sum_ptr + 1);
    pthread_mutex_init(my_mutex_ptr, NULL);

    pid_t pid = fork();

    if (pid != 0) {
        // Parent process for Writing
        printf("Hello from parent , id:%d\n", getpid());
        sem_wait(my_sem); // ->down
        pthread_mutex_init(my_mutex_ptr, NULL);    // Initialize mutex
        int fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) exit(1);
        writer(fd, my_sem);
        sleep(3);
        sem_post(my_sem); // <-up

        // Make sure that parent will wait for the child to finish
        int status;
        waitpid(pid, &status, 0);
        printf("Parent will close the door of execution!, sum=%d\n", *global_sum_ptr);
        close(fd);
        sem_close(my_sem);
        sem_unlink(sem_name);
        pthread_mutex_destroy(my_mutex_ptr); // Destroy mutex

        // Close and unlink shared memory
        munmap(global_sum_ptr, sizeof(int) + sizeof(pthread_mutex_t));
        shm_unlink("/global_sum_memory");
    }
    else {
        // Child process for Reading
        printf("Hello from child , id:%d\n", getpid());
        sem_wait(my_sem); // ->down
        pthread_mutex_init(my_mutex_ptr, NULL); // Initialize mutex
        int fd = open(file, O_RDONLY);
        if (fd < 0) exit(1);
        pthread_t tid[N_THREADS];
        int *arg = malloc(sizeof(*arg));
        *arg = fd;
        for (int i = 0; i < N_THREADS; i++)
            pthread_create(&tid[i], NULL, reader, arg);
        for (int i = 0; i < N_THREADS; i++)
            pthread_join(tid[i], NULL);

        sem_post(my_sem); // <-up

        printf("(*)-> I am the writer ;)\n");
        update_stats(global_sum_ptr, N_LINES * N_COLMS);

        // Close and unlink shared memory
        pthread_mutex_destroy(my_mutex_ptr); // Destroy mutex
        munmap(global_sum_ptr, sizeof(int) + sizeof(pthread_mutex_t));
        shm_unlink("/global_sum_memory");
    }

    return 0;
}

// Function Definitions
void update_stats(int *d, int n) {
    // Simulates the writer of the database
    for (int i = 0; i < n; i++)
        d[i] = rand();
}

void *reader(void *arg) {
    // Simulates the reader of the database
    int fd = *((int *)arg);
    scan_columns(fd, N_COLMS, global_sum_ptr, my_mutex_ptr);
    return NULL;
}

int writer(int fd, sem_t *my_sem) {
    // Simulates the writer of the database
    update_stats(global_sum_ptr, N_LINES * N_COLMS);
    display(global_sum_ptr, N_LINES * N_COLMS);
    write(fd, global_sum_ptr, sizeof(int) * N_LINES * N_COLMS);
    sem_post(my_sem); // <-up
    return 0;
}

int scan_columns(int fd, int ncols, int *buffer, pthread_mutex_t *my_mutex_ptr) {
    // Simulates the reader of the database
    int column_sum;
    for (int i = 0; i < ncols; i++) {
        column_sum = 0;
        for (int j = 0; j < N_LINES; j++)
            column_sum += buffer[j * ncols + i];
        printf("(*)-> I am the reader ;)\n");
        printf("* (line:%d)-> global sum:%d\n", i, column_sum);
        pthread_mutex_lock(my_mutex_ptr);
        *global_sum_ptr += column_sum;
        pthread_mutex_unlock(my_mutex_ptr);
    }
    return 0;
}

void display(int *d, int n) {
    // Display the content of a buffer (the database, e.g.,)
    for (int i = 0; i < n; i++) {
        printf("d[%d] = %d\n", i, d[i]);
    }
}
