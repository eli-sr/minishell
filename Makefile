all: fragmenta minishell

fragmenta: fragmenta.c
	gcc -Wall -c fragmenta.c -g

minishell:  minishell.c fragmenta.o
	gcc -Wall -o minishell minishell.c fragmenta.o -g
