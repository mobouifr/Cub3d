/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:51:25 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 18:51:04 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"


void	fill_map(t_game *gamevar, t_data *data)
{
	int		i;
	char	*line;
	int		fd;
	int		count;
	int		j;

	i = 0;
	gamevar->map = ft_gc_malloc(&data->gc, sizeof(char *) * (gamevar->map_height
				+ 1));
	if (gamevar->map == NULL)
	{
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	while (i < gamevar->map_height)
	{
		gamevar->map[i] = ft_gc_malloc(&data->gc, sizeof(char)
				* (gamevar->map_width + 1));
		if (gamevar->map[i] == NULL)
		{
			return ;
		}
		i++;
	}
	gamevar->map[gamevar->map_height] = NULL;
	fd = open(gamevar->mapfile_path, O_RDONLY);
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
	i = 0;
	while (line != NULL)
	{
		if (line_is_empty(line))
		{
			line = get_next_line(fd, data);
			continue ;
		}
		else if (i < gamevar->map_height)
		{
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
			i++;
		}
		line = get_next_line(fd, data);
	}
	i = 0;
	while (gamevar->map[0][i] != '\0')
	{
		if (gamevar->map[0][i] == ' ')
			gamevar->map[0][i] = '1';
		if (gamevar->map[0][i] != '1')
		{
			write(2, "error (first map row should all 1)\n", 35);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i++;
	}
	i = 0;
	while (gamevar->map[gamevar->map_height - 1][i])
	{
		if (gamevar->map[gamevar->map_height - 1][i] == ' ')
			gamevar->map[gamevar->map_height - 1][i] = '1';
		if (gamevar->map[gamevar->map_height - 1][i] != '1')
		{
			write(2, "error (last map row should be all 1)\n", 37);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i++;
	}
}

t_game	*parser(int argc, char **argv, t_data *data)
{
	int		fd;
	t_game	*gamevar;
	int		j;

	if (argc != 2)
	{
		write(2, "Wrong number of argument\n", 25);
		exit(1);
	}
	gamevar = ft_gc_malloc(&data->gc, sizeof(t_game));
	if (!gamevar)
	{
		write(2, "allocation Error\n", 17);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	vars_init(gamevar);
	gamevar->mapfile_path = argv[1];
	valid_extention_check(gamevar, data);
	fd = open(gamevar->mapfile_path, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Fd error\n", 9);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	parse_line(fd, gamevar, data);
	fill_map(gamevar, data);
	close(fd);
	j = 0;
	while (gamevar->map[j] != NULL)
	{
		printf("%s\n", gamevar->map[j]);
		j++;
	}
	print_gamevar(gamevar);
	return (gamevar);
}
