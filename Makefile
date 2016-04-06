##
## Makefile for C:\Users\Julien\Desktop\rendu in C:\Users\Julien\Desktop\rendu\math_s2\106bombyx
##
## Made by Julien Leleu
## Login   <Julien@epitech.net>
##
## Started on  Mon Feb 08 12:59:35 2016 Julien Leleu
## Last update Mon Feb 08 12:59:35 2016 Julien Leleu
##

NAME	= 109titration

SRC	= 109titration.c

FLAGS	= -W -Wall -ansi -pedantic -Wextra 

all: $(NAME)

$(NAME):
	gcc -g -o $(NAME) $(SRC) -I./ -lm $(FLAGS) 

clean:
	rm -rf *~

fclean: clean
	rm -rf $(NAME)

re: fclean all
