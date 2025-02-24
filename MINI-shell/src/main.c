/*
  Projet minishell - Licence 3 Info - PSI 2024
 
  Nom :Moumad ,Oubalkass
  Prénom :Imrane , Nizar
  Num. étudiant : 22114553 , 22414007
  Groupe de projet : 28
  Date : 12/19/2024
 
  Gestion des processus (implémentation).
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include "parser.h"
#include "builtin.h"
#include "cmd.h"
void print_cmdtoks(char* cmdtoks[], size_t max_size) {
    for (size_t i = 0; i < max_size; i++) {
        if (cmdtoks[i] != NULL) { 
            printf("cmdtoks[%zu]: %s\n", i, cmdtoks[i]);
        } else {
            printf("cmdtoks[%zu]: NULL\n", i); 
            break;
        }
    }
}

void print_cmds(cmd_t cmds[], size_t max_size) {
    for (size_t i = 0; i < max_size; i++) {
        if (cmds[i].argv[0] != NULL) {
            printf("cmds[%zu]->argv[0]: %s\n", i, cmds[i].argv[0]);
        } else {
            printf("cmds[%zu]->argv[0]: NULL\n", i); 
            break;
        }
    }
}
int main(int argc, char* argv[]) {
  char cmdline[MAX_LINE_SIZE]; // buffer des lignes de commandes
  char* cmdtoks[MAX_CMD_SIZE]; // "mots" de la ligne de commandes
  cmd_t cmds[MAX_CMD_SIZE];
  cmd_t* current;


  
  
  while (1) {
      // Effacer les contenus de cmdline, cmdtoks et cmds
      // Initialiser les valeurs par défaut dans cmds (stdin, stdout, stderr, ...)
      
      // Afficher un prompt

        char result[1024]; 
        if (!getcwd(result, sizeof(result))) {
              return -1;
          }
        printf("imrane:~%s:$ ",result);
        if (chdir(result) == -1) {
            return -1; // Return the error code
        }

        
      // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
      if (fgets(cmdline, MAX_LINE_SIZE, stdin)==NULL) break;
      cmdline[strlen(cmdline)-1]='\0';
      

    // Traiter la ligne de commande



    if(!trim(cmdline)){//- supprimer les espaces en début et en fin de ligne
      fprintf(stderr,"trim failed\n");
    }

    printf("trim\t:%s\n",cmdline);




    if(!separate_s(cmdline, "=;!|&", strlen(cmdline))){//   - ajouter d'éventuels espaces autour de ; ! || && & ...
      fprintf(stderr,"separate_s failed\n");
     
    } 
    printf("separate_s\t:%s\n ",cmdline);
        
    if(!clean(cmdline)){ //   - supprimer les doublons d'espaces

      fprintf(stderr,"clean failed\n");
    }
    printf("clean\t:%s\n",cmdline);
        

    if(!substenv(cmdline, MAX_CMD_SIZE)){ //   - traiter les variables d'environnement

      fprintf(stderr,"substenv failed\n");
    }  
   
    if(!strcut(cmdline,' ', cmdtoks,MAX_CMD_SIZE)){// Découper la ligne dans cmdtoks

        fprintf(stderr,"strcut failed\n");
    }
  



        
        print_cmdtoks(cmdtoks, MAX_CMD_SIZE);
      if(! parse_cmd(cmdtoks,cmds,MAX_CMD_SIZE)){// Traduire la ligne en structures cmd_t dans cmds

        fprintf(stderr,"parse_cmd failed\n");
    }  
      

          
        print_cmds(cmds,MAX_CMD_SIZE);
     
        // Les commandes sont chaînées en fonction des séparateurs
        //   - next -> exécution inconditionnelle
        //   - next_success -> exécution si la commande précédente réussit
        //   - next_failure -> exécution si la commande précédente échoue

        // Exécuter les commandes dans l'ordre en fonction des opérateurs
        // de flux
        int i=0;
      for (current = cmds; current != NULL; ){


        int exec_status = exec_cmd(current);
        int builtin_status = 0;

        // If exec_cmd fails, try as a built-in command
        if (exec_status != 0) {
            builtin_status = builtin(current);
        }
            
        if( current->next!=NULL){// rain the next commend lie with the current 
         
          i++;

          current = current->next;
        }else if( current->next_success!=NULL){
          if (exec_status != 0 && builtin_status != 0) {// check if exec or builtin failed
            
            current = NULL; // Stop the second comande when one of them failed
            break;
        }
          

          current = current->next_success;

        }else if( current->next_failure!=NULL){

            if (exec_status != 0 && builtin_status != 0) {//check if exec or builtin failed
           
            current = current->next_failure; // run the second camd only if it failed
            
        }else{
            current = NULL; // Stop execution
            break;
        }

          
        }else{
          
          current = NULL; // Stop execution
          break;
         
        }
      }

      for(int i=0;i<MAX_CMD_SIZE ;i++){ // empty the cmdtoks

        cmdtoks[i]=NULL;
      }

  }
  
  fprintf(stderr, "\nGood bye!\n");
  return 0;
}
