#The Unix Shell

To run the program:
1. `gcc -O -Wall -o whoosh whoosh.c`.
2. `./whoosh`.

A simple Unix-style shell program which support some basic built-in commands and could execute non-built-in commands.

The built-in commands includes: exit, cd, pwd and path:

`exit` will stop the shell program. 
`cd` will change the directory into the given one, if no directory is given, change the directory to `HOME`. 
`pwd` will print out the current path with directory. 
`path` will reset the path of the program. One thing that needs to be mentioned here is that `/bin` will always be included in paths.

If the command is not built-in, then the program will first check if there is such a program in the paths. Then it will create a new process for the given program and execute it. Note that if redirection is announced, the program will create two new files, namely .out and .err file, to store the output and error messages only for this command.

All the errors happened in the shell program will be handled by printing `An error has occured` to `STDERR_FILENO`.
