
## Basic Unix System calls in C :

System calls are the calls that a programm makes to the system kernel
to provide the services to which the program does not have direct access.

>
- Unlike processes in user mode, which can be replaced by another process any
time a process in kernel mode cannot be arbitrarily replaced by another.

- Unix system calls are primarily used to manage the file system or control 
processes or to provide communication between multiple processes.
 
- **File Descriptor**
is an integer that uniquely identifies an open file of the process.


## Section 1 :
>
### Managing the File System with syscalls:

### `create()`: 
Create a file.
```
# include <sys/file.h> // can be replaced by <fcntl.h>
int creat(char* filename, mode_t mode)
```
The mode is specified by an octall number (i.e. 0444 = read access
for USER, GROUP and ALL for the file.)


Constants about premisions are also provided by the `/usr/include/sys/stat.h` library.

```
S_IREAD --- read permission for the owner
S_IWRITE --- write permission for the owner
S_IEXEC --- execute/search permission for the owner
S_IRWXU --- read, write, execute permission for the user
S_IRGRP – read for group
S_IWGRP – write for group
S_IXGRP – execute for group
S_IRWXG – read, write, execute for the group
S_IROTH --- read for others
S_IWOTH – write for others
S_IXOTH -- execute for others
S_IRWXO – read , write , execute for others
```    


Example of create a file for READ or WRITE :
```
creat("file.txt", S_READ | S_IWRITE)
```

### `open()`  : 
Open a file

```
# include <sys/file.h> // can be replaced by <fcntl.h>
int open(char* filename, int flads, int mode)
```
The above code opens the filename for reading or writing as specified by the access and
returns an integer descriptor for that file. 

**Descriptor** is an integer *(usually less than 16)*
that indicates a table entry for the file reference for the current process.

1. `filename` : A string thst represents the absolute,relative or filename of the file.
2. `flags` : An integre code describing the access.

```
Some of them are the given : 
O_RDONLY -- opens file for read only
O_WRONLY – opens file for write only
O_RDWR – opens file for reading and writing
O_NDELAY – prevents possible blocking
O_APPEND --- opens the file for appending
O_CREAT -- creates the file if it does not exists
O_TRUNC -- truncates the size to zero
O_EXCL – produces an error if the O_CREAT bit is on and file exists

```
3. `mode` : The file protection mode usually given by 9 bits indicating `rwx` permision.

If the open call fails, a -1 is returned; otherwise a descriptor is returned.


Example of open a file for READ :
```
open(“filename”, O_RDONLY | O_TRUNC, 0);
```

### `read()` : 
Read a certain amount of bytes in a file.
```
# include <sys/types.h> // or #include <unistd.h>
size_t read(int fd, char *buffer, size_t bytes);
```
1. `fd` is the file descriptor.
2. `buffer` is address of a memory area into which the data is read.
3. `bytes` is the maximum amount of data to read from the stream.
4. `@return` value is the actual amount of data read from the file. The pointer is incremented by the amount of data read. Bytes should not exceed the size of the buffer.

### `write()` : 
Write a certain amount of bytes (data) to a file ot other object identified by a file descriptor.

```
# include <sys/types.h>
size_t write(int fd, char *buffer, size_t bytes);
```
1. `fd` is the file descriptor.
2. `buffer` is the address of the area of memory that data is to be written out.
3. `bytes` is the amount of data to copy. 
4. `@return` value is the actual amount of
data written, if this differs from bytes then something may be wrong.

### `close()` : 
The close system call is used to close files. 
```
# include <unistd.h>
int close(int fd);
```
When a process terminates, all the files associated with the process are closed. But it is
always a good idea to close a file as they do consume resources and systems impose
limits on the number of files a process can keep open.

### `lseek()` : 

Lseek is a preety great syscall. It is used to change the position
of the file pointer, which called *offset* and to place them in a 
different posiotion in the file. Some characteristics of this syscall are the following.

1. lseek is alwaws measured in bytes.
2. lseek start its offset manipulation started from the start
of the file.
3. lseek allows random access to any byte of the file.

```
# include <sys/types.h>
 #include <unistd.h>

Prototype:
long lseek(int fd,int offset,int origin)

origin          position
0               beginning of the file
1               Current position
2               End of the file

Call            Actuall Action
lseek(fd,0,0)   places the current position at the first byte
lseek(fd,0,2)   places the current position at EOF
lseek(fd,-10,1) Backs up the current position by 10 bytes

```

### `remove()`
...
### `alias() `
...



## Resources :
- [CMU lecture notes | Lecture 24 System Programming in C.](https://www.cs.cmu.edu/~guna/15-123S11/Lectures/Lecture24.pdf)
- [WikiBooks A Little C Primer/C File-IO Through System.](https://en.wikibooks.org/wiki/A_Little_C_Primer/C_File-IO_Through_System_Calls)