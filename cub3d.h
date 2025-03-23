#ifndef CUB3D_H
# define CUB3D_H

#define MAP_ROWS 7
#define MAP_COLS 23
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
#define TILE_SIZE_W (SCREEN_WIDTH / MAP_COLS)  // Tile size based on width
#define TILE_SIZE_H (SCREEN_HEIGHT / MAP_ROWS) // Tile size based on height
#define TILE_SIZE (TILE_SIZE_W < TILE_SIZE_H ? TILE_SIZE_W : TILE_SIZE_H)

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