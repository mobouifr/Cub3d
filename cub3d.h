#ifndef CUB3D_H
# define CUB3D_H

# define SCREEN_WIDTH 600
# define SCREEN_HEIGHT 600
# define FOV 60
# define TILE_SIZE 60
# define ROTATE_SPEED 0.045
# define MOVE_SPEED 4
# define TILE_SIZE 32

# include <stdio.h>
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
    int player_x;
    int player_y;
    double player_angle;
    float fov_rd;
    int rot;
    int l_r;
    int u_d;
    double player_x;
    double player_y;
    int radius;
    int turn_dir;
    int walk_dir;
    int rot_angle;
    int move_speed;
    int rot_speed;

    
} t_player;

typedef struct s_ray
{
    double ray_angle;
    double distance;
    int flag;
} t_ray;
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
typedef struct s_map
{
    char **map;

    int fd;
    int line_count;
    char *path;
    char **file;
    int index;
    int width;
    int height;
} t_map;
typedef struct s_data
{
    t_map *map;
    int win_width;
    int win_height;
    t_mlx *mlx;
    t_player *player;
    t_ray *ray;

} t_data;


#endif