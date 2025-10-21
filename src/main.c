#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void GetUserInput() {
    char userInput[1000] = "";

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

    if (strcmp(userInput, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    if (strlen(userInput) == 0) {
        return; // empty line, ignore
    }

    // ---- ls ----
    if (strcmp(userInput, "ls") == 0) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return;
        } 
        else if (pid == 0) {
            char *args[] = {"ls", NULL};
            execvp("/bin/ls", args);
            perror("execvp failed");
            _exit(1);
        } 
        else {
            wait(NULL);
        }
    }

    // ---- mkdir ----
    else if (strncmp(userInput, "mkdir", 5) == 0) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return;
        }
        else if (pid == 0) {
            char *args[100];
            int i = 0;
            char *token = strtok(userInput, " ");
            
            while (token != NULL && i < 99) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;  
            execvp("/bin/mkdir", args);
            perror("execvp failed");
            _exit(1);
        }
        else {
            wait(NULL);
        }
    }

    // ---- cd ----
    else if (strncmp(userInput, "cd", 2) == 0) {
        char *token = strtok(userInput, " ");
        char *directory = strtok(NULL, " ");

        if (directory == NULL) {
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
            if (chdir(directory) != 0) {
                perror("cd");
            }
        }
    }

    else {
        printf("Command not found: %s\n", userInput);
    }
}

int main() {
    while (1) {
        GetUserInput();
    }
    return 0;
}
