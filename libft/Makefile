# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/20 09:47:13 by mturgeon          #+#    #+#              #
#    Updated: 2025/12/10 10:36:14 by mkeerewe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc

CFLAGS= -Wall -Werror -Wextra -g -I $(INCLUDE_PATH)
##-include $(SRCS:.c=.d) 
SRCS= src/ft_atoi.c \
	src/ft_bzero.c \
	src/ft_calloc.c \
	src/ft_isalnum.c \
	src/ft_isalpha.c \
	src/ft_is_whitespace.c \
	src/ft_isascii.c \
	src/ft_isdigit.c \
	src/ft_isnum.c \
	src/ft_isprint.c \
	src/ft_itoa.c \
	src/ft_memchr.c \
	src/ft_memcmp.c \
	src/ft_memcpy.c \
	src/ft_memmove.c \
	src/ft_memset.c \
	src/ft_putchar_fd.c \
	src/ft_putendl_fd.c \
	src/ft_putnbr_fd.c \
	src/ft_putstr_fd.c \
	src/ft_split.c \
	src/ft_strchr.c \
	src/ft_strdup.c \
	src/ft_striteri.c \
	src/ft_strjoin.c \
	src/ft_strlcat.c \
	src/ft_strlcpy.c \
	src/ft_strlen.c \
	src/ft_strmapi.c \
	src/ft_strncmp.c \
	src/ft_strnstr.c \
	src/ft_strrchr.c \
	src/ft_strtrim.c \
	src/ft_substr.c \
	src/ft_tolower.c \
	src/ft_toupper.c \
	src/ft_printf.c \
	src/printfutils.c \
	src/printfutils2.c \
	src/print_utils.c \
	src/ft_lstiter.c \
	src/ft_lstadd_back.c \
	src/ft_lstadd_front.c \
	src/ft_lstclear.c \
	src/ft_lstdelone.c \
	src/ft_lstlast.c \
	src/ft_lstmap.c \
	src/ft_lstnew.c \
	src/ft_lstsize.c \
	src/get_next_line.c \
	src/get_next_line_utils.c \
	src/ft_strcat.c \
	src/ft_strcmp.c

INCLUDE_PATH= src/libft.h

OFILES= $(SRCS:.c=.o)

NAME= libft.a

all: $(NAME)

$(NAME): $(OFILES)
	ar rcs $(NAME) $(OFILES)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(SRCS:.c=.d)
	rm -f $(OFILES_BONUS) $(BONUS:.c=.d)

fclean: clean 
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re: fclean all

.PHONY: all clean fclean re bonus
