/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:12:01 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 21:29:10 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

static void	fill_map_row(t_game *gamevar, char *line, int i)
{
	int	j;

	j = 0;
	while (line[j] == ' ')
	{
		gamevar->map[i][j] = '1';
		j++;
	}
	while (line[j] != '\0' && line[j] != '\n')
	{
		if (line[j] == ' ')
			line[j] = '1';
		gamevar->map[i][j] = line[j];
		j++;
	}
	while (j < gamevar->map_width)
	{
		gamevar->map[i][j] = '1';
		j++;
	}
	gamevar->map[i][j] = '\0';
}

static void	read_and_fill_map(int fd, t_game *gamevar, t_data *data)
{
	int		i;
	char	*line;

	i = 0;
	line = get_next_line(fd, data);
	while (line != NULL)
	{
		if (line_is_empty(line))
		{
			line = get_next_line(fd, data);
			continue ;
		}
		else if (i < gamevar->map_height)
		{
			fill_map_row(gamevar, line, i);
			i++;
		}
		line = get_next_line(fd, data);
	}
}

void	fill_map(t_game *gamevar, t_data *data)
{
	int	fd;

	allocate_map(gamevar, data);
	fd = open(gamevar->mapfile_path, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Fd error\n", 9);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	skip_config_lines(fd, data);
	read_and_fill_map(fd, gamevar, data);
	validate_map_borders(gamevar, data);
	close(fd);
}
