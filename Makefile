# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/02 14:41:28 by hskrzypi          #+#    #+#              #
#    Updated: 2024/12/03 19:38:30 by llaakson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= minishell

CFLAGS	:= -Wextra -Wall -Werror

LIBLIBFT	:= ./complete_Libft

HEADERS	:= -I ./include -I $(LIBLIBFT)

LIBS := -L$(LIBLIBFT) -lreadline -lft

SRCS 	:= mainh.c ft_tokens.c ft_parsing.c \

OBJS	:= ${SRCS:.c=.o}

RM = rm -rf

all: libft $(NAME)

libft:
	@$(MAKE) -C $(LIBLIBFT)

%.o: %.c
	@$(CC) $(CFLAGS) $< -c -o $@ $(HEADERS) 
	@printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "$(NAME) compiled successfully.\n"

clean:
	$(RM) $(OBJS)
	@$(MAKE) clean -C $(LIBLIBFT)

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) fclean -C $(LIBLIBFT)

re: fclean all

.PHONY: all clean fclean re libft
