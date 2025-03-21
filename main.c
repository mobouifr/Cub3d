#include "cub3d.h"


void init_data(t_data *data)
{
    data->player = (t_player *)malloc(sizeof(t_player));
    data->ray = (t_ray *)malloc(sizeof(t_ray));
    data->mlx = (t_mlx *)malloc(sizeof(t_mlx));
    data->map = (t_map *)malloc(sizeof(t_map));

    data->mlx->mlx = NULL;
    data->mlx->win = NULL;
    data->mlx->img = NULL;
    data->win_height = SCREEN_HEIGHT;
    data->win_width =  SCREEN_WIDTH;
}

void init_mlx(t_data *data)
{
    data->mlx->mlx = mlx_init();
    if (!data->mlx->mlx)
    {
        printf("Error\nmlx_init failed\n");
        exit(1);
    }
    data->mlx->win = mlx_new_window(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3d");
    if (!data->mlx->win)
    {
        printf("Error\nmlx_new_window failed\n");
        exit(1);
    }
    data->mlx->img = mlx_new_image(data->mlx->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!data->mlx->img)
    {
        printf("Error\nmlx_new_image failed\n");
        exit(1);
    }
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);
    return;
}

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx->mlx, data->mlx->win);
    printf("Exited Game\n");
    exit(0);
    return (0);
}

int key_press(int keycode, t_data *data)
{
    printf("Key pressed: %d\n", keycode);
    if (keycode == 65307)
    {
        printf("Exited Game!\n");
        close_window(data);
    }
    return (0);
}

void start_game(t_data *data)
{
    init_mlx(data);
    mlx_hook(data->mlx->win, 2, 1L<<0, key_press, data); // Listen for key press events
    mlx_hook(data->mlx->win, 17, 0, close_window, data); // Listen for window close events
    mlx_loop(data->mlx->mlx);
    return;
}

int main()
{
    t_data data;
    init_data(&data);
    start_game(&data);
    return (0);
}
