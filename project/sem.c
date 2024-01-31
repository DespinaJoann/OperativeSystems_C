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

void update_stats(int *d, int n);
int writer(int fd);
int scan_columns(int fd, int ncols, int *buffer);
void *reader(void *argv);

#define N_LINES 100
#define N_COLMS 50
#define N_THREADS 4

long int global_sum = 0;
int curr_line = 0;
pthread_mutex_t my_mutex;

int main()
{
    srand(time(NULL));
    char *file = "temp.txt";

    const char *sem_name = "/my_sem01";  // Use a leading slash for a named semaphore
    sem_t *my_sem;

    my_sem = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
    if (my_sem == SEM_FAILED)
    {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&my_mutex, NULL);

    pid_t pid = fork();

    if (pid != 0)
    {
        sem_wait(my_sem);
        int fd = open(file, O_RDWR | O_APPEND | O_CREAT, 0644);
        if (fd < 0)
        {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        writer(fd);

        sem_post(my_sem);

        int status;
        waitpid(pid, &status, 0);
        close(fd);
    }
    else
    {
        sem_wait(my_sem);
        pthread_t threads[N_THREADS];
        char file_copy[100];
        strcpy(file_copy, file);

        for (int i = 0; i < N_THREADS; ++i)
        {
            pthread_create(&threads[i], NULL, reader, (void *)file_copy);
        }

        for (int i = 0; i < N_THREADS; ++i)
        {
            pthread_join(threads[i], NULL);
        }

        sem_post(my_sem);
        pthread_mutex_destroy(&my_mutex);
    }

    sem_close(my_sem);
    sem_unlink(sem_name);

    printf("\n->[\n(line:%d)-> global sum:%ld\n]\n", curr_line, global_sum);

    return 0;
}

int writer(int fd)
{
    int wrb, num;
    char space = ' ';

    for (int i = 0; i < N_LINES; i++)
    {
        for (int j = 0; j < N_COLMS; j++)
        {
            num = rand() % (100 + 1);
            wrb = write(fd, &num, sizeof(int));
            if (wrb < 0)
            {
                perror("Error writing to file");
                return -1;
            }
            wrb = write(fd, &space, sizeof(char));
            if (wrb < 0)
            {
                perror("Error writing to file");
                return -1;
            }
        }
    }

    close(fd);
    return 0;
}

void *reader(void *arg)
{
    char *fn = (char *)arg;

    int fd = open(fn, O_RDONLY, 0644);
    if (fd < 0)
    {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    int count = 0, sc;

    while (curr_line < N_LINES)
    {
        int *line = (int *)malloc(N_COLMS * sizeof(int));
        sc = scan_columns(fd, N_COLMS, line);
        if (sc < 0)
            perror("Scanner Error\n");
        else
            update_stats(line, N_COLMS);

        curr_line++;
        printf("* (line:%d)-> global sum:%ld\n", curr_line, global_sum);

        free(line);
    }

    close(fd);
    return NULL;
}

int scan_columns(int fd, int ncols, int *buffer)
{
    int rbr, num;
    char space;

    for (int c = 0; c < ncols; c++)
    {
        rbr = read(fd, &num, sizeof(int));
        if (rbr < 0)
            return -1;
        buffer[c] = num;
        rbr = read(fd, &space, sizeof(char));
        if (rbr < 0)
            return -1;
    }

    return 0;
}

void update_stats(int *d, int n)
{
    int i, s = 0;
    for (i = 0; i < n; i++)
        s += d[i];
    printf("%d iterations, global sum :%d\n", curr_line, s);

    pthread_mutex_lock(&my_mutex);
    global_sum += s;
    pthread_mutex_unlock(&my_mutex);
}
