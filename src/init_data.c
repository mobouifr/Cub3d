#include "cub3d.h"

void	*ft_memset(void *buffer, int value, size_t len)
{
	unsigned char	*c;
	size_t			i;

	c = (unsigned char *)buffer;
	i = 0;
	while (i < len)
	{
		c[i] = (unsigned char)value;
		i++;
	}
	return (buffer);
}

void initialize_data(t_data *data)
{
    ft_memset(&data->gc, 0, sizeof(data->gc));
    data->mlx = ft_gc_malloc(&data->gc, sizeof(t_mlx));
    // data->mlx = malloc(sizeof(t_mlx));
    data->player = ft_gc_malloc(&data->gc,sizeof(t_player));
    // data->player = malloc(sizeof(t_player));
    data->map = ft_gc_malloc(&data->gc,sizeof(t_map));
    // data->map = malloc(sizeof(t_map));
    data->colors = ft_gc_malloc(&data->gc,sizeof(t_colors));
    // data->colors = malloc(sizeof(t_colors));
        
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
