/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:52:13 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 15:55:31 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

int	line_is_empty(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

int	ft_strlen_v2(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
	{
		i++;
	}
	return (i);
}

void	check_file_exists(char *filepath, t_data *data)
{
	int fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Error texture file path not found\n", 34);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	close(fd);
}