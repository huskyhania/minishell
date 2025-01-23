
#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../complete_Libft/libft.h"

# define PROMPT "minishell~$ "
# define READ 0
# define WRITE 1

extern int	g_signal;
// Token types
typedef enum e_token_type {
	t_command,
	t_pipe,
	t_great,
	t_less,
	t_greatgreat,
	t_lessless,
	t_bracketleft,
	t_bracketrigth,
	t_quote,
	t_dollar
}	t_token_type;

// Builtin types
typedef enum e_builtin_type {
	BUILTIN_ECHO = 101,
	BUILTIN_CD = 102,
	BUILTIN_PWD = 103,
	BUILTIN_EXPORT = 104,
	BUILTIN_UNSET = 105,
	BUILTIN_ENV = 106,
	BUILTIN_EXIT = 107,
}	t_builtins;

//Struct for tokens
typedef struct s_tokens {
	t_token_type	type;
	char			*str;
	int				dir_flag;
	int				merge;
	int				failexp;
	int				dollar;
	struct s_tokens	*next;
	struct s_tokens	*prev;
}	t_tokens;

//Final command table after ast
typedef struct s_cmd_table {
	t_token_type		type;
	char				*str;
	char				**cmd_arr;
	char				**infile;
	char				**outfile;
	char				**here;
	char				**herefile;
	char				**append;
	int					*type_arr;
	int					last_infile;
	int					last_outfile;
	int					input_fd;
	int					output_fd;
	int					failexp;
	char				*in1;
	char				*out1;
	char				*name;
	struct s_cmd_table	*right;
	struct s_cmd_table	*left;
}	t_cmd_table;

typedef struct s_envp
{
	char			*key;
	char			*value;
	struct s_envp	*prev;
	struct s_envp	*next;
}	t_envp;

typedef struct s_minishell
{
	int					exitcode;
	int					input_fd;
	int					output_fd;
	int					cmd_index;
	int					num_pipes;
	int					i;
	int					type_count;
	int					*pids;
	int					**pipe_arr;
	char				*readret;
	char				**array;
	char				**envp_arr;
	struct s_cmd_table	*commands;
	struct s_tokens		*tokens;
	t_envp				*envp_heap;
}	t_mini;

t_mini		*ft_attributes(void);

//ft_signal.c
void		ft_sigint(void);
void		ft_heresignal(void);
void		ft_resetsignal(void);
void		ft_resetposthere(void);
void		ft_sigignore(void);

//ft_signal_handler.c
void		ft_handle_post_here(int sig);
void		ft_handle_sigint(int sig);
void		ft_heredoc_sighandler(int sig);
void		ft_print_signal_error(int sig);

//ft_tokens.c
int			ft_tokenization(t_mini *attributes);

//ft_tokens_memory.c
void		ft_free_tokens(t_mini *attributes);
t_tokens	*ft_add_token(t_mini *attributes);

//ft_tokens_tools.c
int			ft_skip_whitespace(char *line);
int			ft_is_special(char *line);
int			ft_is_whitespace(char *line);
int			ft_is_quote(char *line);
int			ft_check_expansion(char *str, int j);

//ft_syntax.c
int			ft_check_quotes(char *line);
int			ft_syntax_check(t_mini *attributes);
void		ft_delete_token(t_tokens *token);

void		ft_convert(t_mini *attributes);
int			ft_merge_tokens(t_mini *attributes);

//ft_expand.c
int			ft_expand(t_mini *attributes);

char		*ft_replace_expansion(char *token, char *path, char *expansion);
char		*ft_replace_expansion(char *token, char *path, char *expansion);
int			ft_expand_pid(t_mini *attributes, t_tokens *token, int j);
int			ft_expand_exitcode(t_mini *attributes, t_tokens *token, int j);
int			ft_expand_small(t_mini *attributes, t_tokens *token, int j);

//ft_parsing.c
int			ft_parsing(t_mini *attributes);

//ft_parsing_tools.c
char		**ft_add_cmd_arr(char **old_array, char *str);
void		ft_free_cmd_array(char **arr);
t_cmd_table	*ft_add_new(t_tokens *token);
t_cmd_table	*ft_add_new(t_tokens *token);

//ft_type_array.c
int			*ft_add_type_arr(t_mini *attributes, int *old_array, int type);

//ft_free_ast.c
int			ft_free_ast(t_mini *attributes);
void		ft_check_ast_array(t_cmd_table *print);

//ft_execution.c - executing simple commands
int			is_empty_or_space(const char *cmd);
int			create_pids_array(t_mini *attributes);
int			check_if_valid_command(t_cmd_table *node, t_mini *attributes);
void		ft_execution(t_mini *attributes);
void		single_command(t_cmd_table *node, t_mini *attributes);
void		wait_for_all_processes(t_mini *attributes);
void		close_pipes_in_parent(t_mini *attributes);

//pipes
int			create_pipes(t_mini *attributes);
void		free_pipes(t_mini *attributes);
void		handle_pipes(t_mini *attributes);
void		builtin_pipe(t_mini *attributes);

//redirections
int			check_files(t_cmd_table *node, t_mini *attributes, int i);
int			redir_in(t_cmd_table *node, t_mini *attributes);
int			redir_out(t_cmd_table *node, t_mini *attributes);
void		redirect_child(t_cmd_table *node, t_mini *attributes);

//ft_path.c - checking for path in envp if necessary
char		*get_command_path(const char *cmd, t_mini *attributes);
char		*check_command(const char *cmd, t_mini *attributes);
char		*join_paths(const char *dir, const char *cmd);
char		*find_path_in_envp(char *envp[]);
int			check_access(const char *full_path, int *found);

//builtins
int			is_builtin(char *cmd_text);
void		handle_builtin(t_cmd_table *node, int flag, t_mini *attributes);
int			remove_env_var(char **cmd_array, t_mini *attributes);
char		*get_env_value(t_mini *attributes, char *key);
void		ft_env(t_cmd_table *node, t_mini *attributes);
char		*find_in_envp(char **envp, const char *key);
int			ft_export(t_cmd_table *node, t_mini *attributes);
int			env_var_exists(char *export, t_envp **envp_heap);
int			replace_append(char *export, t_envp **envp_heap);
int			envp_list_size(t_envp *envp_heap);
t_envp		*envp_to_list(char **envp);
void		print_env_value(t_mini *attributes, char *key);
void		free_envp_list(t_envp *envp_heap);
t_envp		*create_node(char *s, t_envp **envp_heap);
char		**envp_to_array(t_envp *envp_heap);
char		*create_envp_entry(t_envp *envp_node);
void		print_envp_list(t_envp *envp_heap);
char		*get_key(char *s);
char		*get_value(char *s);
void		ft_echo(t_cmd_table *node, t_mini *attributes);
void		ft_cd(char **cmd_array, int i, t_mini *attributes);

//ft_exit.c
void		ft_final_exit(t_mini *attributes, int exit_number);
void		ft_exit(char **cmd_array, t_mini *attributes);

//heredoc
int			here_doc_handler(t_mini *attributes, char *delimit);
int			process_heredocs(t_cmd_table *node, t_mini *attributes);
int			check_for_heredocs(t_cmd_table *node, t_mini *attributes);
int			ultimate_check_heredoc(t_cmd_table *node, t_mini *atrbs, int index);

//cleanup
void		envp_cleanup(t_mini *attributes);
void		clean_envp_ll(t_envp **envp_heap);
void		cleanup_child(t_mini *attributes);
void		cleanup_parent(t_mini *attributes);

//other general utils
void		free_array(char **array);
int			ft_strcmp(char *s1, char *s2);
int			save_std(t_mini *attributes, int fd);
void		restore_std(int saved_fd, int fd, t_mini *attributes);
void		check_and_free_string(char *str);

//error messages
void		set_error_and_display(int err, t_mini *attribs, const char *cmd);
void		syscall_fail(int err_code, t_mini *attributes, const char *message);
void		update_exitcode(int err_code, t_mini *attributes);
void		update_exit_recursion(t_mini *attributes);

#endif
