MAIN_SRC = ${addprefix src/, main.cpp Webserv.cpp}

INC_DIR = inc

INC = ${addprefix inc/, Webserv.hpp}

SRC = ${MAIN_SRC}

NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98


all: ${NAME}

${NAME}: ${SRC} ${INC}
	${CC} ${CFLAGS} -I ${INC_DIR} ${SRC} -o ${NAME}

clean:
	rm -rf ${NAME}

fclean: clean

re: fclean all

.PHONY: all clean fclean re
