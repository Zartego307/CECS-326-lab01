# CECS 326 Lab 1: Threads in C

## Assignment Description

Read the documentation for `pthread_cancel` by typing `man pthread_cancel` on the Linux command line. Using this information and the model provided in [threads.c](threads.c), write a program where the initial (main) thread creates a second thread. The main thread should read input from the keyboard, waiting until the user presses the Enter key. At that point, it should kill off the second thread and print out a message reporting that it has done so. Meanwhile, the second thread should be in an infinite loop, each time around sleeping 3 seconds and then printing out a message. Once the enter key has been pressed, the main thread should wait for 5 seconds to demonstrate that the second thread has been successfully killed and then exit.
  
This program should be coded in C, using the [gcc C compiler](https://gcc.gnu.org/) on POSIX (i.e. Linux or MacOS) systems.
  
Answer the following questions in a long-form comment at the top of your code:

1. Explain what `pthread` is doing in this program. Be specific.
2. Explain why the sleeping thread can print its periodic messages while the main thread is waiting for keyboard input.

### Tips

On some systems (mainly, Linux using `gcc`), the compiler option `-pthread` may have to be added in order to avoid an error from not being able to link the libraries. On MacOS (which links `gcc` to the `clang` compiler), this should not be an issue.

## Deliverables

I will require the following items for grading:

* Your `*.c` source code file
* The answer to the two questions as comments in your code
* At least one screenshot of your executed code, in `*.png` or `*.jpg` format

Submit your files through your git repository. Your submission must follow the following rules, else *I will not grade it and you will receive a zero for the submission*:

* Do not use compression on your files
* Make sure that all significant code is *commented* with your own explanations--even the code I have provided to you
