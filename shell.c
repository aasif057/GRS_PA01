#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_CMD_LEN 1000


void tokenize_input(char *input, char **args){
     int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;  
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // terminating the arguements by null
}
void execute_commands(char **args){
    pid_t pid = fork();  // Create a child process

    if (pid == 0) {  // Child process
        if (execvp(args[0], args) == -1) {  // Executing the command
            perror("Error executing command");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {  // Error during fork
        perror("Fork failed");
    } else {  // Parent process
        wait(NULL);  // Wait for the child to complete
    }
}

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