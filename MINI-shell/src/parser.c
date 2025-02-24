
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
Fonction trim
Paramètre str : pointeur vers une chaîne de caractères à traiter.
                La chaîne est modifiée pour supprimer les espaces
                en début et en fin de chaîne.

Retourne      : 1 si la suppression des espaces a réussi,
                0 si la chaîne passée est NULL.

Description   : Cette fonction supprime les espaces blancs au début
                et à la fin d'une chaîne de caractères. Elle utilise
                un pointeur pour avancer jusqu'au premier caractère
                non-espace et un autre pointeur pour reculer depuis
                la fin. Elle déplace ensuite la chaîne proprement.
*/

int trim(char* str){
    if (str==NULL) return 0;
    char* ptr = str;
    char *fin;
   

    while(*ptr == ' '){
        ptr++;
    }
    fin = str+ strlen(str) - 1; // la fin de la chaine

    while(fin > ptr && *fin == ' '){
        fin--;
    }
    *(fin+1)='\0';
    memmove(str,ptr,fin-ptr+1 /*strlen(ptr)+1*/);

    return 1;

}

/*
Fonction clean
Paramètre str : pointeur vers une chaîne de caractères à traiter.
                La chaîne est modifiée pour supprimer les espaces
                consécutifs à l'intérieur de la chaîne.

Retourne      : 1 si le nettoyage des espaces a réussi,
                0 si la chaîne passée est NULL.

Description   : Cette fonction parcourt la chaîne de caractères
                et supprime les espaces consécutifs en décalant
                les caractères restants. Elle garantit qu'il n'y
                a pas d'espaces multiples consécutifs, tout en
                conservant un seul espace entre les mots.

                Le processus utilise une boucle pour détecter les
                espaces et la fonction memmove pour déplacer les
                caractères dans la chaîne.
*/

int clean(char* str){
    if (str == NULL) return 0;

    char* ptr = str;
 

    while (*ptr != '\0' ) {// search for none '\0' char
       
        while (*ptr != ' ' && *ptr != '\0') { // skip ' ' and '\0'
          
            ptr++;
        }

       
        if (*(ptr + 1) == ' ') {//delete ' '
            memmove(ptr, ptr + 1, strlen(ptr)); 
        } else {
         
            ptr++;
        }
    }

    return 1;
}

/*
Fonction separate
Paramètre str : pointeur vers une chaîne de caractères à traiter.
Paramètre s   : caractère délimiteur pour lequel on ajoute des espaces autour.
Paramètre max : taille maximale de la chaîne à traiter pour éviter tout dépassement.

Retourne      : 1 si le traitement est réussi,
                0 si la chaîne passée est NULL.

Description   : Cette fonction parcourt la chaîne de caractères `str` et ajoute des
                espaces autour du caractère délimiteur `s`. Cela garantit que chaque
                occurrence du délimiteur est entourée par un espace, sauf si l'espace
                est déjà présent avant ou après le délimiteur.

                Le processus inclut :
                - Vérifier si le caractère courant est `s`.
                - Ajouter un espace avant ou après le délimiteur s'il n'y en a pas déjà.
                - Utiliser `memmove` pour déplacer les caractères dans la chaîne
                  afin de faire de la place pour les espaces ajoutés.

                La fonction s'arrête lorsque la fin de la chaîne est atteinte ou
                lorsque la limite `max` est dépassée.
*/

int separate(char* str, char s, size_t max){
    if (str == NULL) return 0;

    char* ptr = str;
     
    int ct=0;

 

    while (*ptr != '\0' && ct<=max) {
          
        while (*ptr != s && *ptr != '\0') { // skip ' ' and '\0'
          
            ptr++;
            ct++;
        }

       
        if (ptr==str) {
          
         
            memmove(ptr+1,ptr,strlen(ptr));
           
            ptr[1]=' ';
            ptr[strlen(ptr)-1]='\0';
             ptr++;
              
             

   
        } else if (ptr==str+max){
            ptr[max+1]='\0';

        } else{
           if(*(ptr-1)!=' '){
               
                    memmove(ptr+1,ptr,strlen(ptr));
                
                    ptr[0]=' ';
                    ptr[strlen(ptr)-1]='\0';
             ptr++;

                    

                     
                     
             

            }

            if(*(ptr+1)!=' '){
                printf("bbbbbbbbbbbbbb:%s\n",ptr);
                memmove(ptr+1,ptr,strlen(ptr));
                printf("llllllllllllll:%s\n",ptr);
                
            
                ptr[1]=' ';
                ptr[strlen(ptr)-1]='\0';
             ptr++;

                 
                 
                    

            }
             ptr++;
             ptr++;
          
         
            
        }
        
       
        ct++;
    }

    return 1;
}
/*
Fonction separate_s
Paramètre str : pointeur vers une chaîne de caractères à traiter.
                La chaîne est modifiée pour ajouter des espaces
                autour des caractères délimiteurs spécifiés.
Paramètre s   : pointeur vers une chaîne contenant les délimiteurs.
                Chaque caractère dans cette chaîne est considéré
                comme un délimiteur.
Paramètre max : taille maximale de la chaîne à traiter pour éviter
                tout dépassement.

Retourne      : 1 si le traitement a réussi,
                0 si la chaîne passée est NULL.

Description   : Cette fonction parcourt la chaîne de caractères `str`
                et ajoute des espaces autour de chaque occurrence des
                délimiteurs fournis dans la chaîne `s`. Pour chaque
                caractère délimiteur :
                - Si aucun espace ne précède le délimiteur, un espace
                  est inséré avant.
                - Si aucun espace ne suit le délimiteur, un espace est
                  inséré après.
                
                Le processus utilise une boucle pour détecter chaque
                délimiteur et la fonction `memmove` pour décaler les
                caractères afin d'insérer les espaces nécessaires.

                La fonction s'arrête soit à la fin de la chaîne `str`,
                soit lorsque la taille maximale spécifiée par `max` est
                atteinte.

Sécurité      : La fonction vérifie que le pointeur `str` n'est pas NULL
                et respecte la taille maximale spécifiée par `max`.
*/

int separate_s(char* str, char* s, size_t max){

    if (str == NULL) return 0;
    while(*s!='\0'){

        char* ptr = str;
        
        int ct=0;

 

        while (*ptr != '\0' && ct<=max) {
        
            while (*ptr != *s && *ptr != '\0') {
            
                ptr++;
                ct++;
            }

        
            if (ptr==str) {
            
                memmove(ptr+1,ptr,strlen(ptr)); 
                ptr[1]=' ';
                ptr[strlen(ptr)-1]='\0';
                ptr++;
                
                

    
            } else if (ptr==str+max){
                ptr[max+1]='\0';
                continue;

            } else{
            if(*(ptr-1)!=' '){
                    
                        memmove(ptr+1,ptr,strlen(ptr));
                    
                        ptr[0]=' ';
                        ptr[strlen(ptr)+1]='\0';
                ptr++;
                }
                if(*(ptr+1)!=' '){
                
                    memmove(ptr+1,ptr,strlen(ptr));
            
                    ptr[1]=' ';
                    ptr[strlen(ptr)+1]='\0';
                ptr++;



                }
                ptr++;
                ptr++;

            }
    
            ct++;
        }
            s++;

    }

    return 1;
}

/*
    Fonction strcut
    Paramètre str    : pointeur vers une chaîne de caractères à découper.
                    La chaîne d'origine n'est pas modifiée.
    Paramètre sep    : caractère utilisé comme séparateur pour découper la chaîne.
    Paramètre tokens : tableau de pointeurs où chaque élément pointe vers un token
                    (partie de la chaîne séparée par le caractère `sep`).
    Paramètre max    : taille maximale du tableau `tokens` pour limiter le nombre de tokens.

    Retourne         : 1 si le découpage a réussi,
                     0 si `str` est NULL,
                     1 avec affichage d'erreur si une allocation mémoire échoue.

    Description      : Cette fonction prend une chaîne `str` et la découpe en tokens,
                    en utilisant le caractère `sep` comme séparateur. Chaque token
                    est alloué dynamiquement avec `malloc` et stocké dans le tableau
                    `tokens`.

                    Étapes :
                    - Parcourir la chaîne `str` caractère par caractère.
                    - Identifier les sous-chaînes (tokens) délimitées par `sep`.
                    - Allouer dynamiquement de la mémoire pour chaque token.
                    - Copier chaque sous-chaîne trouvée dans le tableau `tokens`.
                    - Ignorer les séparateurs supplémentaires pour trouver le prochain token.

    Gestion des erreurs :
    - La fonction retourne 0 si `str` est NULL.
    - En cas d'échec de l'allocation mémoire, un message d'erreur est affiché et la fonction retourne 1.
*/

int strcut(char* str, char sep, char** tokens, size_t max) {
    if (str == NULL) return 0;

    char* ptr = str;
    int c = 0;  // Token counter

    while (*ptr != '\0' && c < max) {
        int cont = 0;// nb of caratect before finding sep 
        char* temp = ptr;

        // Count characters until the separator or end of string
        while (*temp != sep && *temp != '\0') {
            temp++;
            cont++;
        }

        // set tokens[c]
        tokens[c] = (char *)malloc(cont + 1);  // +1 for '\0'
        if (tokens[c] == NULL) {
            perror("malloc failed");
            return 1; 
        }

        memmove(tokens[c], ptr, cont);
        tokens[c][cont] = '\0'; // add '\0'
       // printf("Token %d: %s\n", c, tokens[c]);
        
        c++;

        
        ptr = temp;
        if (*ptr == sep) {
            ptr++; // Skip the separator
        }
    }

    return 1;
}

/*
Fonction substenv
Paramètre str : pointeur vers une chaîne de caractères contenant
                des variables d'environnement à remplacer. 
                Exemple : "Bonjour $USER"
Paramètre max : taille maximale du tampon (buffer) `str` pour
                éviter un dépassement de mémoire.

Retourne      : 1 si toutes les variables d'environnement ont été 
                   remplacées correctement.
                -1 en cas d'erreur (nom de variable trop long, 
                   dépassement du tampon, etc.).
                0 si le pointeur `str` est NULL.

Description   :
    Cette fonction recherche toutes les occurrences de variables 
    d'environnement dans la chaîne `str`, identifiées par un 
    caractère `$` suivi d'un nom de variable (composé de lettres, 
    chiffres ou underscore `_`).

    Pour chaque variable trouvée :
    - Elle est extraite (nom de la variable).
    - Sa valeur est récupérée à l'aide de `getenv()`.
    - Si la variable existe, elle est remplacée par sa valeur.
    - Si la variable n'existe pas, elle est remplacée par une 
      chaîne vide.

    La fonction ajuste la chaîne de manière dynamique, en déplaçant
    les parties restantes pour accommoder la nouvelle taille résultante.

Précautions :
    - La chaîne `str` doit avoir une taille maximale définie par `max`.
    - Les noms de variables d'environnement sont limités à 255 caractères.
    - La fonction s'arrête si elle détecte un risque de dépassement 
      du tampon.

Exemple :
    Entrée : "Bonjour $USER, votre home est $HOME"
    Sortie : "Bonjour imrane, votre home est /home/imrane"
*/

int substenv(char* str, size_t max) {
    if (str == NULL) return 0;

    char* ptr = str;

    while (*ptr != '\0') {
        // Look for '$'
        char* dollar = strchr(ptr, '$');
        if (!dollar) {
            // No more variables
            break;
        }

        // Move past '$'
        char* var_start = dollar + 1;
        
        // Identify variable name characters (alnum + '_')
        char* var_end = var_start;
        while (*var_end != '\0' && (isalnum((unsigned char)*var_end) || *var_end == '_')) {
            var_end++;
        }

        // Length of the variable name
        size_t var_len = (size_t)(var_end - var_start);
        if (var_len == 0) {
            // No valid variable name after '$', skip it
            ptr = var_end;
            continue;
        }

        // Extract variable name
        char var_name[256]; // assuming variable name won't exceed 255 chars
        if (var_len >= sizeof(var_name)) {
            fprintf(stderr, "Variable name too long\n");
            return -1;
        }
        memcpy(var_name, var_start, var_len);
        var_name[var_len] = '\0';

        // Get environment variable value
        char* value = getenv(var_name);
        if (!value) {
            value = ""; // If not found, replace with empty string
        }

        // Length of old substring ($VAR)
        size_t old_len = var_len + 1; // '$' + var name
        // Length of new substring (the env value)
        size_t new_len = strlen(value);

        // Check for buffer size to avoid overflow
        size_t current_len = strlen(str);
        if (current_len - old_len + new_len >= max) {
            fprintf(stderr, "Not enough space in buffer to replace variable\n");
            return -1;
        }

        // Positions (as indices)
        size_t dollar_pos = (size_t)(dollar - str);
        size_t after_var_pos = (size_t)(var_end - str);

        // Move the tail of the string if necessary:
        // We'll move everything after $VAR either forward or backward
        // to accommodate new_len instead of old_len.
        if (new_len != old_len) {
            memmove(
                str + dollar_pos + new_len,  // destination
                str + after_var_pos,         // source: char after $VAR
                strlen(str + after_var_pos) + 1 // +1 to move the '\0'
            );
        }

        // Now copy the value in place of $VAR
        memcpy(str + dollar_pos, value, new_len);

        // Advance ptr after the inserted value
        ptr = str + dollar_pos + new_len;
    }

    return 1;
}

