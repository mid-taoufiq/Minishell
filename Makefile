CC = cc
CFLAGS = -Wall -Werror -Wextra
MFLAGS = -lreadline -lncurses
TSRC = syntax_error.c check_quotes.c split_input.c ft_split_pipe.c sep_redirection.c expanding.c
SRCS = main.c $(TSRC)
OBJECTS = $(SRCS:%.c=%.o)
LIBFT_SRCS = libft/ft_atoi.c libft/ft_bzero.c libft/ft_calloc.c libft/ft_isalnum.c libft/ft_isalpha.c \
            libft/ft_isascii.c libft/ft_isdigit.c libft/ft_isprint.c libft/ft_itoa.c libft/ft_memchr.c \
            libft/ft_memcmp.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_memset.c libft/ft_putchar_fd.c \
            libft/ft_putendl_fd.c libft/ft_putnbr_fd.c libft/ft_putstr_fd.c libft/ft_split.c \
            libft/ft_strchr.c libft/ft_strdup.c libft/ft_striteri.c libft/ft_strjoin.c libft/ft_strlcat.c \
            libft/ft_strlcpy.c libft/ft_strlen.c libft/ft_strmapi.c libft/ft_strncmp.c libft/ft_strnstr.c \
            libft/ft_strrchr.c libft/ft_strtrim.c libft/ft_substr.c libft/ft_tolower.c libft/ft_toupper.c \
			libft/ft_lstnew_bonus.c \
			libft/ft_lstsize_bonus.c libft/ft_lstlast_bonus.c libft/ft_lstadd_front_bonus.c \
			libft/ft_lstadd_back_bonus.c libft/ft_lstdelone_bonus.c libft/ft_lstclear_bonus.c \
			libft/ft_lstiter_bonus.c libft/ft_lstmap_bonus.c libft/is_whitespace.c
LIBFT_OBJS= $(LIBFT_SRCS:libft/%.c=libft/%.o)

NAME = minishell

all: $(NAME)

libft/%.o : libft/%.c libft/libft.h
	$(CC) $(CFLAGS) -c $< -I libft/ -o $@

libft/libft.a: libft/libft.h $(LIBFT_OBJS)
	cd libft && make all bonus && cd ..

$(NAME):$(OBJECTS) libft/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) ${MFLAGS} -L libft/ -lft -o $@

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	rm -f $(OBJECTS) && cd libft && make clean && cd ..

fclean: clean
	rm -f $(NAME) && cd libft && rm -f libft.a && cd ..

re: fclean all

.PHONY:	clean 