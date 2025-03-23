#ifndef CUB3D_H
# define CUB3D_H

# define TILE_SIZE 32

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <limits.h>
# include <mlx.h>
# include "get_next_line/get_next_line.h"

typedef struct s_player
{
    double player_x;
    double player_y;
    int radius;
    int turn_dir;
    int walk_dir;
    int rot_angle;
    int move_speed;
    int rot_speed;

    
} t_player;

typedef struct s_map
{
    int rows;
    int cols;
    char **map;
} t_map;

typedef struct s_mlx
{
    void *mlx;
    void *win;
    void *img;
    char *addr;
    int bpp;
    int line_length;
    int endian;
} t_mlx;

typedef struct s_data
{
    t_player *player;
    t_map *map;
    t_mlx *mlx;

} t_data;


#endif