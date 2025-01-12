/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:12:26 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/12 22:24:52 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_merge_pipe(t_mini *attributes, t_cmd_table *old_table)
{
	t_cmd_table	*new_pipe;

	new_pipe = malloc(sizeof(t_cmd_table)); // use add new functon? // checked
	if (new_pipe == NULL)
		return (0);
	new_pipe->type = t_pipe;
	new_pipe->right = old_table;
	new_pipe->left = attributes->commands;
	attributes->commands = new_pipe;
	return (1);
}

t_cmd_table	*ft_merge_redirection(t_mini *attributes, t_tokens **token, t_cmd_table *old_table)
{
	attributes->type_count += 1;
	if ((*token)->type == t_great || (*token)->type == t_greatgreat || (*token)->type == t_less || (*token)->type == t_lessless)
	{
		old_table->herefile = ft_add_command_array(old_table->herefile, (*token)->next->str);
		if (old_table->herefile == NULL)
			return (NULL);
	}
	old_table->type_arr = ft_add_type_array(attributes, old_table->type_arr, (*token)->type);
	if (old_table->type_arr == NULL)
	{
		ft_check_ast_array(old_table);
		free(old_table);
		return (NULL);
	}
	*token = (*token)->next;
	if ((*token)->prev->type == t_great || (*token)->prev->type == t_greatgreat)
		old_table->last_outfile = (*token)->prev->type;
	if ((*token)->prev->type == t_less || (*token)->prev->type == t_lessless)
		old_table->last_infile = (*token)->prev->type;
	old_table->type = (*token)->prev->type;
	return (old_table);
}

t_cmd_table	*ft_merge_command(t_mini *attributes, t_tokens **token)
{
	t_cmd_table	*new_node;

	new_node = ft_add_new(*token); //checked
	if (new_node == NULL)
		return (NULL);
	while (*token != NULL && (*token)->type != t_pipe)
	{
		if (*token && ((*token)->type == t_great || (*token)->type == t_less || (*token)->type == t_lessless || (*token)->type == t_greatgreat))
		{
			new_node = ft_merge_redirection(attributes, token, new_node);
			if (new_node == NULL)
				return (NULL);
		}
		else
		{
			new_node->cmd_arr = ft_add_command_array(new_node->cmd_arr, (*token)->str);
			if (new_node->cmd_arr == NULL)
			{	
				free(new_node); // might need to free arrays int node ??
				return (NULL);
			}
		}
		*token = (*token)->next;
	}
	attributes->type_count = 0;
	attributes->cmd_index += 1;
	return (new_node);
}

int	ft_start_parsing(t_mini *attributes)
{
	t_tokens	*token;
	t_cmd_table	*new_table;

	token = attributes->tokens;
	attributes->cmd_index = 0;
	attributes->commands = ft_merge_command(attributes, &token); //checkedd
	if (attributes->commands == NULL)
		return (0);
	while (token != NULL && token->type == t_pipe)
	{
		token = token->next;
		new_table = ft_merge_command(attributes, &token); // checked
		if (new_table == NULL)
			return (0);
		if (!(ft_merge_pipe(attributes, new_table))) // checked
		{
			ft_check_ast_array(new_table);
			free(new_table);
			return (0);
		}
	}
	return (1);
}

int	ft_parsing(t_mini *attributes)
{
	attributes->type_count = 0;
	attributes->cmd_index = 0;
	attributes->commands = NULL;
	if (!(ft_start_parsing(attributes)))
	{
		ft_free_tokens(attributes);
		ft_free_ast(attributes);
		ft_putstr_fd("Parsing error\n", 2);
		return (0);
	}
	//ft_print_table(attributes);
	ft_free_tokens(attributes);
	return (1);
}
