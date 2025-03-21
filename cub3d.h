#ifndef CUB3D_H
# define CUB3D_H

# define SCREEN_WIDTH 1900
# define SCREEN_HEIGHT 1000
# define FOV 60
# define TILE_SIZE 60
# define ROTATE_SPEED 0.045
# define MOVE_SPEED 4
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <limits.h>
# include <mlx.h>

typedef struct s_player
{
    int player_x;
    int player_y;
    double player_angle;
    float fov_rd;
    int rot;
    int l_r;
    int u_d;
} t_player;

typedef struct s_ray
{
    double ray_angle;
    double distance;
    int flag;
} t_ray;

typedef struct s_mlx
{
    void *mlx;
    void *win;
    void *img;
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