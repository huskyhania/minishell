/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llaakson <llaakson@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:42:19 by llaakson          #+#    #+#             */
/*   Updated: 2025/01/11 18:41:42 by llaakson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_type_array(int *arr)
{
	if (!arr)
		return ;
	if (arr)
	{
		free(arr);
		arr = NULL;
	}
}

int	*ft_add_type_array(t_mini *attributes, int *old_array, int type)
{
	int *new_array;
	int size;
	int i;

	size = attributes->type_count;
	//printf("Counting done size %d\n",size);
	new_array = malloc((size + 1) * sizeof(int));
	if (!new_array)
		printf("no new array");//add malloc check
	//printf("Malloc done\n");
	i = 0;
	while (size-- >= 2)
	{
		new_array[i] = old_array[i];
		//printf("Type %d Index %d size %d \n", old_array[i], i, size);
		i++;
	}
	//printf("array done\n");
	new_array[i] = type;
	new_array[i+1] = 0;
	if (old_array)
		free(old_array);
	return (new_array);
}

