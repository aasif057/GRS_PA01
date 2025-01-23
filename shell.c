#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_CMD_LEN 1000


void tokenize_input(){}
void execute_commands(){}

int main() {

    char *input = NULL;
    size_t len = 0;
    char cwd[1024];
    char *args[MAX_ARGS];

    while (1) {
        // Get the current working directory
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("custom_shell@aasif:~%s$ ", cwd);  
        } else {
            perror("getcwd");
            continue;
        }

        getline(&input, &len, stdin);   //taking input for commands
        input[strcspn(input, "\n")] = '\0'; //to remove newline character

        // Debug for input
        // printf("You entered: %s\n", input);

        // Exit the loop for exit keyword 
        if (strcmp(input, "exit") == 0) {
            break;
        }

        //parsing the input into arguments
        tokenize_input(input,args);

        //executing the commands by passing the args array
        execute_commands(args);
    }

    free(input);  
    return 0;
}