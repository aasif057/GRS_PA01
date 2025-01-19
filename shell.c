#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *input = NULL;
    size_t len = 0;

    while (1) {
        printf("custom_shell@aasif:$ "); 
        getline(&input, &len, stdin);  
        input[strcspn(input, "\n")] = '\0';

        // Debug for input
        printf("You entered: %s\n", input);

        // Exit the loop for exit keyword
        if (strcmp(input, "exit") == 0) {
            break;
        }
    }

    free(input);  
    return 0;
}