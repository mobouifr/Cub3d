/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:51:25 by mobouifr          #+#    #+#             */
/*   Updated: 2025/05/25 16:50:51 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

int	map_char_valid(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}
void set_player_direction(t_game *gamevar, char c, int i)
{
	gamevar->player_dir = c;
	gamevar->player_x = i;
	gamevar->player_y = gamevar->map_height - 1;
	gamevar->has_player_dir = 1;
}

int line_closed_by_walls(char *str, t_data *data)
{
	int len;
	char *trimmed_str;
	
	trimmed_str = ft_strtrim(str, " ", data);
	len = ft_strlen_v2(trimmed_str);
	if (trimmed_str[0] != '1' || trimmed_str[len - 1] != '1')
	{
		write(2, "map error(map rows should be closed by walls)\n", 46);
		return (0);
	}
	return (1);
}

int	map_row_is_valid(char *str, t_game *gamevar, t_data *data)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (!line_closed_by_walls(str, data))
			return (0);
		if (map_char_valid(str[i]) && gamevar->has_player_dir == 0)
		{
			set_player_direction(gamevar, str[i], i);
			i++;
			if (i > gamevar->map_width)
				gamevar->map_width++;
		}
		else if (str[i] == ' ' || str[i] == '1' || str[i] == '0')
		{
			i++;
			if (i > gamevar->map_width)
				gamevar->map_width++;
		}
		else
		{
			write(2, "non valid character\n", 21);
			return (0);
		}
	}

	return (1);
}



void	parse_line(int fd, t_game *gamevar, t_data *data)
{
	char	*line;
	char	**parts;

	line = get_next_line(fd, data);
	while (line != NULL)
	{
		if (line_is_empty(line))
		{
			line = get_next_line(fd, data);
			continue ;
		}
		if (!has_direction(gamevar))
		{
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		if (gamevar->state == INITIAL
			|| gamevar->state == PARSE_DIRECTION_STATE)
		{
			parts = ft_split(line, ' ', data);
			if (!parts || !parts[0] || !parts[1] || parts[2])
			{
				write(2, "input	error\n", 12);
				ft_gc_free_all(&data->gc);
				exit(1);
			}
			gamevar->state = PARSE_DIRECTION_STATE;
			if (ft_strcmp(parts[0], "NO") == 0)
			{
				gamevar->no_path = ft_strdup(parts[1], data);
				check_file_exists(gamevar->no_path, data);
				gamevar->has_no++;
			}
			else if (ft_strcmp(parts[0], "SO") == 0)
			{
				gamevar->so_path = ft_strdup(parts[1], data);
				check_file_exists(gamevar->so_path, data);
				gamevar->has_so++;
			}
			else if (ft_strcmp(parts[0], "WE") == 0)
			{
				gamevar->we_path = ft_strdup(parts[1], data);
				check_file_exists(gamevar->we_path, data);
				gamevar->has_we++;
			}
			else if (ft_strcmp(parts[0], "EA") == 0)
			{
				gamevar->ea_path = ft_strdup(parts[1], data);
				check_file_exists(gamevar->ea_path, data);
				gamevar->has_ea++;
			}
			else if (ft_strcmp(parts[0], "F") == 0)
			{
				gamevar->floor_color_hex = parse_rgb_color(gamevar->floor_color, parts[1], data);
				gamevar->has_floor++;
			}
			else if (ft_strcmp(parts[0], "C") == 0)
			{
				gamevar->ceiling_color_hex = parse_rgb_color(gamevar->ceiling_color, parts[1], data);
				gamevar->has_ceiling++;
			}
		}
		if (gamevar->state == PARSE_MAP_STATE)
		{
			gamevar->map_height++;
			if (!map_row_is_valid(line, gamevar, data))
			{
				close(fd);
				ft_gc_free_all(&data->gc);
				exit(1);
			}
		}
		line = get_next_line(fd, data);
	}
	if (gamevar->map_height == 0 || gamevar->has_player_dir == 0 || gamevar->player_dir == 0)
	{
		write (2, "Error, map not found\n", 21);
		close(fd);
		ft_gc_free_all(&data->gc);
		puts("after gc free ALL");
		exit(1);
	}
	close(fd);
}


void	fill_map(t_game *gamevar, t_data *data)
{
	int		i;
	char	*line;
	int		fd;
	int		count;
	int		j;

	i = 0;
	gamevar->map = ft_gc_malloc(&data->gc, sizeof(char *) * (gamevar->map_height + 1));
	if (gamevar->map == NULL)
	{
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	while (i < gamevar->map_height)
	{
		gamevar->map[i] = ft_gc_malloc(&data->gc, sizeof(char) * (gamevar->map_width + 1));
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
	int fd;
	t_game *gamevar;

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
		exit (1);
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
	int j = 0;
	while (gamevar->map[j] != NULL)
	{
			printf("%s\n", gamevar->map[j]);
			j++;
	}
		print_gamevar(gamevar);
	return (gamevar);
}
