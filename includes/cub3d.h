/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobouifr <mobouifr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:49:22 by mamir             #+#    #+#             */
/*   Updated: 2025/05/25 21:54:30 by mobouifr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define TILE_SIZE 32
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846   /* pi */
#  define M_PI_2 1.57079632679489661923 /* pi/2 */
# endif

// parsing.h -start-

typedef enum e_state
{
	PARSE_TEXTURES_AND_COLORS_STATE,
	PARSE_MAP_STATE
}						t_state;

typedef struct s_dda
{
	double				dir_x;
	double				dir_y;
	int					map_x;
	int					map_y;
	double				delta_x;
	double				delta_y;
	int					step_x;
	int					step_y;
	double				side_x;
	double				side_y;
	int					side_hit;
}						t_dda;

typedef struct s_game
{
	char				*mapfile_path;
	char				*no_path;
	char				*so_path;
	char				*we_path;
	char				*ea_path;
	int					floor_color[3];
	int					ceiling_color[3];
	int					floor_color_hex;
	int					ceiling_color_hex;
	char				**map;
	int					map_width;
	int					map_height;
	double				player_x;
	double				player_y;
	char				player_dir;
	int					has_player_dir;
	int					has_no;
	int					has_so;
	int					has_we;
	int					has_ea;
	int					has_floor;
	int					has_ceiling;
	t_state				state;
}						t_game;

typedef struct s_texture
{
	void				*img;
	char				*addr;
	int					bpp;
	int					line_len;
	int					endian;
	int					width;
	int					height;
	int					img_successfully_loaded;
}						t_texture;

// -end-

/*Garbage collector*/
typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
}						t_gc;

typedef struct s_draw_vars
{
	int					line_height;
	int					unclipped_draw_start;
	int					draw_start;
	int					draw_end;
	int					tex_x;
}						t_draw_vars;

typedef struct s_colors
{
	int					floor_color_hex;
	int					ceiling_color_hex;
}						t_colors;

typedef struct s_player
{
	double				player_x;
	double				player_y;
	int					radius;
	int					turn_dir;
	int					strafe_dir;
	int					walk_dir;
	double				rot_angle;
	double				move_speed;
	double				rot_speed;

}						t_player;

typedef struct s_map
{
	int					rows;
	int					cols;
	char				**map;
}						t_map;

typedef struct s_mlx
{
	void				*mlx;
	void				*win;
	void				*img;
	char				*addr;
	int					bpp;
	int					line_length;
	int					win_width;
	int					win_height;
	int					endian;
}						t_mlx;

typedef struct s_data
{
	t_player			*player;
	t_map				*map;
	t_mlx				*mlx;
	t_texture			*textures[4];
	t_colors			*colors;
	t_gc				gc;
}						t_data;

/*parser*/
t_game					*parser(int argc, char **argv, t_data *data);
void					vars_init(t_game *gamevar);
int						line_is_empty(char *str);
int						ft_strlen_v2(char *str);
void					check_file_exists(char *filepath, t_data *data);
void					valid_extention_check(t_game *game, t_data *data);
int						parse_rgb_color(int *color_code, char *str,
							t_data *data);
int						has_directions(t_game *gamevar);
int						map_row_is_valid(char *str, t_game *gamevar,
							t_data *data);
void					handle_texture_and_color_input(char *line,
							t_game *gamevar, t_data *data);
void					handle_map_row_input(int fd, char *line,
							t_game *gamevar, t_data *data);
void					parse_line(int fd, t_game *gamevar, t_data *data);

void					mlx_start(t_data *data);
int						close_window(t_data *data);
void					my_mlx_pixel_put(t_data *data, int x, int y, int color);
char					*get_next_line(int fd, t_data *data);
void					print_gamevar(const t_game *g);
void					allocate_map(t_game *gamevar, t_data *data);
void					skip_config_lines(int fd, t_data *data);
void					validate_map_borders(t_game *gamevar, t_data *data);
void					exit_error(char *str, t_data *data);
void					fill_map(t_game *gamevar, t_data *data);

/*Garbagr Collector*/
void					*ft_gc_malloc(t_gc *gc, size_t size);
void					ft_gc_free_all(t_gc *gc);
void					*ft_gc_malloc(t_gc *gc, size_t size);
void					ft_gc_free_all(t_gc *gc);
/*init_data*/
void					initialize_data(t_data *data);
void					init_player_data(t_data *data);
void					initialize_data(t_data *data);
void					init_player_data(t_data *data);
/*Handle input*/
int						handle_keypress(int keycode, t_data *data);
int						key_released(int keycode, t_data *data);
int						handle_keypress(int keycode, t_data *data);
int						key_released(int keycode, t_data *data);
/*player_movement*/
void					move_player(t_data *data);
void					strafe_player(t_data *data, int direction);
void					rotate_player(t_data *data, int direction);
int						is_walkable(t_data *data, double new_x, double new_y);
void					move_player(t_data *data);
void					strafe_player(t_data *data, int direction);
void					rotate_player(t_data *data, int direction);
int						is_walkable(t_data *data, double new_x, double new_y);
/*Rendering*/
void					draw(t_data *data);
void					clear_image(t_data *data);
void					cast_rays(t_data *data);
double					cast_ray(t_data *data, double ray_angle, int *side_hit);
/*Ray functions & helpers*/
void					init_draw_vars(t_draw_vars *vars, int win_h,
							double dist);
void					init_ray_vars(double ray_angle, t_data *data,
							t_dda *dda);
void					init_step_dist_x(t_data *data, t_dda *dda);
void					init_step_dist_y(t_data *data, t_dda *dda);
int						dda_loop(t_data *data, t_dda *dda);
double					get_wall_x(t_data *data, double dist, double ray_angle,
							int side);
void					draw_wall_slice(t_data *data, t_texture *tex, int i,
							t_draw_vars *vars);
int						get_tex_index_for_column(double ray_angle,
							int side_hit);
t_texture				*get_tex_for_column(t_data *data, int tex_index);
int						get_tex_x_for_column(t_texture *tex, double wall_x,
							int side_hit, double ray_angle);
/**2d draw
void	draw_map_2d(t_data *data, char **map, int rows,
							int cols);
void	draw_map_2d(t_data *data, char **map, int rows,
							int cols);
void	draw_line(t_data *data, int x1, int y1, int x2, int y2,
							int color);
void					draw_player_facing_line(t_data *data);
void					draw_player(t_data *data);
void	draw_map_2d(t_data *data, char **map, int rows,
							int cols);
void	draw_map_2d(t_data *data, char **map, int rows,
							int cols);
void	draw_line(t_data *data, int x1, int y1, int x2, int y2,
							int color);
void					draw_player_facing_line(t_data *data);
void					draw_player(t_data *data);
**/
unsigned int			get_tex_color(t_texture *tex, int x, int y);
int						get_texture_index(double ray_dir_x, double ray_dir_y,
							int side_hit);
unsigned int			get_tex_color(t_texture *tex, int x, int y);
int						get_texture_index(double ray_dir_x, double ray_dir_y,
							int side_hit);

#endif