#ifndef PARSER_H
# define PARSER_H

# include "../cub3d.h"

typedef struct s_game
{
    char *no_path;
    char *so_path;
    char *we_path;
    char *ea_path;
    int floor_color[3];
    int ceiling_color[3];
    char **map;
    int map_width;
    int map_height;
    int player_x;
    int player_y;
    char player_dir;
    int has_no;
    int has_so;
    int has_we;
    int has_ea;
    int has_floor;
    int has_ceiling;
} t_game;

#endif