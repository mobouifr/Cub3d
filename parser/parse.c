#include "parser.h"

void ft_free(char **arr)
{
    int i = 0;
    while (arr && arr[i])
	{
        free(arr[i]);
		i++;
	}
	free(arr);
}

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

int	valid_extention_check(t_game *game)
{
	int		i;
	int		j;
	char	*str;

	i = ft_strlen(game->mapfile_path);
	j = 4;
	str = ".cub";
	if (game->mapfile_path[i - 5] == '/' || i <= j)
	{
		write(2, "Error\nWrong file extension\n", 27);
		return (0);
	}
	while (j >= 0)
	{
		if (game->mapfile_path[i] != str[j])
		{
			write(2, "Error\nWrong file extension\n", 27);
			return (0);
		}
		i--;
		j--;
	}
	return (1);
}

void	parse_rgb_color(int	*color_code, char *str)
{
	char **rgb;
	int i;


	i = 0;
	rgb = ft_split(str, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
    {
        write(2, "error\n", 6);
        // free the 2d array;
        exit(1);
    }
	while (i < 3)
    {
        color_code[i] = ft_atoi(rgb[i]);
        if (color_code[i] < 0 || color_code[i] > 255)
        {
            write(2, "error\n", 6);
            // free the 2d array;
            exit(1);
        }
        i++;
    }
}

int	has_direction(t_game *gamevar)
{
	if (gamevar->has_no >= 2 || gamevar->has_so >= 2 || gamevar->has_we >= 2 || gamevar->has_ea >= 2 || gamevar->has_floor >= 2 || gamevar->has_ceiling >= 2)
	{
		write(2, "error\n", 6);
		return(1);
	}
	if (gamevar->has_no == 1 && gamevar->has_so == 1 && gamevar->has_we == 1 && gamevar->has_ea == 1 && gamevar->has_floor == 1 && gamevar->has_ceiling == 1)
		gamevar->state = PARSE_MAP_STATE;
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

int	map_is_valid(char *str, t_game *gamevar)
{
	int	i;
	int len;

	i = 0;
	len = ft_strlen_v2(str);
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (str[0] != '1' || str[len - 1] != '1')
		{
			write (2, "error\n", 6);
			return (0);
		}
		if ((str[i] == 'N' || str[i] == 'S' || str[i] == 'E' || str[i] == 'W') && gamevar->has_player_dir == 0)
		{
			gamevar->player_dir = str[i];
			gamevar->player_x = i;
			gamevar->player_y = gamevar->map_height;
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
			write (2, "error\n", 6);
			return (0);
		}
	}
	return (1);
}

void    parse_line(int fd, t_game *gamevar)
{
    char    *line;
    char    **parts;
	
	line = get_next_line(fd);
    while (line != NULL)
    {
		if (line_is_empty(line))
		{
			line = get_next_line(fd);
			continue;
		}
		has_direction(gamevar);
		if (gamevar->state == INITIAL || gamevar->state == PARSE_DIRECTION_STATE)
		{
			parts = ft_split(line, ' ');
			if (!parts || !parts[0] || !parts[1] || parts[2])
			{
				write(2, "error\n", 6);
				exit (1);
			}
			gamevar->state = PARSE_DIRECTION_STATE;
			if (ft_strcmp(parts[0], "NO") == 0)
			{
				gamevar->no_path = ft_strdup(parts[1]);
				gamevar->has_no++;
			}
			else if (ft_strcmp(parts[0], "SO") == 0)
			{
				gamevar->so_path = ft_strdup(parts[1]);
				gamevar->has_so++;
			}
			else if (ft_strcmp(parts[0], "WE") == 0)
			{	
				gamevar->we_path = ft_strdup(parts[1]);
				gamevar->has_we++;
			}
			else if (ft_strcmp(parts[0], "EA") == 0)
			{
				gamevar->ea_path = ft_strdup(parts[1]);
				gamevar->has_ea++;
			}
			else if (ft_strcmp(parts[0], "F") == 0)
			{
				parse_rgb_color(gamevar->floor_color, parts[1]);
				gamevar->has_floor++;
			}
			else if (ft_strcmp(parts[0], "C") == 0)
			{
				parse_rgb_color(gamevar->ceiling_color, parts[1]);
				gamevar->has_ceiling++;
			}
		}
		if (gamevar->state == PARSE_MAP_STATE)
		{
			gamevar->map_height++;
			if (!map_is_valid(line, gamevar))
			{
				write(2, "error\n", 6);
				close (fd);
				free(line);
				exit (1);

			}
		}
		free(line);
		line = get_next_line(fd);
    }
	close(fd)
}

void    var_init(t_game* gamevar)
{
    int i = 0;

    gamevar->mapfile_path = NULL;
    gamevar->no_path = NULL;
    gamevar->so_path = NULL;
    gamevar->we_path = NULL;
    gamevar->ea_path = NULL;
    while(i < 3)
    {
        gamevar->floor_color[i] = -1;
        gamevar->ceiling_color[i] = -1;
        i++;
    }
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

void fill_map(t_game *gamevar)
{
	int	i;
	char *line;

	i = 0;
	gamevar->map = malloc(sizeof(char *) * (gamevar->map_height + 1));
	if (gamevar->map == NULL)
		return (NULL);
	while (i < gamevar->map_height)
	{
		gamevar->map[i] = malloc(sizeof(char) * (gamevar->map_width + 1));
		if (gamevar->map[i] == NULL)
		{
			//free already allocated lines
			return ;
		}
		i++;
	}
	gamevar->map[gamevar->map_height] = NULL;
	int	fd = open(gamevar->mapfile_path, O_RDONLY);
	int count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (ft_strnstr(line, "NO", ft_strlen_v2(line)) || ft_strnstr(line, "SO", ft_strlen_v2(line)) || ft_strnstr(line, "WE", ft_strlen_v2(line)) || ft_strnstr(line, "EA", ft_strlen_v2(line)) ||
			ft_strnstr(line, "F", ft_strlen_v2(line))  || ft_strnstr(line, "C", ft_strlen_v2(line)))
		{
			count++;
			free (line);
			continue ;
		}
		if (count >= 6)
		{
			break ;
		}
		free (line);
		line = get_next_line(fd);
	}
	i = 0;
	while (line != NULL)
	{
		if (line_is_empty(line))
		{
			line = get_next_line(fd);
			continue;
		}
		else if (i < gamevar->map_height) 
		{
			int j = 0;
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
		line = get_next_line(fd);
	}
	i = 0;
	while (gamevar->map[0][i] != '\0')
	{
		if (gamevar->map[0][i] != '1')
		{
			write (2, "error\n", 6);
			//free all the 2d array;
			exit (1);
		}
		i++;
	}
	i = 0;
	while (gamevar->map[gamevar->map_height - 1][i])
	{
		if (gamevar->map[gamevar->map_height - 1][i] != '1)'
		{
			write (2, "error\n", 6);
			//free all the 2d array;
			exit (1);
		}
		i++;
	}
}

int parser(int argc, char **argv)
{
    int     fd;
    t_game  *gamevar;

    if (argc != 2)
    {
		write(2, "Error\n", 6);
		exit (1);
    }

	gamevar = malloc(sizeof(t_game));
	if (!gamevar)
   		return (write(2, "Error\n", 6), 1);

    var_init(gamevar);

	gamevar->mapfile_path = argv[1];
    fd = open(gamevar->mapfile_path, O_RDONLY);
    if (fd == -1 || !valid_extention_check(gamevar))
    {
        write(2, "Error\n", 6);
        return (1);
    }

    parse_line(fd, gamevar);
	
	fill_map(gamevar);

	printf("%s", gamevar->ea_path);
	close(fd);

}