/*
 *               d8b          d8b          888               888 888 
 *               Y8P          Y8P          888               888 888 
 *                                         888               888 888 
 * 88888b.d88b.  888 88888b.  888 .d8888b  88888b.   .d88b.  888 888 
 * 888 "888 "88b 888 888 "88b 888 88K      888 "88b d8P  Y8b 888 888 
 * 888  888  888 888 888  888 888 "Y8888b. 888  888 88888888 888 888 
 * 888  888  888 888 888  888 888      X88 888  888 Y8b.     888 888 
 * 888  888  888 888 888  888 888  88888P By Eliezer Sotomayor Ramos
 *
 * Fichero: minishell.c
 * Version: 1.0.0
 * Autor: Eliezer Sotomayor Ramos
 * 
 * El programa 'minishell' se encarga de emular una shell
 * capaz de ejecutar comandos y realizar deberes básicos como
 * utilizar pipes o redireccionar información a otros ficheros.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "fragmenta.h"

#define r 0
#define w 1

void error(char *str);
void executeLine(char *str);

int main(){
    char str[64];
    int pid;
    while(1){
        printf("minishell> ");
        fgets(str, 64, stdin);
        if(!strcmp(str,"\n"))
            continue;
        else if(!strcmp(str,"exit\n"))
            break;
        pid = fork();
        if(pid == 0)
            executeLine(str);
        else if(pid < 0)
            error("fork()");
        else
            wait(0);
    }
    return 0;
}

void error(char *str){
    printf("minishell: error: %s\n",str);
}

void executeLine(char *str){
    char **args, **aux;
    int i,fd,fd2[2],pid;
    pipe(fd2);
    i = 0;
    str[strcspn(str, "\n")] = 0;
    args = fragmenta(str);

    while(args[i] != NULL){
        if(!strcmp(args[i],"|")){
            free(args[i]);
            args[i] = NULL;
            aux = &args[i+1];
            pid = fork();
            if(pid == 0){
                close(fd2[r]);
                dup2(fd2[w],STDOUT_FILENO);
                close(fd2[w]);
                execvp(args[0],args);
                exit(0);
            }
            else if(pid > 0){
                wait(0);
                close(fd2[w]);
                dup2(fd2[r],STDIN_FILENO);
                close(fd2[r]);
                execvp(aux[0],aux);
                exit(0);
            }
            else{
                error("fork()");
                exit(-1);
            }
        }
        else if(!strcmp(args[i],">")){
            fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if(fd == -1){
                error("redirección de fichero");
                exit(-1);
            }
            dup2(fd,STDOUT_FILENO);
            close(fd);
            free(args[i]);
            free(args[i+1]);
            args[i] = NULL;
            i++;
        }
        else if(!strcmp(args[i],">>")){
            fd = open(args[i+1], O_WRONLY | O_CREAT |O_APPEND, 0666);
            if(fd == -1){
                error("redirección de fichero");
                exit(-1);
            }
            dup2(fd,STDOUT_FILENO);
            close(fd);
            free(args[i]);
            free(args[i+1]);
            args[i] = NULL;
            i++;
        }
        else if(!strcmp(args[i],"<")){
            fd = open(args[i+1], O_RDONLY);
            if(fd == -1){
                error("redirección de fichero");
                exit(-1);
            }
            dup2(fd,STDIN_FILENO);
            close(fd);
            free(args[i]);
            free(args[i+1]);
            args[i] = NULL;
            i++;
        }
        i++;
    }
    if(execvp(args[0],args)){
        error("comando no encontrado");
        borrarg(args);
        exit(-1);
    }
    borrarg(args);
    exit(0);
}
