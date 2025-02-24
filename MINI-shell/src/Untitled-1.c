
        if (strcmp(token, ">") == 0) {

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
        } else if (strcmp(token, ">&2") == 0) {

            if (tokens[i + 1] == NULL) {
                fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                return -1;
            }
            

            if (cmd_count >= max) {
                fprintf(stderr, "Error: Too many commands.\n");
                return -1;
            }

            current_cmd->stdout=2;
        
             i++;
             i++;
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
        } else if (strcmp(token, "2>&1") == 0) {

                    if (tokens[i + 1] == NULL) {
                        fprintf(stderr, "Error: Invalid syntax near '>'.\n");
                        return -1;
                    }
                    

                    if (cmd_count >= max) {
                        fprintf(stdout, "Error: Too many commands.\n");
                        return -1;
                    }

                    current_cmd->stderr=1;
                
                    i++;
                    i++;
        } else if (strcmp(token, "2>>") == 0) {

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

                    current_cmd->stdin=open(tokens[i + 1],O_WRONLY|O_CREAT|O_TRUNC,0644);
                
                    i++;
                    i++;
        }