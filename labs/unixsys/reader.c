/** 
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Unix System calls (I/O) in C.
 * - (Sect.1)
 * * Managing the File System with syscalls: 
 * 1 Read data from file
 * */

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
#include <string.h> 

# define N_READS 10


void err (int errc) {
    printf("Error code:%d(%s)\n", errc, strerror(errc));
    exit(1);
}

void display (int *bff, int bs) {
    for(int i=0;i<bs;i++) printf("%d\n",bff[i]);
}

void main () {
    const char *newfile = "bla.txt";
    // Open file with file descriptor for read or write or create or append
    int fd = open(newfile, O_RDONLY);
    if (fd<0) err(errno);

    // Read N_READS random numbers to the file.
    int *buffer = (int *)malloc(N_READS * sizeof(int));
    int rrb = read(fd, buffer, sizeof(int)*N_READS);
    printf("Must:%d , Actually:%ld\n", rrb,sizeof(int)*N_READS);

    // Display what you read
    display(buffer, N_READS);

    // Clean up and close
    free(buffer);
    close(fd);
}