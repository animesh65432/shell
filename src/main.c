#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void GetUserInput() {
    char userInput[1000] = "";

    fgets(userInput, sizeof(userInput), stdin);

    userInput[strcspn(userInput, "\n")] = 0;

    if (strcmp(userInput, "ls") == 0) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork failed");
            return;
        } else if (pid == 0) {
            char *args[] = {"ls", NULL};
            execvp("/usr/bin/ls", args);
            perror("execvp failed");
            _exit(1);
        } else {
            wait(NULL); 
        }
    }
}

int main() {
    // while (1) {
    //     printf("mysh> ");
    //     fflush(stdout); 
    //     GetUserInput();
    // }

    char x = 'a';
    char y = 'a';

    if (x == y){
        printf("same");
    }
    

    return 0;
}