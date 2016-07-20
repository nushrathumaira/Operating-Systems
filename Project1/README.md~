1a. Fast sort
In the fast sort program, I added a new struct including the line content and the word to be compared, so that we could directly sort the array according to the word, and print the result using line content. 
First, check if the arguments are legal. Then, go through the file one time to count the line number, and read the file again to copy the line content and one of the word that is identified by the parameter (or the first word). Next, use the built-in qsort() function to sort the array. Finally, print the output according to the sorting result.

1b. getprocs()
In proc.c file, add a function to count the number of existing function. Basically it iterates through the process table, check if each process is UNUSED or not, and count those processes in the status which is not UNUSED. 
In sys_proc.s file, add sys_getprocs() and directly call getprocs() in proc.c and return the result from the function.
In addition, some little changes are made on other files to include sys_getprocs() in syscall and make the call legal, which are not listed here.
