
#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../complete_Libft/libft.h"

#define PROMPT "minishell~$ "
#define READ 0
#define WRITE 1

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
	t_quote, // ' "
	t_dollar // $
} t_token_type;

// Builtin types
typedef enum e_builtin_type {
	BUILTIN_ECHO = 101,
	BUILTIN_CD = 102,
	BUILTIN_PWD = 103,
	BUILTIN_EXPORT = 104,
	BUILTIN_UNSET = 105,
	BUILTIN_ENV = 106,
	BUILTIN_EXIT = 107,
} t_builtins;

//Struct for tokens
typedef struct s_tokens {
	t_token_type	type;
	char 			*str;
	int 			dir_flag;
	int				merge;
	struct s_tokens	*next;
	struct s_tokens	*prev;
} t_tokens;

//Final command table after ast
typedef struct s_cmd_table {
	t_token_type	type;
	char			*str;
	char			**cmd_arr;
	char 			**infile;
	char			**outfile;
	char			**here;
	char			**append;
	struct s_cmd_table *right;
	struct s_cmd_table *left;
} t_cmd_table;

typedef struct s_envp
{
	char	*key;
	char	*value;
	struct s_envp	*prev;
	struct s_envp	*next;
}	t_envp;

typedef struct s_minishell
{
	int exitcode;
	char *readret;
	char **array;
	struct s_cmd_table *commands;
	struct s_tokens *tokens;
	t_envp	*envp_heap;
	char	**envp_arr;
	int	input_fd;
	int	output_fd;
	int	cmd_index;
	int	**pipe_arr;
	int	num_pipes;
	int	i;
}	t_mini;

//ft_signal.c
void ft_sigint(void);


//ft_tokens.c
void    ft_tokenization(t_mini *attributes);

//ft_tokens_memory.c
void    ft_free_tokens(t_mini *attributes);
void    print_tokens(t_mini *attributes);
t_tokens    *ft_add_token(t_mini *attributes);

//ft_tokens_tools.c
int ft_skip_whitespace(char *line);
int ft_is_special(char *line);
int ft_is_whitespace(char *line);
int ft_is_quote(char *line);

//ft_syntax.c
int ft_check_quotes(char *line);
int ft_syntax_check(t_mini *attributes);
void ft_merge_tokens(t_mini *attributes);

//ft_expand.c
void    ft_expand(t_mini *attributes);
//ft_parsing.c
 void ft_parsing(t_mini *attributes);
 
//ft_parsing_tools.c
t_cmd_table *ft_find_last_parse(t_cmd_table *stack);
t_cmd_table *ft_add_new(t_mini *attributes, t_tokens *token);
char **ft_add_command_array(char **old_array, char *str);

//ft_execution.c - executing simple commands
int	is_empty_or_space(const char *cmd);
char	**check_if_valid_command(const char *cmd);
void	execute_simple_command(char **cmd_arr, t_mini *attributes, t_cmd_table *node);
void	handle_simple_command(char **cmd_arr, t_mini *attributes, t_cmd_table *node);
void	ft_execution(t_mini *attributes);
void	single_command(t_cmd_table *node, t_mini *attributes);

//pipes
int	create_pipes(t_mini *attributes);
void	free_pipes(t_mini *attributes);

//redirections
int	check_redirs(t_cmd_table *node, t_mini *attributes);
int	check_infile(t_cmd_table *node, t_mini *attributes);

//ft_path.c - checking for path in envp if necessary
char	*get_command_path(const char *cmd, t_mini *attributes);
char	*check_command(const char *cmd, t_mini *attributes);
char	*join_paths(const char *dir, const char *cmd);
char	*find_path_in_envp(char *envp[]);

//builtins
int	is_builtin(char *cmd_text);
void	handle_builtin(char **cmd_array, int flag, t_mini *attributes);
int	remove_env_var(char **cmd_array, t_mini *attributes);
char	*get_env_value(t_mini *attributes, char *key);
void    ft_env(t_mini *attributes);
char	*find_in_envp(char **envp, const char *key);
int	ft_export(char **cmd_array, t_mini *attributes);
t_envp	*envp_to_list(char **envp);
void	print_env_value(t_mini *attributes, char *key);
t_envp	*create_node(char *s, t_envp **envp_heap);
char	**envp_to_array(t_envp *envp_heap);
void	print_envp_list(t_envp *envp_heap);
char	*get_key(char *s);
char	*get_value(char *s);

//heredoc
int	here_doc_handler(t_cmd_table *node, t_mini *attributes);

//cleanup
void	envp_cleanup(t_mini *attributes);
void	clean_envp_ll(t_envp **envp_heap);

//other general utils
void	free_array(char **array);
int     ft_strcmp(char *s1, char *s2);
void	set_error_and_display(int error_code, t_mini *attributes, const char *cmd);

#endif
