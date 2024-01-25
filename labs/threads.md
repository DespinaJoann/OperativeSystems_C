
## Multithreading in C
### What is a `thread` in C ?

A `thread` is a single sequence stream within a process. Because threads have 
some of the properties of processes, they are sometimes called lightweight 
processes.

- A `thread` can ececute part of the same task, or can allocate the same task for
different clients.
- The Difference : A `thread` is not independent from each other threads, unlike 
processes. As a result, a thread shares with other threads their code section, data
section and OS resources like open files and signals. But, like processes, a thread
has its own program counter (pc), a register set and a stack space.

### Why Multithreading ?
Threads are popular solutions to improve application through **paralilism**.
For instance, in a browser, multiple tabs can be different threads. MS
word use multiple threads, one thread to format the text, other to process
inputs, etc..

```
/** 
 * Threads -> concurrent execution of 2 or more threads in
 * the same period of time.
*/

```

Threads **operates faster** than processes due to the following reasons:
1. Threads creation is much faster.
2. Context switching between threads is musch faster
3. Thread can be terminated easily
4. Communication between threads is faster.

### Does actually C supports Multithreading ??
In reality because of the age of the C language , does not actually supports 
multithreading by the language standarts. But, ofcourse is a POSIX standard
for threads. Implementation of `pthread` is available with `gcc` compiler.

#### Check notes:
