#include "parser.h"

void    skip_empty_line(int fd)
{
    char *line;

    line = get_next_line(fd);
    while (line != NULL);
    {
        
    }
}

void    var_init(t_game* gamevar)
{
    int i = 0;
    
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
    gamevar->map = NULL
    gamevar->map_width = 0;
    gamevar->map_heigth = 0;
    gamevar->player_x = -1;
    gamevar->player_y = -1;
    gamevar->player_dir = '\0';
    game->has_no = 0;
    game->has_so = 0;
    game->has_we = 0;
    game->has_ea = 0;
    game->has_floor = 0;
    game->has_ceiling = 0;
}

int parser(int argc, char **argv)
{
    int fd;
    t_game* gamevar;

    if (argc != 2)
    {
        write(2, "Error\n", 6);
        return (1);
    }
    
    /* 
        texture_files_check()
        {}
    */
    var_init(&gamevar);
    fd = open("map.cub", O_RDONLY);
    if (fd == -1 /* || !valid_extention_check()*/)
    {
        write(2, "Error\n", 6);
        return (1);
    }
    skip_empty_lines(fd);

}