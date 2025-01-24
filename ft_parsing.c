/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:12:26 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/24 13:27:23 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_merge_pipe(t_mini *attributes, t_cmd_table *old_table)
{
	t_cmd_table	*new_pipe;

	new_pipe = malloc(sizeof(t_cmd_table));
	if (new_pipe == NULL)
		return (0);
	new_pipe->type = t_pipe;
	new_pipe->right = old_table;
	new_pipe->left = attributes->commands;
	attributes->commands = new_pipe;
	return (1);
}

t_cmd_table	*ft_merge(t_mini *attributes, t_tokens **token, t_cmd_table *old)
{
	attributes->type_count += 1;
	if ((*token)->type >= t_great && (*token)->type <= t_lessless)
	{
		old->herefile = ft_add_cmd_arr(old->herefile, (*token)->next->str);
		if (old->herefile == NULL)
		{
			ft_free_merge_fail(old);
			return (NULL);
		}
	}
	old->type_arr = ft_add_type_arr(attributes, old->type_arr, (*token)->type);
	if (old->type_arr == NULL)
	{
		ft_free_merge_fail(old);
		return (NULL);
	}
	*token = (*token)->next;
	if ((*token)->prev->type == t_great || (*token)->prev->type == t_greatgreat)
		old->last_outfile = (*token)->prev->type;
	if ((*token)->prev->type == t_less || (*token)->prev->type == t_lessless)
		old->last_infile = (*token)->prev->type;
	old->type = (*token)->prev->type;
	return (old);
}

t_cmd_table	*ft_merge_command(t_mini *attributes, t_tokens **token)
{
	t_cmd_table	*node;

	node = ft_add_new(*token);
	while (node && *token != NULL && (*token)->type != t_pipe)
	{
		if ((*token)->type >= t_great && (*token)->type <= t_lessless)
		{
			node = ft_merge(attributes, token, node);
			if (node == NULL)
				return (NULL);
			node->failexp = (*token)->failexp;
		}
		else if ((*token)->failexp == 0)
		{	
			node->cmd_arr = ft_add_cmd_arr(node->cmd_arr, (*token)->str);
			if (node->cmd_arr == NULL)
			{	
				free(node);
				node = NULL;
				return (NULL);
			}
		}
		*token = (*token)->next;
	}
	return (node);
}

int	ft_start_parsing(t_mini *attributes, t_tokens *token)
{
	t_cmd_table	*new_table;

	attributes->commands = ft_merge_command(attributes, &token);
	if (attributes->commands == NULL)
		return (0);
	attributes->type_count = 0;
	attributes->cmd_index += 1;
	while (token != NULL && token->type == t_pipe)
	{
		token = token->next;
		new_table = ft_merge_command(attributes, &token);
		if (new_table == NULL)
			return (0);
		attributes->type_count = 0;
		attributes->cmd_index += 1;
		if (!(ft_merge_pipe(attributes, new_table)))
		{
			ft_check_ast_array(new_table);
			free(new_table);
			new_table = NULL;
			return (0);
		}
	}
	return (1);
}

int	ft_parsing(t_mini *attributes)
{
	t_tokens	*token;

	token = attributes->tokens;
	attributes->type_count = 0;
	attributes->cmd_index = 0;
	attributes->commands = NULL;
	ft_convert(attributes);
	if (!(ft_start_parsing(attributes, token)))
	{
		ft_free_tokens(attributes);
		ft_free_ast(attributes);
		ft_putstr_fd("Parsing error\n", 2);
		return (0);
	}
	ft_free_tokens(attributes);
	return (1);
}
