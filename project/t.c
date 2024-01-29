
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

long int global_sum =  0;

/** Display the column's values */
void display (int *d, int n) {
   
    int i, c=0, s=0;
    printf("Data:\n -> [\n");

    for(i=0; i<n; i++) {
        printf("%d ,",d[i]); s+=d[i]; c++;
        if (c==10) { c=0;  printf("\n"); }
    }

    printf(" ]\n");
    printf("%d elements scanned and the sum of them is : %d\n",i,s);
}

void display_stats (int *d, int n) {   
    int i,s=0;
    for(i=0; i<n; i++) s+=d[i];
    printf("%d elements scanned and the sum of them is : %d\n",i,s);

    global_sum += s;
}


/** Write random data to the file with csv format */
int writer (char *fn) {

    int fd = open(fn, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fd<0) return -1;

    int wrb, num;
    char space = ' ';

    for (int i=0; i<N_LINES; i++) {
        for (int j=0; j<N_COLMS; j++) {
            num = i+j; //rand() % (100+1);       
            wrb = write(fd,&num, sizeof(int));    
            if (wrb<0) return -1;    
            wrb = write(fd,&space, sizeof(char)); // add the "" as the columns separator
            if (wrb<0) return -1;    
        }
    }

    close(fd);
    return 0;
}

/** Perform Scann by columns */
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

/** Perform Scann by lines */
int scan_lines (int fd, int nl, int nc) {
    
    int i, sc;
    int *columns;
    for (i=0; i<nl; i++) {
        columns = (int *)malloc(nc*sizeof(int));
        sc = scan_columns(fd, nc, columns);
        if (sc<0)  return -1; 
        else display_stats(columns,nc);
        free(columns);
    }
    
    return 0;
}

/** Reads the data from each liune and pass them to the scanner */
int reader (char *fn) {

    // Open file to Reading mode
    int fd = open(fn, O_RDONLY , 0644);
    if (fd<0) return -1;
    int sc = scan_lines(fd,N_LINES,N_COLMS);
    close(fd);
}


void main () {

    char *f = "temp.txt";
    //int x = writer(f);
    int x = reader(f);
    printf("Code : %d -> Sum : %ld\n", x, global_sum);
}


// TODO:
// * put the **buffer to the line
// * make it more fun using the argc,argv
// arguments to make user choose the iteration
// ok ... so how i will do it with threads and
// semaphores.
// Is needed a small reverse to structs, enums ..etc..