/**
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * The difference between processes and threads
 * -> Threads are combined into the processes. 
 * -> Threads share the same memory space and computer 
 *    resources stack (i.e.: memmory stack). 
 * 
 * -> Processes are independent, mature and responsible.
 * -> Each process has it's own space on the memory stack.
 * */

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>  
# include <stdio.h>

# define MAX_EPOCHS 10

void *add1 (void *arg) {
    int *casted = (int *)(arg);
    int temp = *casted;
    printf("Process with id:%d ,got into with value:(%d), changed it to:%d\n", 
    getpid(),temp, ++(*casted));
}

void main () {

    // Create the base
    int flag = 0;
    int c = 0;

    printf("Hello from the parent process [id:%d]\n",getpid());

    pid_t pid = fork();
    if (pid==-1) exit(1);


    if (pid==0) { 
        // If is the child
        printf("Hello from the  child process [id:%d]\n",getpid());        
        // Create two threads for the child
        pthread_t t0, t1;     
        if (pthread_create(&t0,NULL,add1,(void *)&c) != 0) exit(1);
        if (pthread_create(&t1,NULL,add1,(void *)&c) != 0) exit(1);
        pthread_join(t0, NULL);
        pthread_join(t1, NULL);
        printf("Child process terminates its task with c->(=%d)\n",c);        

    } 
    else {
        // If is the parent
        sleep(3);
        printf("Hello from the parent process [id:%d], init c->(=%d)\n",getpid(), c);
    }
    

}