/** 
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Unix System calls (I/O) in C.
 * - (Sect.0)
 * * Managing the File System with syscalls: 
 * 0 Writing data to file
 * */

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>

# define N_INTS 10

void err (int errc) {
    printf("Error code:%d(%s)\n", errc, strerror(errc));
    exit(1);
}


void main () {
    
    const char *newfile = "bla.txt";

    // Open file with file descriptor for read or write or append
    int fd = open(newfile, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd<0) err(errno); 

    // Get file size
    struct stat st;
    if (fstat(fd, &st) == -1)  err(errno);
    printf("File size: %ld bytes\n", st.st_size);

    // Write N_INTS random numbers to the file.
    int *buffer = (int *)malloc(N_INTS * sizeof(int));
    for (int i = 0; i < N_INTS; i++) buffer[i] = i;
    int wrb = write(fd, buffer, sizeof(int) * N_INTS);
    if (wrb<0) {free(buffer); close(fd); err(errno);}
    printf("Must:%d , Actually:%ld\n", wrb,sizeof(int)*N_INTS);


    free(buffer);
    close(fd);

}