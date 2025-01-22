# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/02 14:41:28 by hskrzypi          #+#    #+#              #
#    Updated: 2025/01/22 15:31:23 by llaakson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= minishell

CFLAGS	:= -Wextra -Wall -Werror

LIBLIBFT	:= ./complete_Libft

HEADERS	:= -I ./include -I $(LIBLIBFT)

LIBS := -L$(LIBLIBFT) -lreadline -lft

SRCS 	:= mainh.c ft_tokens.c ft_parsing.c ft_execution.c ft_path.c ft_tokens_tools.c ft_builtins.c ft_envp_builtins.c ft_export_builtin.c ft_ll_envp.c ft_parsing_tools.c ft_expand.c ft_heredoc.c ft_tokens_memory.c ft_utils.c single_command.c ft_syntax.c piping.c ft_cleanup.c ft_signal.c error_check.c ft_redirections.c ft_free_ast.c ft_command_checks.c ft_exit.c ft_empty_command.c ft_type_array.c ultimate_redirections.c ft_echo.c ft_cd.c ft_path_utils.c ft_merge_tokens.c ft_signal_handler.c ft_export.c ft_expand_tools.c\

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
