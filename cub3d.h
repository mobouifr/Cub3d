#ifndef CUB3D_H
# define CUB3D_H

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
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
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
} t_player;

typedef struct s_map
{
    int width;
    int height;
    char **map;
} t_map;

typedef struct s_mlx
{
    void *mlx;
    void *win;
    void *img;
} t_mlx;

typedef struct s_data
{
    t_player *player;
    t_map *map;
    t_mlx *mlx;
    bool keys[256];
} t_data;

void init_data(t_data *data, const char *filename);
void start_game(t_data *data);
bool can_move_to(t_map *map, int x, int y);
double get_time();
void main_loop(t_data *data);

#endif