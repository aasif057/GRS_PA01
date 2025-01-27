# Custom Shell

## Overview

The Custom Shell program is a simple command-line shell written in C that allows users to execute commands using system calls. The shell also supports the execution of custom programs.

### Features

1. **Command Execution**: The shell can execute system commands and custom executables (such as compiled C programs).
2. **Built-in Commands**:
   - `cd`: Change the current working directory.
   - `help`: Display the content of the `help.txt` file.
   - `exit`: Exit the shell.
3. **Input/Output Redirection**: Supports input (`<`) and output (`>`) redirection for commands.
4. **Interactive Interface**: The shell provides an interactive prompt where users can type commands and receive output.
5. **Custom Command Execution**: The shell can execute any custom program compiled and available in the working directory.

---

## Project Structure

The project consists of the following files:
├── help.txt # Help documentation file to run the help builtin command
├── Makefile # Makefile for building the project 
├── shell.c # C source code for the custom shell
├── Screenshot.png # C source code for the custom shell


### Implementation

The shell program is implemented using several key system calls and custom functions. Here's an overview of the code structure:
1. Command Execution
The execute_commands function processes the user input and executes the corresponding system or custom command.
It uses the fork() system call to create a child process. The child process calls execvp() to execute the command. If the command is a built-in (like cd or help), it's handled without forking.
The parent process waits for the child process to finish using wait().
2. Built-in Commands
cd Command: The cd command is implemented as a built-in function. When the user types cd, the shell directly calls chdir() to change the working directory without creating a child process.
help Command: The help command displays the contents of the help.txt file. If the file is missing or unreadable, an error message is printed.
exit Command: The exit command simply terminates the shell program by breaking the main loop.
3. Input/Output Redirection
The tokenize_input function splits the user input into arguments and checks for redirection symbols (< for input redirection and > for output redirection).
If input redirection is detected, the shell opens the file with open() and redirects the standard input (STDIN_FILENO) using dup2().
Similarly, for output redirection, the file is opened and the standard output (STDOUT_FILENO) is redirected using dup2().
4. Process Handling
The shell continuously prompts the user for input using getline() and processes the command.
If the command is not a built-in (like cd or help), a new process is created using fork(). The child process executes the command with execvp().
The parent process waits for the child process to complete using wait().
5. Tokenization and Redirection
The tokenize_input function breaks the input string into tokens (arguments) using strtok().
It also handles input and output redirection, storing the filenames for redirection and passing them to the execute_commands function.
6. Error Handling
Errors such as invalid commands, file I/O failures, or failure during fork() or execvp() are handled using perror() to print descriptive error messages to the user.

### Vizualization of the process tree
I have visulized the custom shell and its child process by "pstree -p $(pgrep -f custom_shell)" command. 
"$(pgrep -f custom_shell)" finds the pid of the custom_shell and pass it to pstree -p which then prints the tree containing the child process and the pid of both the parent(custom_shell) process and the child process.


![Screenshot](https://github.com/user-attachments/assets/383e83b0-e26b-4094-83c6-be8e68035cdb)


## Compilation

complie all the file using makefile by running make command in the terminal

## Usage

Navigate to the project directory and run the following command
$ make
this will create an executable for the shell named "custom_shell"

$ ./custom_shell
this will run our custom shell in the terminal where we can run or commands.

$ make clean
this will clean all the object and executables in the folder
