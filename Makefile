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

SRC_DIR := sources

SRCS_NO_DIR	:= main.c signals/ft_signal.c signals/ft_signal_handler.c \
	parsing/ft_tokens.c parsing/ft_parsing.c parsing/ft_tokens_tools.c parsing/ft_parsing_tools.c \
	parsing/ft_expand.c parsing/ft_tokens_memory.c parsing/ft_utils.c parsing/ft_syntax.c \
	parsing/ft_free_ast.c parsing/ft_merge_tokens.c parsing/ft_expand_tools.c \
	builtins/ft_builtins.c builtins/ft_exit.c builtins/ft_echo.c builtins/ft_cd.c \
	execution/ft_execution.c execution/ft_path.c execution/ft_envp_builtins.c \
	execution/ft_export_builtin.c execution/ft_ll_envp.c execution/ft_heredoc.c \
	execution/ft_single_command.c execution/ft_piping.c execution/ft_cleanup.c \
	execution/ft_error_check.c execution/ft_redirections.c execution/ft_command_checks.c \
	execution/ft_type_array.c execution/ft_ultimate_redirections.c execution/ft_path_utils.c \
	execution/ft_export.c execution/ft_exec_utils.c execution/ft_envp_utils.c \
	execution/ft_envp_creation.c execution/ft_save_and_restore_std.c \

SRCS := $(addprefix $(SRC_DIR)/, $(SRCS_NO_DIR))

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
