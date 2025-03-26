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
    double rot_angle; 
    double move_speed;
    double rot_speed;

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
    int win_width;
    int win_height;
    int endian;
} t_mlx;

typedef struct s_data
{
    t_player *player;
    t_map *map;
    t_mlx *mlx;

} t_data;

void draw_map(t_data *data, char **map, int rows, int cols);
char **read_map_from_file(char *filename, int *rows, int *cols);
void mlx_start(t_data *data);
int close_window(t_data *data);
void my_mlx_pixel_put(t_data *data, int x, int y, int color);
#endif