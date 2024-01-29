# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>
# include <time.h>

# define N_INTS   50
# define N_LINES 100


void err (int errc) {
    printf("Error code:%d(%s)\n", errc, strerror(errc));
    exit(1);
}

int write_lines (int fd, int lns, int wds, int *bff) {
    int sum=0;
    for (int l=0;l<lns;l++) {
        // Write n words in line 
        for (int i = 0; i < N_INTS; i++) bff[i] = rand() %100 +1;
        int wrb = write(fd, bff, sizeof(int) * wds);
        if (wrb<0) {free(bff); close(fd); err(errno);}
        
        sum += wrb;
    }

    return sum;
}

void main () {
    
    const char *newfile = "data.txt";
    srand(time(NULL));
    int fd = open(newfile, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd<0) err(errno); 

    int *buffer = (int *)malloc(N_LINES* N_INTS * sizeof(int));
    int  nb = write_lines(fd,N_LINES,N_INTS,buffer);
    printf("Must:%d , Actually:%ld\n",sizeof(int)*N_INTS*N_LINES,nb);


    free(buffer);
    close(fd);

}