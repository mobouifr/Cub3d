#include "cub3d.h"
#include <mlx.h>
#include <math.h>
#include <sys/time.h>

void init_mlx(void **mlx, void **mlx_win)
{
    *mlx = mlx_init();
    *mlx_win = mlx_new_window(*mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "raycaster!");
    mlx_loop(*mlx);
}

double get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec + (time.tv_usec / 1000000.0));
}

void main_loop(t_data *data)
{
    static double oldTime = 0;
    double time = get_time();
    double frameTime = time - oldTime;
    oldTime = time;

    // Speed modifiers
    double moveSpeed = frameTime * 5.0; // The constant value is in squares/second
    double rotSpeed = frameTime * 3.0; // The constant value is in radians/second

    // Event handling
    if (data->keys['w']) // Move forward
    {
        if (can_move_to(data->map, (int)(data->player->player_x + data->player->dir_x * moveSpeed), (int)(data->player->player_y)))
            data->player->player_x += data->player->dir_x * moveSpeed;
        if (can_move_to(data->map, (int)(data->player->player_x), (int)(data->player->player_y + data->player->dir_y * moveSpeed)))
            data->player->player_y += data->player->dir_y * moveSpeed;
    }
    if (data->keys['s']) // Move backward
    {
        if (can_move_to(data->map, (int)(data->player->player_x - data->player->dir_x * moveSpeed), (int)(data->player->player_y)))
            data->player->player_x -= data->player->dir_x * moveSpeed;
        if (can_move_to(data->map, (int)(data->player->player_x), (int)(data->player->player_y - data->player->dir_y * moveSpeed)))
            data->player->player_y -= data->player->dir_y * moveSpeed;
    }
    if (data->keys['a']) // Rotate left
    {
        double oldDirX = data->player->dir_x;
        data->player->dir_x = data->player->dir_x * cos(rotSpeed) - data->player->dir_y * sin(rotSpeed);
        data->player->dir_y = oldDirX * sin(rotSpeed) + data->player->dir_y * cos(rotSpeed);
        double oldPlaneX = data->player->plane_x;
        data->player->plane_x = data->player->plane_x * cos(rotSpeed) - data->player->plane_y * sin(rotSpeed);
        data->player->plane_y = oldPlaneX * sin(rotSpeed) + data->player->plane_y * cos(rotSpeed);
    }
    if (data->keys['d']) // Rotate right
    {
        double oldDirX = data->player->dir_x;
        data->player->dir_x = data->player->dir_x * cos(-rotSpeed) - data->player->dir_y * sin(-rotSpeed);
        data->player->dir_y = oldDirX * sin(-rotSpeed) + data->player->dir_y * cos(-rotSpeed);
        double oldPlaneX = data->player->plane_x;
        data->player->plane_x = data->player->plane_x * cos(-rotSpeed) - data->player->plane_y * sin(-rotSpeed);
        data->player->plane_y = oldPlaneX * sin(-rotSpeed) + data->player->plane_y * cos(-rotSpeed);
    }

    // Raycasting and rendering
    // Implement raycasting and rendering logic here

    // Update the window
    mlx_clear_window(data->mlx->mlx, data->mlx->win);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
    mlx_do_sync(data->mlx->mlx);
}

bool can_move_to(t_map *map, int x, int y)
{
    if (x < 0 || x >= map->width || y < 0 || y >= map->height)
        return false;
    return map->map[y][x] != '1';
}

int key_press(int keycode, t_data *data)
{
    if (keycode < 256)
        data->keys[keycode] = true;
    return 0;
}

int key_release(int keycode, t_data *data)
{
    if (keycode < 256)
        data->keys[keycode] = false;
    return 0;
}

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    printf("Exited Game\n");
    exit(0);
    return 0;
}

void init_data(t_data *data, const char *filename)
{
    data->map = (t_map *)malloc(sizeof(t_map));
    data->player = (t_player *)malloc(sizeof(t_player));
    data->mlx = (t_mlx *)malloc(sizeof(t_mlx));

    // Initialize player position and direction
    data->player->player_x = 22;
    data->player->player_y = 12;
    data->player->dir_x = -1;
    data->player->dir_y = 0;
    data->player->plane_x = 0;
    data->player->plane_y = 0.66;

    // Initialize map
    data->map->width = 24;
    data->map->height = 24;
    data->map->map = (char **)malloc(sizeof(char *) * data->map->height);
    for (int i = 0; i < data->map->height; i++)
    {
        data->map->map[i] = (char *)malloc(sizeof(char) * data->map->width);
        for (int j = 0; j < data->map->width; j++)
        {
            if (i == 0 || i == data->map->height - 1 || j == 0 || j == data->map->width - 1)
                data->map->map[i][j] = '1';
            else
                data->map->map[i][j] = '0';
        }
    }

    // Initialize keys
    for (int i = 0; i < 256; i++)
        data->keys[i] = false;

    // Initialize mlx
    data->mlx->mlx = mlx_init();
    data->mlx->win = mlx_new_window(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
    data->mlx->img = mlx_new_image(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void start_game(t_data *data)
{
    mlx_loop_hook(data->mlx->mlx, (int (*)())main_loop, data);
    mlx_hook(data->mlx->win, 2, 1L<<0, key_press, data); // Listen for key press events
    mlx_hook(data->mlx->win, 3, 1L<<1, key_release, data); // Listen for key release events
    mlx_hook(data->mlx->win, 17, 0, close_window, data); // Listen for window close events
    mlx_loop(data->mlx->mlx);
}

int main(int argc, char **argv)
{
    t_data data;
    init_data(&data, argv[1]);
    start_game(&data);
    return (0);
}