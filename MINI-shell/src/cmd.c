

#include "cmd.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int exec_cmd(cmd_t* p) {
    if (!p || !p->argv[0]) {
        fprintf(stderr, "Error: Invalid command structure or path\n");
        return -1; // Failure
    }

    // Handle pipes (set stdout and stdin appropriately)
    if (p->next_success != NULL) { // For piping
        if(p->fdclose[1]!=-1 && p->fdclose[0]!=-1 ){
         p->stdout = p->fdclose[1];              // Redirect stdout to the pipe's write end
         p->next_success->stdin = p->fdclose[0];
        } // Pass the read end to the next command
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return -1; // Failure
    }

    if (pid == 0) { // Child process
        // Redirect stdin if necessary
      


        // Redirect stdout if necessary
            if (p->stdin != STDIN_FILENO) {
                if (dup2(p->stdin, STDIN_FILENO) == -1) {
                    perror("dup2 failed for stdin");
                    _exit(1);
                }
                close(p->stdin);
            }

            if (p->stdout != STDOUT_FILENO) {
                if (dup2(p->stdout, STDOUT_FILENO) == -1) {
                    printf("\n%d\n",p->stdout);
                    perror("dup2 failed for stdout");
                    _exit(1);
                }
                close(p->stdout);
            }
        // Execute the command
        execvp(p->argv[0], p->argv);
         // Print error if exec fails
        _exit(127); // Terminate child process with error code
    } else { // Parent process
        p->pid = pid;

        // Close unnecessary pipe ends in the parent process
        if (p->next_success != NULL) {
            close(p->fdclose[1]); // Close the write end of the pipe in the parent
        }

        // Wait for the child process if required
      
        if (p->wait) {
            int status;
            waitpid(pid, &status, 0); // Wait for the child process

            if (WIFEXITED(status)) { 
                int exit_code = WEXITSTATUS(status); // Get the exit code

                if (exit_code == 127 || exit_code != 0 ) {
                    // Command not found (execvp failed)
                    
                    p->status=!p->status; // Signal failure
                } 
                
             
                return p->status;

                
            } else if (WIFSIGNALED(status)) {
                // Handle termination by a signal
           
               
                p->status=!p->status;// Mark as failed
                return p->status;
                 
            }
        }else{
            printf("%d \n",pid);
        }
    }
     
    return p->status; // Success
}

int init_cmd(cmd_t* p) {
    if (!p) {
        return -1; // Return an error if the pointer is NULL
    }

    // Initialize integers
    p->pid = -1;          // Default PID (not started)
    p->status = 0;        // Default status
    p->stdin = 0;         // Standard input
    p->stdout = 1;        // Standard output
    p->stderr = 2;        // Standard error
    p->wait = 1;          // Default to foreground execution

    // Initialize pointers
    p->path = NULL;       // No command path yet
    p->next = NULL;       // No next command
    p->next_success = NULL;
    p->next_failure = NULL;

    // Clear argv array


    for (int i = 0; i < MAX_CMD_SIZE; i++) {
        p->argv[i] = NULL; // No arguments yet
    }

    // Clear fdclose array
    for (int i = 0; i < MAX_CMD_SIZE; i++) {
       
        p->fdclose[i] = -1; // Initialize fdclose to invalid value
    }
    return 0;

}

int parse_cmd(char* tokens[], cmd_t* cmds, size_t max) {
    if (!tokens || !cmds || max == 0) {
        return -1; // Invalid input
    }


    size_t cmd_count = 0;       // Track the number of commands parsed
    cmd_t* current_cmd = NULL;  // Pointer to the current command
   

    for (size_t i = 0; tokens[i] != NULL; ) {

        char* token = tokens[i]; 
       

            // Start a new command if necessary
        if (current_cmd == NULL) {
            if (cmd_count >= max) {
                fprintf(stderr, "Error: Too many commands.\n");
                return -1;
            }
                current_cmd = &cmds[cmd_count++];
                init_cmd(current_cmd);
        }
        
        if (strcmp(token, ">") == 0) {

                if (strcmp(tokens[i+1], "&") == 0) {//>&2
                    if (strcmp(tokens[i+2], "2") == 0){
                    i=i+2;
                   

                    if (cmd_count >= max) {
                        fprintf(stderr, "Error: Too many commands.\n");
                        return -1;
                    }

                    current_cmd->stdout=2;
                
                    i++;
                    }
                   
               }else{// >
                if (tokens[i + 1] == NULL) {
                    fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                    return -1;
                }
                

                if (cmd_count >= max) {
                    fprintf(stderr, "Error: Too many commands.\n");
                    return -1;
                }

                current_cmd->stdout=open(tokens[i + 1],O_WRONLY|O_CREAT|O_TRUNC,0644);
            
                i++;
                i++;
               }
        } else if(strcmp(token, ">>") == 0) {

                    if (tokens[i + 1] == NULL) {
                        fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                        return -1;
                    }
                    

                    if (cmd_count >= max) {
                        fprintf(stderr, "Error: Too many commands.\n");
                        return -1;
                    }

                    current_cmd->stdout = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                
                
                    i++;
                    i++;
        } else if (strcmp(token, "2>") == 0) {

                if (strcmp(tokens[i+1], "&") == 0) {//2>&1
                    if (strcmp(tokens[i+2], "2") == 0){
                        i=i+2;
                 
                        current_cmd->stderr=1;
                        
                        i++;
                        i++;
                    }
                   
                }else{// 2>
                        if (tokens[i + 1] == NULL) {
                            fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                            return -1;
                        }
                        

                        if (cmd_count >= max) {
                            fprintf(stdout, "Error: Too many commands.\n");
                            return -1;
                        }

                        current_cmd->stderr=open(tokens[i + 1],O_WRONLY|O_CREAT|O_TRUNC,0644);
                    
                        i++;
                        i++;
                }
        }  else if (strcmp(token, "2>>") == 0) {

                    if (tokens[i + 1] == NULL) {
                        fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                        return -1;
                    }
                    

                    if (cmd_count >= max) {
                        fprintf(stderr, "Error: Too many commands.\n");
                        return -1;
                    }

                    current_cmd->stderr = open(tokens[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                
                
                    i++;
                    i++;
        } else if (strcmp(token, "<") == 0) {

                        if (tokens[i + 1] == NULL) {
                            fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                            return -1;
                        }
                        

                        if (cmd_count >= max) {
                            fprintf(stdout, "Error: Too many commands.\n");
                            return -1;
                        }

                        current_cmd->stdin=open(tokens[i + 1], O_RDONLY);
                    
                        i++;
                        i++;
        } else if (strcmp(token, "<<") == 0) {
            if (tokens[i + 1] == NULL) { // Check for missing delimiter
                fprintf(stderr, "Error: Missing delimiter after '<<'.\n");
                return -1;
            }

            char* delimiter = tokens[i + 1];

            if (strcmp(delimiter, "") == 0) { // Prevent empty delimiter
                fprintf(stderr, "Error: Invalid delimiter near '<<'.\n");
                return -1;
            }

            if (cmd_count >= max) {
                fprintf(stderr, "Error: Too many commands.\n");
                return -1;
            }

            // Create a temporary file for the Here Document input
            char temp_filename[] = "/tmp/heredocXXXXXX";
            int fd = mkstemp(temp_filename);
            if (fd == -1) {
                perror("Error creating temporary file");
                return -1;
            }

            printf("Enter input for Here Document (end with '%s'):\n", delimiter);

            // Read input from the user until the delimiter is found
            char buffer[1024];
            while (1) {
                printf("> "); // Prompt for input
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) break; // End of input

                // Check if the line matches the delimiter
                if (strncmp(buffer, delimiter, strlen(delimiter)) == 0 && buffer[strlen(delimiter)] == '\n') {
                    break; // End of Here Document input
                }

                // Write the input to the temporary file
                ssize_t bytes_written = write(fd, buffer, strlen(buffer));
            if (bytes_written == -1) {
                perror("Error writing to temporary file");
                close(fd);
                return -1;
            }

            }

            // Reset the file descriptor to the beginning of the file
            lseek(fd, 0, SEEK_SET);

            // Redirect stdin of the current command to this file
            current_cmd->stdin = fd;

            i += 2; // Skip "<<" and the delimiter
        }else if (strcmp(token, "&") == 0) {

                            if (cmd_count >= max) {
                            fprintf(stdout, "Error: Too many commands.\n");
                            return -1;
                        }
                    if (tokens[i + 1] && strcmp(tokens[i + 1], "&") == 0) { 
                        // Handle "&&" logic
                        i++; // Skip first "&"

                        if (cmd_count >= max) {
                            fprintf(stdout, "Error: Too many commands.\n");
                            return -1;
                        }

                        // Initialize the next command in the sequence
                        
                        current_cmd->next_success = &cmds[cmd_count]; // Chain commands
                        current_cmd = &cmds[cmd_count];
                        init_cmd(current_cmd);
                          
                        cmd_count++;

                        i++; // Skip second "&"

                    } else {
                        // Handle single "&" logic (background process)
                    

                        current_cmd->wait = 0; // Do not wait for the process to complete
                        i++; // Skip "&"
                    }
        }else{// for none redicter commandes
                    

            if (strcmp(token, "|") == 0) {
                if (tokens[i + 1] == NULL) {
                    fprintf(stderr, "Error: Invalid syntax near '|'.\n");
                    return -1;
                }
                if (cmd_count >= max) {
                                fprintf(stdout, "Error: Too many commands.\n");
                                return -1;
                }

                if (tokens[i + 1] && strcmp(tokens[i + 1], "|") == 0) { 
                            // Handle "&&" logic
                            i++; // Skip first "&"

                            

                            // Initialize the next command in the sequence
                            current_cmd->next_failure = &cmds[cmd_count];
                            current_cmd = &cmds[cmd_count];
                            init_cmd(current_cmd);
                            cmd_count++;
                            i++; 

                        
                
                }else{
                    int pipe_fd[2];
                    if (pipe(pipe_fd) == -1) {
                        perror("pipe failed");
                        return -1;
                    }

                    // Set up the current command to write to the pipe
                    current_cmd->stdout = pipe_fd[1];
                    current_cmd->fdclose[0] = pipe_fd[0]; // Track read end to close later
                    current_cmd->fdclose[1] = pipe_fd[1]; // Track write end to close later

                    // Move to the next command
                    current_cmd->next_success = &cmds[cmd_count];
                    current_cmd = &cmds[cmd_count];
                    init_cmd(current_cmd); // Initialize the next command

                    // Set up the next command to read from the pipe
                    current_cmd->stdin = pipe_fd[0];

                    cmd_count++;
                    i++; // Skip the "|"

                    }

            
            }
            if (strcmp(token, ";") == 0) {
                        if (tokens[i + 1] == NULL) {
                            fprintf(stderr, "Error: Invalid syntax near ';'.\n");
                            return -1;
                        }

                        if (cmd_count >= max) {
                            fprintf(stderr, "Error: Too many commands.\n");
                            return -1;
                        }

                        current_cmd->next = &cmds[cmd_count];
                        current_cmd = &cmds[cmd_count];
                        init_cmd(current_cmd);
                        cmd_count++;
                        i++; 
                    
                        
                        continue;
            }
            if (strcmp(token, "!") == 0) {

                    
                    
                        
                        if (cmd_count >= max) {
                            fprintf(stderr, "Error: Too many commands.\n");
                            return -1;
                        }

                        current_cmd->status = -1;
                        printf("\ncurrent_cmd->status befoooooore \t %d\n\n",current_cmd->status);
                    
                        
                        i++; 
                    
                        
                        continue;
            }
                    
                    
            // Add arguments to the current command
            size_t arg_index = 0;
                    
            while (tokens[i] != NULL && strcmp(tokens[i], "|") && strcmp(tokens[i], ";")  && strcmp(tokens[i], ">") && strcmp(tokens[i], "2>")  && strcmp(tokens[i], ">>") && strcmp(tokens[i], "2>&")  && strcmp(tokens[i], "<") && strcmp(tokens[i], "<<") && strcmp(tokens[i], "<<<")&& strcmp(tokens[i], "2>&1")  && strcmp(tokens[i], "&")) {
                if (arg_index >= MAX_CMD_SIZE) {
                    fprintf(stderr, "Error: Too many arguments for command.\n");
                        return -1;
                    }
    
                    current_cmd->argv[arg_index++] = tokens[i++];
                    
            }
        }
            
        
    }

    return cmd_count; // Return the number of commands parsed
}

