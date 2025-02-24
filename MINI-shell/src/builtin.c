

#include "cmd.h"
#include "builtin.h"
#include <unistd.h>  // For chdir()
#include <string.h>  // For strerror()
#include <errno.h>   // For errno
#include <stdio.h>   // For dprintf()
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>




int is_builtin(const char* cmd) {
    if (!cmd) {
        return 0; // Return false if the input is NULL
    }

    // List of built-in commands
    const char* builtins[] = {"cd", "exit", "help","export", "unset" ,NULL};

    // Check if the input matches any built-in command
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(cmd, builtins[i]) == 0) {
            return 1; // It's a built-in command
        }
    }

    return 0; // Not a built-in command
}

int builtin(cmd_t* cmd) {
    if (!cmd || !cmd->argv[0]) {
        return -1; // Invalid command structure or no command specified
    }

    const char* command = cmd->argv[0];
        if(!is_builtin(command)){  
        
            //dprintf(2, "Error: '%s' is not a built-in command.\n", command);
            return -1;
        }
        // Handle built-in commands
        if (strcmp(command, "cd") == 0) {
            // Call the standalone cd function
            const char* path = cmd->argv[1]; // The path to change to
            return cd(path, 2);  // Use STDERR_FILENO for error output

        }else if (strcmp(command, "exit") == 0) {
            // Call the standalone exit_shell function
            int exit_code = 0;
            if (cmd->argv[1]) {
                exit_code = atoi(cmd->argv[1]); // Convert argument to exit code
            }
            return exit_shell(exit_code,2); 

        }else if (strcmp(command, "export") == 0) {
            // Call the standalone export function
           
            const char* var;
            const char* value=NULL;

          
                var = cmd->argv[1]; 
            

            if (cmd->argv[3]) {
                value = cmd->argv[3]; 
            }

            return export(var, value,2);

        }else if (strcmp(command, "unset") == 0) {
            // Call the standalone export function
           
            const char* var = cmd->argv[1];
            

            
            if (unsetenv(var) != 0) {
                fprintf(stderr, "unset: failed to unset '%s': %s\n", var, strerror(errno));
                return -1;
            }

            return 0;

        }else if (strcmp(command, "help") == 0) {
            // Help command
            printf("Built-in commands:\n");
            printf("  cd [directory]   : Change the current directory\n");
            printf("  exit [code]      : Exit the shell with an optional code\n");
            printf("  help             : Display this help message\n");
            return 0; // Success

        }
         return -1;

    
}



int cd(const char* path, int fderr) {
    if (!path) {
        // If no path is provided, output an error
        dprintf(fderr, "cd: missing argument\n");
        return -1;
    }
    
    
      char result[1024]; 
      if (!getcwd(result, sizeof(result))) {
            return -1;
        }
        // Append the relative path
        strncat(result, "/", sizeof(result) - strlen(result) - 1);
        strncat(result, path, sizeof(result) - strlen(result) - 1); 
        
    // Attempt to change the directory
    if (chdir(result) == -1) {
        // If chdir fails, print an error message to fderr
        dprintf(fderr, "cd: %s: %s\n", path, strerror(errno));
        return -1; // Return the error code
    }

    return 0; // Success
}


int export(const char* var, const char* value, int fderr) {
   

    if (!value) {
        // If value is NULL, we could treat it as an empty string
        value = "";
    }

    // setenv(var, value, 1) sets or updates the environment variable,
    // with overwrite=1 to always overwrite existing values.
    if (setenv(var, value, 1) == -1) {
        // setenv failed, print an error message
        dprintf(fderr, "export: failed to set variable '%s': %s\n", var, strerror(errno));
        return -1;
    }

    return 0;
}


int exit_shell(int ret, int fdout) {
    // Optionally display a message before exiting
    if (fdout >= 0) {
        dprintf(fdout, "Exiting minishell with code %d\n", ret);
    }

    // Exit the shell with the given return code
    exit(ret);

    // The exit call should terminate the program, so this is unreachable
    return -1; // If exit fails (very unlikely), return an error code
}
