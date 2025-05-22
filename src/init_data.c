#include "cub3d.h"

void initialize_data(t_data *data)
{
    data->mlx = malloc(sizeof(t_mlx));
    data->player = malloc(sizeof(t_player));
    data->map = malloc(sizeof(t_map));
    data->colors = malloc(sizeof(t_colors));
        
    if (!data->mlx || !data->player || !data->map)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    init_player_data(data);

}

void init_player_data(t_data *data)
{
    data->player->turn_dir = 0;
    data->player->strafe_dir = 0;
    data->player->walk_dir = 0;
    data->player->move_speed = 0.02;
    data->player->rot_speed = 0.5 * (M_PI / 180);
}
