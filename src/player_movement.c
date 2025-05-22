#include  "cub3d.h"

void move_player(t_data *data)
{
    double move_step = data->player->walk_dir * data->player->move_speed;
    double cos_angle = cos(data->player->rot_angle);
    double sin_angle = sin(data->player->rot_angle);

    double new_x = data->player->player_x + cos_angle * move_step;
    double new_y = data->player->player_y + sin_angle * move_step;

    if (is_walkable(data, new_x, data->player->player_y))
        data->player->player_x = new_x;
    if (is_walkable(data, data->player->player_x, new_y))
    data->player->player_y = new_y;
}

void strafe_player(t_data *data, int direction)
{
    double move_step = direction * data->player->move_speed;
    double cos_angle = cos(data->player->rot_angle + M_PI_2);
    double sin_angle = sin(data->player->rot_angle + M_PI_2);
    double new_x = data->player->player_x + cos_angle * move_step;
    double new_y = data->player->player_y + sin_angle * move_step;

    if (is_walkable(data, new_x, data->player->player_y))
        data->player->player_x = new_x;
    if (is_walkable(data, data->player->player_x, new_y))
    data->player->player_y = new_y;
}

void rotate_player(t_data *data, int direction)
{
    data->player->rot_angle += (direction * data->player->rot_speed);
    if (data->player->rot_angle < 0)
        data->player->rot_angle += 2 * M_PI;
    if (data->player->rot_angle > 2 * M_PI)
        data->player->rot_angle -= 2 * M_PI;
}

int is_walkable(t_data *data, double new_x, double new_y)
{
    // Define the player's collision buffer size (half the player's size)
    double buffer = 0.2; // Adjust this value as needed

    // Check all four corners of the player's collision box
    int top_left_x = (int)(new_x - buffer);
    int top_left_y = (int)(new_y - buffer);
    int top_right_x = (int)(new_x + buffer);
    int top_right_y = (int)(new_y - buffer);
    int bottom_left_x = (int)(new_x - buffer);
    int bottom_left_y = (int)(new_y + buffer);
    int bottom_right_x = (int)(new_x + buffer);
    int bottom_right_y = (int)(new_y + buffer);

    // Check if any corner is in a wall
    if (top_left_x < 0 || top_left_y < 0 || top_right_x >= data->map->cols || bottom_right_y >= data->map->rows)
        return 0; // Out of bounds

    if (data->map->map[top_left_y][top_left_x] == '1' ||
        data->map->map[top_right_y][top_right_x] == '1' ||
        data->map->map[bottom_left_y][bottom_left_x] == '1' ||
        data->map->map[bottom_right_y][bottom_right_x] == '1')
        return 0; // Collision with a wall

    return 1; // No collision
}
