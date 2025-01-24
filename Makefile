# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/22 18:27:48 by hskrzypi          #+#    #+#              #
#    Updated: 2025/01/23 15:53:14 by llaakson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= minishell

CFLAGS	:= -Wextra -Wall -Werror

LIBLIBFT	:= ./complete_Libft

HEADERS	:= -I ./include -I $(LIBLIBFT)

LIBS := -L$(LIBLIBFT) -lreadline -lft

SRCS 	:= main.c ft_tokens.c ft_parsing.c ft_execution.c ft_path.c ft_tokens_tools.c ft_builtins.c ft_envp_builtins.c ft_export_builtin.c ft_ll_envp.c ft_parsing_tools.c ft_expand.c ft_heredoc.c ft_tokens_memory.c ft_utils.c ft_single_command.c ft_syntax.c ft_piping.c ft_cleanup.c ft_signal.c ft_error_check.c ft_redirections.c ft_free_ast.c ft_command_checks.c ft_exit.c ft_type_array.c ft_ultimate_redirections.c ft_echo.c ft_cd.c ft_path_utils.c ft_merge_tokens.c ft_signal_handler.c ft_export.c ft_exec_utils.c ft_envp_utils.c ft_envp_creation.c ft_expand_tools.c ft_save_and_restore_std.c\

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
