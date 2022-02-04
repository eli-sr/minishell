/*
 * Fichero: fragmenta.c
 * Version: 1.0.0
 * Autor: Eliezer Sotomayor Ramos
 * 
 * El programa 'fragmenta' se encarga de tomar una string normal, y crear
 * a partir de ella, un array de strings con cada una de sus palabras.
 */

#include <string.h>
#include <stdlib.h>
#include "fragmenta.h"

char **fragmenta(char str[]){
    //char str[30];
    char *copia;
    char *tok;
    //scanf("%[^\n]",str);
    copia = (char *) calloc(strlen(str)+1,sizeof(char));
    strcpy(copia,str);

    //CONTADOR
    int i=1;
    tok = strtok (str," ,");
    while (tok != NULL){
        tok = strtok (NULL, " ,");
        i++;
    }

    //*ARGS
    char **args = calloc(i+1,sizeof(char *));
    i=0;
    tok = strtok (copia," ,");
    while (tok != NULL){
        args[i] = malloc((strlen(tok)+1)*sizeof(char));
        ////args[i]=tok; //Saving each token to *args
        strcpy(args[i],tok);
        tok = strtok (NULL, " ,");
        i++;
    }
    //PONER EN LA ULTIMA POSICION EL NULL
    //args[i] == NULL;
    return args;
}

void borrarg(char **args){
    int i;
    i = 0;
    while(args[i] != NULL){
        free(args[i]);
        i++;
    }
    free(args);
}
