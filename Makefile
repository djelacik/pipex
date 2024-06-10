SRCS = pipex.c #pipex_utils.c
OBJS = ${SRCS:.c=.o}
NAME = pipex
LIBC = ar rcs
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g
LIBDIR = libft
LIBFT = $(LIBDIR)/libft.a

all: ${NAME}

${NAME}: ${LIBFT} $(OBJS)
	$(CC) $(CFLAGS) -I$(LIBDIR) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBDIR) -c $< -o $@

${LIBFT}:
	@make -C $(LIBDIR)

clean:
	${RM} ${OBJS}
	make -C $(LIBDIR) clean
	rm -f .bonus
fclean: clean
	make -C $(LIBDIR) fclean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re