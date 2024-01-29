
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <string.h>
# include <time.h>


# define N_LINES  100   // 100 lines
# define N_COLMS   50   // the number of the actuall data



void display (int *data, int n) {
   
    int i, c=0, sum=0;

    printf("Data:\n -> [\n");
    for(i=0; i<n; i++) {
        printf("%d ,",data[i]);
        sum+=data[i]; c++;
        if (c==10) { c=0; printf("\n");}
    }
    printf(" ]\n");
    printf("%d elements scaned and the sum of them is : %d\n",i,sum);
}

int writer (char *fn) {

    int fd = open(fn, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd<0) return -1;

    int wrb, num;
    char space = ' ';

    for (int i=0; i<N_LINES; i++) {
        for (int j=0; j<N_COLMS; j++) {
            num = rand() % (100+1);               // create new random number 
            wrb = write(fd,&num, sizeof(int));    // write new random number to the file
            if (wrb<0) return -1;    
            wrb = write(fd,&space, sizeof(char)); // add the "" as the columns separator
            if (wrb<0) return -1;    
        }
    }

    close(fd);
    return 0;
}


int scan_columns (int fd, int ncols, int *buffer) {
    
    int rbr,num; char space;

    for (int c=0; c<ncols; c++) {
        rbr = read(fd, &num, sizeof(int));
        if (rbr<0)  return -1;
        buffer[c] = num;
        rbr = read(fd,&space, sizeof(char));
        if (rbr<0)  return -1;
    }

    return 0;
}


int reader (char *fn) {

    // Open file to Reading mode
    int fd = open(fn, O_RDONLY , 0644);
    if (fd<0) return -1;

    // Create a storing buffer
    int *buffer = (int *)malloc(N_COLMS*sizeof(int));
    int sc = scan_columns(fd, N_COLMS, buffer);
    if (sc<0) {
        printf("Couldn't read this line\n");
    }
    else {
        display(buffer, N_COLMS);
    }
    free(buffer);
    close(fd);
}


void main () {

    char *f = "temp.txt";
   // int x = writer(f);
    int x = reader(f);
    printf("Result : %d\n", x);
}