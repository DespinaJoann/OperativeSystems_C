
## The `fork()` System Call

Fork is a syscall that is used to create processes. It makes no arguments and
returns a process ID.

- The purpose of `fork()` is to create a new process, which becomes the **child** 
process of the caller.  
- After a new child process is created, *both processes will share the same code* after
(both processes will excute the next instruction).
- Therefore, we have to distinguish the parent from the child. This can be easily 
done , by testing the the *returned* value of `fork()`:

```
    int process_id=fork();
    if (process_id<0) {
        printf("Unsuccessful process to create a child!");
    }
    else {
        if (process_id==0) {
            printf("I am a new borned child!!");
        }
        else {
            printf("I am the parent");
        }
    }
```

As we can understand from the previous examle, there are 3 different possible outcome
values of fork:
1. When `fork()<0 ` : Error caused and child did not borned 
2. When `fork()==0` : This is the child.
3. When `fork()>0 ` : The new identifier of the child is returned to the parent


### Usefull functions
- `getpid() ` : returns the current process id
- `getppid()` : returns the parent's id of the current process 


