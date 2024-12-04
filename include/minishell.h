/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:45:13 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/04 16:13:00 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../complete_Libft/libft.h"

#define PROMPT "minishell~$"

// Token types
typedef enum e_token_type {
	t_command,
	t_pipe, // |
	t_great, // >
	t_less, // <
	t_greatgreat, // >>
	t_lessless, // <<
	t_bracketleft, // (
	t_bracketrigth, // )
} t_token_type;

//Struct for tokens
typedef struct s_tokens {
	t_token_type	type;
	char 			*str;
	struct s_tokens	*next;
	struct s_tokens	*prev;
} t_tokens;

//Final command table after ast
typedef struct s_cmd_table {
	t_token_type	type;
	char			*str;
} t_cmd_table;
	
typedef struct s_minishell
{
	int exitcode;
	char *readret;
	char **array;
	struct s_cmd_table commands[6];
	struct s_tokens *tokens;
	char **envp_copy;
}	t_mini;

//ft_tokens.c
void    ft_tokenization(t_mini *attributes);

//ft_tokens_tools.c
void ft_skip_whitespace(char **line);

//ft_parsing.c
 void ft_parsing(t_mini *attributes);

//ft_execution.c - executing simple commands
int	is_empty_or_space(const char *cmd);
char	**check_if_valid_command(const char *cmd);
void	execute_simple_command(char **cmd_arr, char **envp);
void	handle_simple_command(char **cmd_arr, char **envp);
void	ft_execution(t_mini *attributes);

//ft_path.c - checking for path in envp if necessary
char	*get_command_path(const char *cmd, char **envp);
char	*check_command(const char *cmd);
char	*find_path_in_envp(char *envp[]);
char	*join_paths(const char *dir, const char *cmd);


#endif
