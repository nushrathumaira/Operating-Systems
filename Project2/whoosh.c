#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXLEN 128

void print_error() {
    char error_message[30] = "An error has occurred\n";
    if (write(STDERR_FILENO, error_message, strlen(error_message)) == -1) {
        printf("Error: cannot write in STDERR_FILENO\n");
    }
}

void resume_std(int stdout_copy, int stderr_copy) {
    dup2(stdout_copy, STDOUT_FILENO);
    dup2(stderr_copy, STDERR_FILENO);
    close(stdout_copy);
    close(stderr_copy);
}

int main(int argc, char** argv) {
    char* paths[MAXLEN]; // Store the paths given by path command
    int path_count = 2;
    paths[0] = "/bin";
    // Wrong number of arguments
    if (argc != 1) {
        print_error();
        exit(1);
    }
    while (1) {
        if (write(STDOUT_FILENO, "whoosh> ", strlen("whoosh >")) == -1) {
            printf("Error: cannot write in STDOUT_FILENO\n");
        }
        char command[MAXLEN + 2];      
        /* Read the command line */
        // Read error
        if ((fgets(command, MAXLEN + 2, stdin)) == NULL) {
            print_error();
            // Clean the stream;
            while (getchar() != '\n');
            continue;
        }
        // Too long
        if (command[strlen(command) - 1] != '\n') {
            print_error();
            // Clean the stream
            while (getchar() != '\n');
            continue;
        }
        // Parse the command line
        char* tokens[MAXLEN];
        tokens[0] = strtok(command, " \n");
        int arg_count = 1;
        while (tokens[arg_count - 1]) {
            tokens[arg_count++] = strtok(NULL, " \n");
        }
        --arg_count;
        // Empty command
        if (!tokens[0]) {
            continue;
        }
        /* Built-in commands */
        // exit
        if (!strcmp(tokens[0], "exit")) {
            exit(0);
        }
        // cd
        else if (!strcmp(tokens[0], "cd")) {
            if (chdir(tokens[1] ? tokens[1] : getenv("HOME")) == -1) {
                print_error();
            }
        }
        // pwd
        else if (!strcmp(tokens[0], "pwd")) {
            char pwd_buffer[10 * MAXLEN];
            if (!getcwd(pwd_buffer, 10 * MAXLEN)) {
                print_error();
            }
            else {
                if (write(STDOUT_FILENO, pwd_buffer, strlen(pwd_buffer)) == -1) {
                    printf("Error: cannot write in STDOUT_FILENO\n");
                }
                if (write(STDOUT_FILENO, "\n", 1) == -1) {
                    printf("Error: cannot write in STDOUT_FILENO\n");
                }
            }
        }
        // path
        else if (!strcmp(tokens[0], "path")) {
            if (!tokens[1]) {
                print_error();
            }
            else {
                int i = 1;
                memset(&paths[1], 0, sizeof(paths) - sizeof(char*));
                while (tokens[i]) {
                    paths[i] = malloc(strlen(tokens[i]) * sizeof(char));
                    strcpy(paths[i], tokens[i]);
                    ++i;
                }
                path_count = i;
            }
        }
        /* other commands */
        else {
            // Check path
            int found = 0, i = 0;
            for (i = 0; i < path_count; ++i) {
                struct stat* buffer = malloc(sizeof(struct stat*));
                char path[MAXLEN];
                strcpy(path, paths[i]);
                strcat(path, "/");
                strcat(path, tokens[0]);
                // found
                if (!stat(path, buffer)) {
                    int stdout_copy = dup(STDOUT_FILENO), stderr_copy = dup(STDERR_FILENO);
                    int pid = fork();
                    // child process
                    if (pid == 0) {
                        // Check redirection
                        int j = 0;
                        while (tokens[j]) {
                            if (strcmp(tokens[j], ">") == 0 && j != arg_count - 2) {
                                print_error();
                                exit(1);
                            }
                            ++j;
                        }
                        // redirection
                        if (arg_count > 2 && strcmp(tokens[arg_count - 2], ">") == 0) {
                            if (close(STDOUT_FILENO) == -1 || close(STDERR_FILENO) == -1) {
                                print_error();
                                exit(1);
                            }
                            // Make new file name for output
                            char out[MAXLEN], err[MAXLEN];
                            strcpy(out, tokens[arg_count - 1]);
                            strcpy(err, tokens[arg_count - 1]);
                            strcat(out, ".out");
                            strcat(err, ".err");
                            // Remove '>' from the ls command
                            tokens[arg_count - 2] = NULL;
                            // Open new files, if failed, reopen stdout and stderr
                            if (open(out, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR) == -1 
                                || open(err, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR) == -1) {
		                resume_std(stdout_copy, stderr_copy);
                                print_error();
                                exit(1);
                            }
                        }
                        tokens[0] = path;
                        execv(path, tokens);
                        // error occurs if execv fails or illegal '>'
                        print_error();
                        exit(1);
                    }
                    // parent process
                    else if (pid > 0) {
                        waitpid(pid, NULL, 0);
                        resume_std(stdout_copy, stderr_copy);
                    }
                    // error
                    else {
                        print_error();
                    }

                    found = 1;
                    break;
                }
            }
            // not found
            if (!found) {
                print_error();
            }
        }
    }

    exit(1);
}
