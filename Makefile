# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: djelacik <djelacik@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/18 11:59:53 by djelacik          #+#    #+#              #
#    Updated: 2024/07/09 11:25:07 by djelacik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC          = cc
CFLAGS      = -Wall -Wextra -Werror

NAME        = pipex
SOURCES     = pipex.c pipex_utils.c pipex_utils2.c
OBJECTS     = ${SOURCES:.c=.o}

B_SOURCES   = pipex_bonus.c pipex_utils.c pipex_utils2.c
B_OBJECTS   = ${B_SOURCES:.c=.o}

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

all:		${NAME}

${NAME}:	${OBJECTS}
			@echo "\033[35m----Compiling libft and get_next_line----"
			@make -C ./libft
			@make -C ./get_next_line
			$(CC) $(OBJECTS) -Llibft -lft -Lget_next_line -lgetnextline -o ${NAME}
			@echo "Pipex Compiled!\n"

bonus:		.bonus

.bonus:		${B_OBJECTS}
			@echo "\033[36m----Compiling libft and get_next_line----"
			@make -C ./libft
			@make -C ./get_next_line
			$(CC) $(B_OBJECTS) -Llibft -lft -Lget_next_line -lgetnextline -o ${NAME}
			touch .bonus
			@echo "Pipex Bonus Compiled!\n"

clean:
			@make clean -C ./libft
			@make clean -C ./get_next_line
			@rm -f ${OBJECTS} ${B_OBJECTS}
			rm -f .bonus

fclean:		clean
			@echo "\033[31mDeleting EVERYTHING\n"
			@make fclean -C ./libft
			@make fclean -C ./get_next_line
			@rm -f ${NAME}

re:			fclean all

re_bonus:	fclean bonus

.PHONY: all clean fclean re bonus re_bonus
