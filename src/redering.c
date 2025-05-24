#include "cub3d.h"
static void	init_ray_vars(double ray_angle, t_data *data, double *dir_x, double *dir_y, int *map_x, int *map_y)
{
    *dir_x = cos(ray_angle);
    *dir_y = sin(ray_angle);
    *map_x = (int)data->player->player_x;
    *map_y = (int)data->player->player_y;
}

static void	init_step_dist(double dir, double ray, int map, int *step, double *side_dist, double delta_dist)
{
    if (dir < 0)
    {
        *step = -1;
        *side_dist = (ray - map) * delta_dist;
    }
    else
    {
        *step = 1;
        *side_dist = (map + 1.0 - ray) * delta_dist;
    }
}

static int	check_hit(t_data *data, int map_x, int map_y)
{
    if (map_x < 0 || map_x >= data->map->cols)
        return (1);
    if (map_y < 0 || map_y >= data->map->rows)
        return (1);
    if (data->map->map[map_y][map_x] == '1')
        return (2);
    return (0);
}


static void	draw_sky(t_data *data)
{
    int	y;
    int	x;
    int	color;

    color = data->colors->ceiling_color_hex;
    y = 0;
    while (y < data->mlx->win_height / 2)
    {
        x = 0;
        while (x < data->mlx->win_width)
        {
            my_mlx_pixel_put(data, x, y, color);
            x++;
        }
        y++;
    }
}

static void	draw_floor(t_data *data)
{
    int	y;
    int	x;
    int	color;

    color = data->colors->floor_color_hex;
    y = data->mlx->win_height / 2;
    while (y < data->mlx->win_height)
    {
        x = 0;
        while (x < data->mlx->win_width)
        {
            my_mlx_pixel_put(data, x, y, color);
            x++;
        }
        y++;
    }
}

void	clear_image(t_data *data)
{
    int	y;
    int	x;

    y = 0;
    while (y < data->mlx->win_height)
    {
        x = 0;
        while (x < data->mlx->win_width)
        {
            my_mlx_pixel_put(data, x, y, 0x000000);
            x++;
        }
        y++;
    }
}

static void	init_draw_vars(t_draw_vars *vars, int win_h, double dist)
{
    vars->line_height = (int)(win_h / dist);
    vars->unclipped_draw_start = -vars->line_height / 2 + win_h / 2;
    vars->draw_start = vars->unclipped_draw_start;
    if (vars->draw_start < 0)
        vars->draw_start = 0;
    vars->draw_end = vars->line_height / 2 + win_h / 2;
    if (vars->draw_end >= win_h)
        vars->draw_end = win_h - 1;
}

static double	get_wall_x(t_data *data, double dist, double ray_angle, int side)
{
    double	wall_x;

    if (side == 0)
        wall_x = data->player->player_y + dist * sin(ray_angle);
    else
        wall_x = data->player->player_x + dist * cos(ray_angle);
    return (wall_x - floor(wall_x));
}

void	draw_wall_slice(t_data *data, t_texture *tex, int i, t_draw_vars *vars)
{
    int	y;
    int	tex_y;
    unsigned int	color;

    y = vars->draw_start;
    while (y < vars->draw_end)
    {
        tex_y = (int)(((double)(y - vars->unclipped_draw_start)
                / vars->line_height) * tex->height);
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= tex->height)
            tex_y = tex->height - 1;
        color = get_tex_color(tex, vars->tex_x, tex_y);
        my_mlx_pixel_put(data, i, y, color);
        y++;
    }
}

static void	draw_column(t_data *data, int i, double ray_angle)
{
    int			side_hit;
    double		perp_wall_dist;
    double		corrected_dist;
    double		wall_x;
    int			tex_index;
    t_texture	*tex;
    t_draw_vars	vars;

    perp_wall_dist = cast_ray(data, ray_angle, &side_hit);
    corrected_dist = perp_wall_dist * cos(ray_angle - data->player->rot_angle);
    init_draw_vars(&vars, data->mlx->win_height, corrected_dist);
    wall_x = get_wall_x(data, perp_wall_dist, ray_angle, side_hit);
    tex_index = get_texture_index(cos(ray_angle), sin(ray_angle), side_hit);
    tex = data->textures[tex_index];
    vars.tex_x = (int)(wall_x * tex->width);
    if ((side_hit == 0 && cos(ray_angle) > 0)
        || (side_hit == 1 && sin(ray_angle) < 0))
        vars.tex_x = tex->width - vars.tex_x - 1;
    draw_wall_slice(data, tex, i, &vars);
}

void	cast_rays(t_data *data)
{
    int		i;
    double	fov;
    double	angle_step;
    double	start_angle;
    double	ray_angle;

    fov = 60 * (M_PI / 180);
    angle_step = fov / data->mlx->win_width;
    start_angle = data->player->rot_angle - (fov / 2);
    i = 0;
    while (i < data->mlx->win_width)
    {
        ray_angle = start_angle + (i * angle_step);
        draw_column(data, i, ray_angle);
        i++;
    }
}

double	cast_ray(t_data *data, double ray_angle, int *side_hit)
{
    double	dir_x, dir_y, delta_x, delta_y;
    double	side_x, side_y;
    int		map_x, map_y, step_x, step_y;
    int		hit;

    ray_angle = fmod(ray_angle, 2 * M_PI);
    if (ray_angle < 0)
        ray_angle += 2 * M_PI;
    init_cast_vars(ray_angle, data, &dir_x, &dir_y, &map_x, &map_y);
    delta_x = fabs(1 / dir_x);
    delta_y = fabs(1 / dir_y);
    init_cast_steps(dir_x, data->player->player_x, map_x, &step_x, &side_x, delta_x);
    init_cast_steps(dir_y, data->player->player_y, map_y, &step_y, &side_y, delta_y);
    hit = cast_hit_loop(data, &map_x, &map_y, &side_x, &side_y,
        delta_x, delta_y, &step_x, &step_y, side_hit);
    if (*side_hit == 0)
        return ((map_x - data->player->player_x + (1 - step_x) / 2) / dir_x);
    return ((map_y - data->player->player_y + (1 - step_y) / 2) / dir_y);
}

void	draw(t_data *data)
{
    clear_image(data);
    draw_sky(data);
    draw_floor(data);
    cast_rays(data);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win,
        data->mlx->img, 0, 0);
}
