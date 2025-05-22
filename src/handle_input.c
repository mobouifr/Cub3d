#include "cub3d.h"

int handle_keypress(int keycode, t_data *data)
{
    if (keycode == 'w') // Move forward
        data->player->walk_dir = 1;
    else if (keycode == 's') // Move backward
        data->player->walk_dir = -1;
    else if (keycode == 'a') // Rotate left (A key)
        data->player->strafe_dir = -1; // Counter-clockwise
    else if (keycode == 'd') // Rotate right (D key)
        data->player->strafe_dir = 1;
    else if (keycode == 65361) // Rotate left (Left Arrow)
        data->player->turn_dir = -1; // Counter-clockwise
    else if (keycode == 65363) // Rotate right (Right Arrow)
        data->player->turn_dir = 1; // Clockwise
    else if (keycode == 65307) // ESC key (exit game)
        close_window(data); // Exit game

    return 0;
}

int key_released(int keycode, t_data *data)
{
    if (keycode == 'w' || keycode == 's') // Stop forward/backward movement
        data->player->walk_dir = 0;      
    else if (keycode == 'a' || keycode == 'd') // Stop strafing
        data->player->strafe_dir = 0;
    else if (keycode == 65361 || keycode == 65363) // Stop rotation
        data->player->turn_dir = 0;
    return 0;
}
