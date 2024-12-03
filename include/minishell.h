/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 13:45:13 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/12/03 15:47:10 by llaakson         ###   ########.fr       */
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
//#include <readline.h>

#define PROMPT "minishell~$"

// Token types
typedef enum e_token_type {
	t_command,
	t_pipe, // |
	t_great, // >
	t_less, // <
	t_greatgreat, // >>
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
	struct s_cmd_table commands[1];
	struct s_tokens *tokens;
}	t_mini;

//ft_tokens.c
void    ft_tokenization(t_mini *attributes);

//ft_parsing.c
 void ft_parsing(t_mini *attributes);
#endif
