
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <semaphore.h>
# include <pthread.h>  


# define NUM_BYTES 14

void main () {

    // Base ..    
    // Define semaphore
    const char *sem_name = "it2022112";
    sem_t *mysem;
    mysem = sem_open(sem_name, O_CREAT,0600, 1);

    // Open file with file descriptor for read or create or append
    int fd = open("test.txt", O_RDWR | O_CREAT | O_APPEND , 00600);
    int pid = fork();
    
    // Clone base -> 2 processes
    if (pid!=0) {
        printf("Hello from parent \n");
        // down to let parent execute their instructions
        sem_wait(mysem);
        write(fd, "parent writer \n", NUM_BYTES);
        printf("Parent writes ...\n");
        sleep(3);
        // up semaphore because the parent finished their task
        sem_post(mysem);

        // Make sure that parent finshes after the child
        int status;
        // wait until childs finish its task
        waitpid(pid,&status,0);
        printf("Parent closes after child!\n");
        // close file descriptor
        close(fd);
        // close semaphore and unlink  
        sem_close(mysem);
        sem_unlink(sem_name);
    }
    else {
        printf("Hello from child \n");
        // down to let child execute their instructions
        sem_wait(mysem);
        write(fd, "child writer", NUM_BYTES-1);
        printf("Child writes ...\n");
        sleep(3);
        // up semaphore because the child finished their task
        sem_post(mysem);

    }
}