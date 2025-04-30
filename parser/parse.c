#include "parser.h"

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

	rgb = ft_split(str, ',');
	color_code[0] = ft_atoi(rgb[0]);
	color_code[1] = ft_atoi(rgb[1]);
	color_code[2] = ft_atoi(rgb[2]);
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

void    parse_line(int fd, t_game *gamevar)
{
    char    *line;
    char    **parts;
	
    gamevar->state = INITIAL;
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
				return(1);
			}
			gamevar->state = PARSE_DIRECTION_STATE;
			if (ft_strcmp(parts[0], "NO") == 0)
			{
				gamevar->no_path = parts[1];
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

		}
		line = get_next_line(fd);
    }
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
    gamevar->has_no = 0;
    gamevar->has_so = 0;
    gamevar->has_we = 0;
    gamevar->has_ea = 0;
    gamevar->has_floor = 0;
    gamevar->has_ceiling = 0;
}

int parser(int argc, char **argv)
{
    int     fd;
    t_game  *gamevar;

    if (argc != 2)
    {
		write(2, "Error\n", 6);
        return (1);
    }
	//  texture_files_check()
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
	printf("%s", gamevar->ea_path);
	close(fd);

}