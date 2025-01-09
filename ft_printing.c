#include "minishell.h"

void print_helper(t_cmd_table *print)
{
	t_cmd_table *print_temp = print;
	print_temp = print_temp->left;
	while(print_temp)
	{
		printf("Redirection node: %s type: %d\n", print_temp->str, print_temp->type);
		print_temp = print_temp->left;
	}
}

void print_array(t_cmd_table *print)
{
	int i = 0;
	while (print && print->cmd_arr && print->cmd_arr[i])
	{
		printf("Index %d string|%s|\n", i, print->cmd_arr[i]);
		//printf("Type: %d ", print->type);
		i++;
	}
	if (print->cmd_arr)
		printf("Index %d string|%s|\n", i, print->cmd_arr[i]);
}

void print_file(t_cmd_table *print)
{
	int i = 0;
	print_array(print);
	printf("Last infile %d Last outfile %d\n", print->last_infile, print->last_outfile);
	/*while (print->cmd_arr && print->cmd_arr[0])
	{
		if (!print->cmd_arr[i])
			break ;
		printf("String type: %d string|%s|\n",print->type,print->cmd_arr[i]);
		i++;
	}
	i=0;*/
	while (print && print->outfile && print->outfile[0])
	{
		if (!print->outfile[i])
			break ;
		printf("Type: %d Index %d Outfile:|%s|\n", print->type ,i, print->outfile[i]);
		i++;
	}
	i=0;
	while (print && print->infile && print->infile[0])
	{
		if (!print->infile[i])
			break ;
		printf("Type: %d Index %d Infile:|%s|\n", print->type ,i, print->infile[i]);
		i++;
	}
	i=0;
	while (print && print->here && print->here[0])
	{
		if (!print->here[i])
			break ;
		printf("Type: %d Index %d Here:|%s|\n", print->type ,i, print->here[i]);
		i++;
	}
	i=0;
	while (print && print->append && print->append[0])
	{
		if (!print->append[i])
			break ;
		printf("Type: %d Index %d Append:|%s|\n", print->type ,i, print->append[i]);
		i++;
	}
}

void ft_print_table(t_mini *attributes)
{
	//printf("COMMAND TABLE:\n");
	t_cmd_table *print = attributes->commands;
	//printf("CMD COUNT %d\n",attributes->cmd_index);
	while(print)
	{
		if (print->type == t_command)
			print_array(print);
		//printf("first node: %s type: %d\n", print->str, print->type);
		if (print->type == t_great || print->type == t_less || print->type == t_greatgreat || print->type == t_lessless)
		{
			print_file(print);
			//printf("string|%s|\n", print->cmd_arr[0]);
			//printf("Type: %d Infile:|%s| Outfile:|%s|\n Deli:|%s|\n", print->type, print->infile, print->outfile[0], print->here);
		}
		if (print->type == t_pipe && print->right)
		{
			print_file(print->right);
		}
		if (print->type == t_pipe)
		{
			printf("PIPE\n");
			print = print->left;
		}
		else
			break ;
	}
	//printf("COMMAND TABLE FINITO:\n");
}
