#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void GetUserInput() {
    char userInput[1000] = "";

    fgets(userInput, sizeof(userInput), stdin);

 
    userInput[strcspn(userInput, "\n")] = 0;

    if (strcmp(userInput, "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    }

    if (strlen(userInput) == 0) {
        return;
    }

    if (strcmp(userInput, "ls") == 0){
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return;
        } 
        else if (pid == 0) {
            char *args[] = {userInput, NULL};
            execvp("/usr/bin/ls", args);
            perror("execvp failed "); 
            _exit(1);
        } 
        else {
            wait(NULL);
        }
    }
}

int main() {
    while (1) {
        printf("mysh> ");
        fflush(stdout); 
        GetUserInput();
    }

    return 0;
}
