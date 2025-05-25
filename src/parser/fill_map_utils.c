/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_map_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:08:04 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 21:28:48 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

void	allocate_map(t_game *gamevar, t_data *data)
{
	int	i;

	gamevar->map = ft_gc_malloc(&data->gc, sizeof(char *) * (gamevar->map_height
				+ 1));
	if (gamevar->map == NULL)
	{
		write(2, "Map allocation failed\n", 22);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	i = 0;
	while (i < gamevar->map_height)
	{
		gamevar->map[i] = ft_gc_malloc(&data->gc, sizeof(char)
				* (gamevar->map_width + 1));
		if (gamevar->map[i] == NULL)
		{
			write(2, "Map allocation failed\n", 22);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i++;
	}
	gamevar->map[gamevar->map_height] = NULL;
}

void	skip_config_lines(int fd, t_data *data)
{
	int		count;
	char	*line;

	count = 0;
	line = get_next_line(fd, data);
	while (line != NULL)
	{
		if (ft_strnstr(line, "NO", ft_strlen_v2(line)) || ft_strnstr(line, "SO",
				ft_strlen_v2(line)) || ft_strnstr(line, "WE",
				ft_strlen_v2(line)) || ft_strnstr(line, "EA",
				ft_strlen_v2(line)) || ft_strnstr(line, "F", ft_strlen_v2(line))
			|| ft_strnstr(line, "C", ft_strlen_v2(line)))
		{
			count++;
			line = get_next_line(fd, data);
			continue ;
		}
		if (count >= 6)
		{
			break ;
		}
		line = get_next_line(fd, data);
	}
}

void	validate_map_borders(t_game *gamevar, t_data *data)
{
	int	i;

	i = 0;
	while (gamevar->map[0][i] != '\0')
	{
		if (gamevar->map[0][i] == ' ')
			gamevar->map[0][i] = '1';
		if (gamevar->map[0][i] != '1')
			exit_error("error (first map row should all 1)", data);
		i++;
	}
	i = 0;
	while (gamevar->map[gamevar->map_height - 1][i])
	{
		if (gamevar->map[gamevar->map_height - 1][i] == ' ')
			gamevar->map[gamevar->map_height - 1][i] = '1';
		if (gamevar->map[gamevar->map_height - 1][i] != '1')
			exit_error("error (last map row should be all 1)", data);
		i++;
	}
}
