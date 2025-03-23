#include "cub3d.h"
#include <mlx.h>
#include <math.h>
#include <sys/time.h>

void mlx_start(t_data *data)
{
    data->mlx->mlx = mlx_init();
    data->mlx->win = mlx_new_window(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
    data->mlx->img = mlx_new_image(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    data->mlx->addr = mlx_get_data_addr(data->mlx->img, &data->mlx->bpp, &data->mlx->line_length, &data->mlx->endian);
}
void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char *dst;

    dst = data->mlx->addr + (y * data->mlx->line_length + x * (data->mlx->bpp / 8));
    *(unsigned int*) dst = color; 
}

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    printf("Exit Game!\n");
    exit(0);
}

int key_press(int keycode, t_data *data)
{
    if (keycode == 65307)
        close_window(data);
    return 0;
}
void draw_map(t_data *data, char **map, int rows, int cols)
{
    int x, y, i, j;

    for (y = 0; y < rows; y++)
    {
        for (x = 0; x < cols; x++)
        {
            int color = (map[y][x] == '1') ? 0xFFFFFF : 0x000000; // White for walls, Black for empty space

            for (i = 0; i < TILE_SIZE; i++) 
            {
                for (j = 0; j < TILE_SIZE; j++) 
                {
                    my_mlx_pixel_put(data, x * TILE_SIZE + j, y * TILE_SIZE + i, color);
                }
            }
        }
    }
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
}


char *map[] = {
    "11111111111111111111111",
    "10000000001000000010001",
    "10000000000100000100001",
    "10000000000010001000001", // Fixed row (added missing '0')
    "10000000000001010000001",
    "10000000000001100000001",
    "11111111111111111111111",
    NULL
};

void start_game(t_data *data)
{
    mlx_hook(data->mlx->win, 2, 1L << 0, key_press, data);
    mlx_hook(data->mlx->win, 17, 0, close_window, data);

    draw_map(data, map, 7, 23); // Draw the map first
    mlx_loop(data->mlx->mlx);
}


void initialize_data(t_data *data)
{
    data->mlx = malloc(sizeof(t_mlx));
    if (!data->mlx)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
}
int main(int argc, char **argv)
{
    t_data data;
    initialize_data(&data);
    mlx_start(&data);
    start_game(&data);
    return (0);
}