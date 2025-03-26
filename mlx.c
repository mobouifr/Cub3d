#include "cub3d.h"

void mlx_start(t_data *data)
{
    // Dynamically calculate the screen dimensions
    // int screen_width = data->map->cols * TILE_SIZE;
    // int screen_height = data->map->rows * TILE_SIZE;

    // data->mlx->mlx = mlx_init();
    // data->mlx->win = mlx_new_window(data->mlx->mlx, screen_width, screen_height, "cub3d");
    // data->mlx->img = mlx_new_image(data->mlx->mlx, screen_width, screen_height);
    // data->mlx->addr = mlx_get_data_addr(data->mlx->img, &data->mlx->bpp, &data->mlx->line_length, &data->mlx->endian);
    data->mlx->win_width = 800; // Set the window width
    data->mlx->win_height = 600; // Set the window height
    data->mlx->mlx = mlx_init();
    data->mlx->win = mlx_new_window(data->mlx->mlx, data->mlx->win_width, data->mlx->win_height, "cub3D");
    data->mlx->img = mlx_new_image(data->mlx->mlx, data->mlx->win_width, data->mlx->win_height);
    data->mlx->addr = mlx_get_data_addr(data->mlx->img, &data->mlx->bpp, &data->mlx->line_length, &data->mlx->endian);
}

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    printf("Exit Game!\n");
    exit(0);
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char *dst;

    dst = data->mlx->addr + (y * data->mlx->line_length + x * (data->mlx->bpp / 8));
    *(unsigned int*) dst = color;
}