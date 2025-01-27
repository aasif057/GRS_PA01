#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 10
#define MAX_CMD_LEN 1000

//function to implement buitin command cd
void change_directory(char **args) {
    if (args[1] == NULL) {
        perror("Error executing command");
    } else {
        if (chdir(args[1]) != 0) {
            perror("error");
        }
    }
}

// function to implement builtin command help
void display_help() {
    FILE *help_file = fopen("help.txt", "r");
    if (help_file == NULL) {
        perror("Error opening help.txt");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), help_file) != NULL) {
        printf("%s", line);
    }
    fclose(help_file);
}

//function to parse the input
int tokenize_input(char *input, char **args, char **input_file, char **output_file) {
    int i = 0;
    char *token = strtok(input, " ");
    
    while (token != NULL) {
        // Input redirection
        if (strcmp(token, "<") == 0) {  
            token = strtok(NULL, " ");
            if (token == NULL) {
                fprintf(stderr, "Error: No input file specified\n");
                return -1;
            }
            *input_file = token;
        }
        // Output redirection
        else if (strcmp(token, ">") == 0) {  
            token = strtok(NULL, " ");
            if (token == NULL) {
                fprintf(stderr, "Error: No output file specified\n");
                return -1;
            }
            *output_file = token;
        } 
        else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null-terminate the arguments array
    return 0;
}

// Function to execute commands
void execute_commands(char **args, char *input_file, char *output_file) {
    // Handle `cd` without `fork`
    if (strcmp(args[0], "cd") == 0) {
        change_directory(args);
        return;
    }
    //Handle builtin command help
    if (strcmp(args[0], "help") == 0) {  
        display_help();
        return;
    }
    pid_t pid = fork();  // Create a child process

    if (pid == 0) {  // Child process
        // Handle cd without fork as exec sys call can't change working directory of parent process
        if (input_file != NULL) {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);  // Redirect stdin
            close(fd_in);
        }

        // Handle output redirection
        if (output_file != NULL) {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);  // Redirect stdout
            close(fd_out);
        }

        // Execute the command
        if (execvp(args[0], args) == -1) {
            perror("Error executing command");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {  // Error during fork
        perror("Fork failed");
    } else {  // Parent process
        wait(NULL);  // Wait for the child process to complete
    }
}

int main() {
    char *input = NULL;
    size_t len = 0;
    char cwd[1024];
    char *args[MAX_ARGS];
    char *input_file = NULL;
    char *output_file = NULL;

    while (1) {
        // Print the current working directory as the prompt
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("custom_shell@aasif:~%s$ ", cwd);
        } else {
            perror("getcwd");
            continue;
        }

        // taking inputs from usert
        if (getline(&input, &len, stdin) == -1) {
            printf("\n");
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';

        // Exiting the shell if input is exit
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Reseting input and output file pointers
        input_file = NULL;
        output_file = NULL;

        // Tokenizing the input and handling redirection
        if (tokenize_input(input, args, &input_file, &output_file) == -1) {
            continue;  // if any error arises during tokenization, skip the iteration
        }

        // Executing the command
        if (args[0] != NULL) {
            execute_commands(args, input_file, output_file);
        }
    }

    free(input);
    return 0;
}
