/** 
 * @author :Despina Ioanna Chalkiadaki
 * @details
 * : Unix System calls (I/O) in C.
 * - (Sect.2)
 * * Managing the File System with syscalls: 
 * 2 Read full data content using lseek
 * */

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
#include <string.h> 

# define N_EPOCHS 10
int count = 0;

void err (int errc) {
    printf("Error code:%d(%s)\n", errc, strerror(errc));
    exit(1);
}

void display (int *bff, int bs) {
    for(int i=0;i<bs;i++) printf("%d\n",bff[i]);
}

/** Reads N number of bytes from a file and returns the catched staff. */
int* readN (int nbs, int fd) {
    int* nbff = (int *)malloc(N_EPOCHS * sizeof(int));
    int rrb = read(fd,nbff,nbs);
    count ++;
    if (rrb!=nbs) {
        printf("Must:%ld , Actually:%d -> Problem in Epoch: %d\n",sizeof(int)*N_EPOCHS,rrb, count);
        exit(1);
    } 
    return nbff;
}

void main () {

    const char *newfile = "bla.txt";
    // Open file with file descriptor for read or write or create or append
    int fd = open(newfile, O_RDONLY);
    if (fd<0) err(errno);
    
    // Get file size
    struct stat st;
    if (fstat(fd, &st) == -1)  err(errno);
    printf("Total Numbers: %ld , Total Batches: %ld\n",
    st.st_size/sizeof(int), (st.st_size/sizeof(int))/N_EPOCHS);
    // Read N_EPOCHS random numbers to the file.
    int *row;
    int i;
    for (i=0; i<((st.st_size/sizeof(int))/N_EPOCHS+1); i++) {
        row = readN(sizeof(int)*N_EPOCHS, fd);
        printf("*-- EPOCH ----------------->[%d]\n",i);
        display(row, N_EPOCHS);
        lseek(fd, N_EPOCHS * sizeof(int), SEEK_SET);
        free(row);
    }
    
    printf("Total bytes: %ld Total rows: %d\n",st.st_size, i-1);
    // Clean up and close
    close(fd);
}