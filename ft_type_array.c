/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:42:19 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/20 15:00:51 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_type_array(int *array)
{
	if (!array)
		return ;
	if (array)
	{
		free(array);
		array = NULL;
	}
}

int	*ft_add_type_arr(t_mini *attributes, int *old_array, int type)
{
	int	*new_array;
	int	size;
	int	i;

	size = attributes->type_count;
	new_array = malloc((size + 1) * sizeof(int));
	if (!new_array)
	{
		free(old_array);
		return (NULL);
	}
	i = 0;
	while (size-- >= 2)
	{
		new_array[i] = old_array[i];
		i++;
	}
	new_array[i] = type;
	new_array[i + 1] = 0;
	if (old_array)
		free(old_array);
	return (new_array);
}
