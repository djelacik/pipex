# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/18 11:59:53 by djelacik          #+#    #+#              #
#    Updated: 2024/06/18 11:59:54 by djelacik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g

PROG		= pipex
SOURCES		= pipex.c pipex_utils.c
OBJECTS		= ${SOURCES:.c=.o}

PROG_B		= pipex_bonus
B_SOURCES	= pipex_bonus.c pipex_utils.c
B_OBJECTS	= ${B_SOURCES:.c=.o}

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

all:		${PROG}

${PROG}:	${OBJECTS}
							@echo "\033[35m----Compiling lib----"
							@make re -C ./libft
							@$(CC) ${OBJECTS} -Llibft -lft -o ${PROG}
							@echo "Pipex Compiled!\n"

bonus:		${PROG_B}

${PROG_B}:	${B_OBJECTS}
							@echo "\033[36m----Compiling lib----"
							@make re -C ./libft
							@${CC} ${B_OBJECTS} -Llibft -lft -o ${PROG_B}
							@echo "Pipex Bonus Compiled!\n"

clean:
							@make clean -C ./libft
							@rm -f ${OBJECTS} ${B_OBJECTS}

fclean:		clean
							@make fclean -C ./libft
							@rm -f ${PROG} ${PROG_B}
							@echo "Deleting EVERYTHING\n"

re:			fclean all

re_bonus:	fclean bonus

.PHONY: all clean fclean re bonus re_bonus