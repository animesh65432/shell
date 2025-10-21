#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

// Clean up zombie processes
void reap_zombies() {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void GetUserInput() {
    char userInput[1000] = "";
    int isBackground = 0;
    char *input_file = NULL;   // For < redirection
    char *output_file = NULL;  // For > redirection

    // Clean up any zombies first
    reap_zombies();

    // Display prompt with current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("mysh:%s> ", cwd);
    } else {
        perror("getcwd");
        printf("mysh> ");
    }

    fflush(stdout);
    fgets(userInput, sizeof(userInput), stdin);
    userInput[strcspn(userInput, "\n")] = 0; // remove newline

    // Exit command
    if (strcmp(userInput, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    // Empty line
    if (strlen(userInput) == 0) {
        return;
    }

    // Check for background process indicator
    if (strlen(userInput) > 0 && userInput[strlen(userInput)-1] == '&') {
        isBackground = 1;
        userInput[strlen(userInput)-1] = 0; // remove & from command
        // Remove trailing spaces
        while (strlen(userInput) > 0 && userInput[strlen(userInput)-1] == ' ') {
            userInput[strlen(userInput)-1] = 0;
        }
    }

    // Parse command into arguments and detect redirection
    char *args[100];
    int i = 0;
    char *token = strtok(userInput, " ");
    
    while (token != NULL && i < 99) {
        if (strcmp(token, ">") == 0) {
            // Output redirection
            token = strtok(NULL, " ");
            if (token != NULL) {
                output_file = token;
            }
        } 
        else if (strcmp(token, "<") == 0) {
            // Input redirection
            token = strtok(NULL, " ");
            if (token != NULL) {
                input_file = token;
            }
        }
        else {
            // Regular argument
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (args[0] == NULL) {
        return; // No command entered
    }

    // Built-in command: cd
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            // No argument → go HOME
            char *home = getenv("HOME");
            if (home) {
                if (chdir(home) != 0) {
                    perror("cd");
                }
            } else {
                fprintf(stderr, "cd: HOME not set\n");
            }
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return; // cd is handled in parent, don't fork
    }

    // Execute external command
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    } 
    else if (pid == 0) {
        // ===== CHILD PROCESS =====
        
        // Handle INPUT redirection (<)
        if (input_file != NULL) {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("open input file");
                _exit(1);
            }
            dup2(fd_in, STDIN_FILENO);  // Redirect stdin to file
            close(fd_in);                // Close original fd
        }
        
        // Handle OUTPUT redirection (>)
        if (output_file != NULL) {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("open output file");
                _exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);  // Redirect stdout to file
            close(fd_out);                 // Close original fd
        }
        
        // Execute the command
        execvp(args[0], args);
        
        // If execvp returns, it failed
        fprintf(stderr, "mysh: command not found: %s\n", args[0]);
        _exit(1);
    } 
    else {
        // ===== PARENT PROCESS =====
        if (isBackground == 0) {
            // Foreground: wait for specific child
            waitpid(pid, NULL, 0);
        } else {
            // Background: don't wait
            printf("[Background process started with PID %d]\n", pid);
        }
    }
}

int main() {
    printf("=== Simple Shell with I/O Redirection ===\n");
    printf("Commands:\n");
    printf("  ls                    - list files\n");
    printf("  ls > output.txt       - save output to file\n");
    printf("  cat < input.txt       - read from file\n");
    printf("  cat < in.txt > out.txt - read from in.txt, write to out.txt\n");
    printf("  echo hello > file.txt - write to file\n");
    printf("  ls &                  - background process\n");
    printf("  cd <directory>        - change directory\n");
    printf("  exit                  - quit shell\n\n");
    
    while (1) {
        GetUserInput();
    }
    return 0;
}