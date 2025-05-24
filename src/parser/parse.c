#include "cub3d.h"
#include "libft.h"

void print_gamevar(const t_game *g)
{
    printf("=== t_game contents ===\n");
    printf("mapfile_path          : [%s]\n",   g->mapfile_path);
    printf("no_path               : [%s]\n",   g->no_path);
    printf("so_path               : [%s]\n",   g->so_path);
    printf("we_path               : [%s]\n",   g->we_path);
    printf("ea_path               : [%s]\n",   g->ea_path);
    printf("floor_color           : [%d,%d,%d]\n",
           g->floor_color[0], g->floor_color[1], g->floor_color[2]);
    printf("ceiling_color         : [%d,%d,%d]\n",
           g->ceiling_color[0], g->ceiling_color[1], g->ceiling_color[2]);
	printf("floor_color_hex       : [%d]\n",   g->floor_color_hex);
	printf("ceiling_color_hex     : [%d]\n",   g->ceiling_color_hex);
	printf("map                   : [%p]\n",   (void*)g->map);
    printf("map_width             : [%d]\n",   g->map_width);
    printf("map_height            : [%d]\n",   g->map_height);
    printf("player_x              : [%f]\n",   g->player_x);
    printf("player_y              : [%f]\n",   g->player_y);
    printf("player_dir            : [%c]\n",   g->player_dir ? g->player_dir        : '0');
    printf("has_player_dir        : [%d]\n",   g->has_player_dir);
    printf("has_no                : [%d]\n",   g->has_no);
    printf("has_so                : [%d]\n",   g->has_so);
    printf("has_we                : [%d]\n",   g->has_we);
    printf("has_ea                : [%d]\n",   g->has_ea);
    printf("has_floor             : [%d]\n",   g->has_floor);
    printf("has_ceiling           : [%d]\n",   g->has_ceiling);
    printf("========================\n");
}

int rgb_to_hex(int r, int g, int b)
{
	return (r * 256 * 256) + (g * 256) + b;
}

// void	ft_free(char **arr)
// {
// 	int	i;

// 	i = 0;
// 	while (arr && arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }

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

void	valid_extention_check(t_game *game, t_data *data)
{
	int		i;
	int		j;
	char	*str;

	i = ft_strlen(game->mapfile_path);
	j = 4;
	str = ".cub";
	if (game->mapfile_path[i - 5] == '/' || i <= j)
	{
		write(2, "Error Wrong file extension\n", 27);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	while (j >= 0)
	{
		if (game->mapfile_path[i] != str[j])
		{
			write(2, "Error Wrong file extension\n", 27);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i--;
		j--;
	}
}

int	parse_rgb_color(int *color_code, char *str, t_data *data)
{
	char	**rgb;
	int		i;

	i = 0;
	rgb = ft_split(str, ',', data);
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
	{
		write(2, "rgb error\n", 10);
		ft_gc_free_all(&data->gc);
		exit(1);
	}
	while (i < 3)
	{
		color_code[i] = ft_atoi(rgb[i]);
		if (color_code[i] < 0 || color_code[i] > 255)
		{
			write(2, "rgb range error\n", 16);
			ft_gc_free_all(&data->gc);
			exit(1);
		}
		i++;
	}
	return ((color_code[0] * 256 * 256) + (color_code[1] * 256) + color_code[2]);
}

int	has_direction(t_game *gamevar)
{
	if (gamevar->has_no >= 2 || gamevar->has_so >= 2 || gamevar->has_we >= 2
		|| gamevar->has_ea >= 2 || gamevar->has_floor >= 2
		|| gamevar->has_ceiling >= 2)
	{
		write(2, "direction error\n", 16);
		return (0);
	}
	if (gamevar->has_no == 1 && gamevar->has_so == 1 && gamevar->has_we == 1
		&& gamevar->has_ea == 1 && gamevar->has_floor == 1
		&& gamevar->has_ceiling == 1)
		gamevar->state = PARSE_MAP_STATE;
	return (1);
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

int	map_is_valid(char *str, t_game *gamevar)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen_v2(str);
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (str[0] != '1' || str[len - 1] != '1')
		{
			write(2, "map error(map rows should be closed by walls)\n", 46);
			return (0);
		}
		if ((str[i] == 'N' || str[i] == 'S' || str[i] == 'E' || str[i] == 'W')
			&& gamevar->has_player_dir == 0)
		{
			gamevar->player_dir = str[i];
			gamevar->player_x = i;
			gamevar->player_y = gamevar->map_height - 1;
			gamevar->has_player_dir = 1;
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

char *set_terminator(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\n')
	{
		i++;
	}
	str[i] = '\0';
	
	return (str);
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
				gamevar->no_path = ft_strdup(set_terminator(parts[1]), data);
				gamevar->has_no++;
			}
			else if (ft_strcmp(parts[0], "SO") == 0)
			{
				gamevar->so_path = ft_strdup(set_terminator(parts[1]), data);
				gamevar->has_so++;
			}
			else if (ft_strcmp(parts[0], "WE") == 0)
			{
				gamevar->we_path = ft_strdup(set_terminator(parts[1]), data);
				gamevar->has_we++;
			}
			else if (ft_strcmp(parts[0], "EA") == 0)
			{
				gamevar->ea_path = ft_strdup(set_terminator(parts[1]), data);
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
			if (!map_is_valid(line, gamevar))
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

void	var_init(t_game *gamevar)
{
	int	i;

	i = 0;
	gamevar->mapfile_path = NULL;
	gamevar->no_path = NULL;
	gamevar->so_path = NULL;
	gamevar->we_path = NULL;
	gamevar->ea_path = NULL;
	while (i < 3)
	{
		gamevar->floor_color[i] = -1;
		gamevar->ceiling_color[i] = -1;
		i++;
	}
	gamevar->floor_color_hex = 0;
	gamevar->ceiling_color_hex = 0;
	gamevar->map = NULL;
	gamevar->map_width = 0;
	gamevar->map_height = 0;
	gamevar->player_x = -1;
	gamevar->player_y = -1;
	gamevar->player_dir = '\0';
	gamevar->has_player_dir = 0;
	gamevar->has_no = 0;
	gamevar->has_so = 0;
	gamevar->has_we = 0;
	gamevar->has_ea = 0;
	gamevar->has_floor = 0;
	gamevar->has_ceiling = 0;
	gamevar->state = INITIAL;
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
			while (line[j] != '\0' && line[j] != '\n')
			{
				gamevar->map[i][j] = line[j];
				j++;
			}
			while (j < gamevar->map_width)
			{
				gamevar->map[i][j] = '\0';
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

	//print_gamevar(gamevar);
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
	var_init(gamevar);

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
	//int j = 0;
	// while (gamevar->map[j] != NULL)
	// {
	// 	printf("%s\n", gamevar->map[j]);
	// 	j++;
	// }
	return (gamevar);
}