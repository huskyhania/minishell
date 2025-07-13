/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 20:07:51 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/24 13:55:22 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_table	*ft_find_last_parse(t_cmd_table *stack)
{
	if (!stack)
		return (NULL);
	while (stack->right)
		stack = stack->right;
	return (stack);
}

t_cmd_table	*ft_add_new(t_tokens *token)
{
	t_cmd_table	*new_node;

	new_node = malloc(sizeof(t_cmd_table));
	if (!new_node)
		return (NULL);
	new_node->right = NULL;
	new_node->left = NULL;
	new_node->cmd_arr = NULL;
	new_node->input_fd = 0;
	new_node->output_fd = 1;
	new_node->in1 = NULL;
	new_node->out1 = NULL;
	new_node->herefile = NULL;
	new_node->type_arr = NULL;
	new_node->last_outfile = 0;
	new_node->last_infile = 0;
	new_node->failexp = 0;
	new_node->type = (token->type);
	return (new_node);
}

int	ft_count_array(char **old_array)
{
	int	i;

	i = 0;
	if (!old_array || !old_array[i])
		return (0);
	while (old_array && old_array[i])
		i++;
	return (i);
}

char	**ft_fill_array(char **old_array, char **new_array, char *str)
{
	int	i;

	i = 0;
	while (old_array && old_array[i])
	{
		new_array[i] = ft_strdup(old_array[i]);
		if (new_array[i] == NULL)
		{
			ft_free_cmd_array(old_array);
			ft_free_cmd_array(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[i] = ft_strdup(str);
	if (new_array[i] == NULL)
	{
		ft_free_cmd_array(old_array);
		ft_free_cmd_array(new_array);
		return (NULL);
	}
	new_array[i + 1] = 0;
	ft_free_cmd_array(old_array);
	return (new_array);
}

char	**ft_add_cmd_arr(char **old_array, char *str)
{
	char	**new_array;
	int		size;

	size = ft_count_array(old_array);
	new_array = malloc((size + 2) * sizeof(char *));
	if (!new_array)
	{	
		ft_free_cmd_array(old_array);
		return (NULL);
	}
	new_array = ft_fill_array(old_array, new_array, str);
	if (new_array == NULL)
		return (NULL);
	return (new_array);
}
