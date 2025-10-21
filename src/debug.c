#include <stdio.h>
#include <string.h>

int main() {
    char userInput[1000];

    printf("Step 1: Prompting user\n");
    printf("Enter a command: ");
    fgets(userInput, sizeof(userInput), stdin);
    printf("Step 2: Raw input: %s\n", userInput);

    // Remove newline
    userInput[strcspn(userInput, "\n")] = 0;
    printf("Step 3: After removing newline: %s\n", userInput);

    char *args[100];
    int i = 0;
    printf("Step 4: Initialized args array and counter i = %d\n", i);

    // Split the input into tokens
    char *token = strtok(userInput, " ");
    
    printf("Step 5: First strtok call, token = %s\n", token);

    while (token != NULL && i < 99) {
        args[i++] = token;
        printf("Step 6: Stored token in args[%d] = %s\n", i-1, token);

        token = strtok(NULL, " ");
        printf("Step 7: Next strtok call, token = %s\n", token);
    }
    
    args[i] = NULL;
    printf("Step 8: Null-terminated args array at index %d\n", i);

    // Debug: print the final args array
    printf("Step 9: Final args array:\n");
    for (int j = 0; j < i; j++) {
        printf("args[%d] = %s\n", j, args[j]);
    }

    printf("Step 10: Program finished\n");
    return 0;
}
