
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <string.h>


# define CHILDS_TASKS 5

// Initialize gloabal counter with 0
int global_count = 0;

void run_task () {
    printf(" %d task done!\n", global_count);
    ++global_count;
}

void main () {

    pid_t pid = fork();
    printf ("Hello from process `%d` .\n",getpid());
    
    if (pid<0) {
        // abort;
        return ;
    }

    if (pid==0) {
        printf("-> `%d` is the child process and their parent is `%d`.\n", getpid(), getppid());
        while (global_count<CHILDS_TASKS) run_task();
    }
    else {
        // parent takes a nap and wakes up when the child finish it's tasks
        sleep(CHILDS_TASKS);
        printf("-> `%d` is the parent process and their parent is `%d`.\n", getpid(),getppid());

    }
}



