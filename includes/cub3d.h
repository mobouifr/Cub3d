#ifndef CUB3D_H
# define CUB3D_H

# define TILE_SIZE 32

# include "libft.h"
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
	INITIAL,
	PARSE_DIRECTION_STATE,
	PARSE_MAP_STATE
}				t_state;

typedef struct s_game
{
	char		*mapfile_path;
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	int			floor_color[3];
	int			ceiling_color[3];
	int			floor_color_hex;
	int			ceiling_color_hex;
	char		**map;
	int			map_width;
	int			map_height;
	int			player_x;
	int			player_y;
	char		player_dir;
	int			has_player_dir;
	int			has_no;
	int			has_so;
	int			has_we;
	int			has_ea;
	int			has_floor;
	int			has_ceiling;
	t_state		state;
}				t_game;

typedef struct s_texture
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			width;
	int			height;
}				t_texture;

// -end-


typedef struct s_colors
{
	int			floor_color_hex;
	int			ceiling_color_hex;
}				t_colors;
typedef struct s_player
{
	double		player_x;
	double		player_y;
	int			radius;
	int			turn_dir;
	int			walk_dir;
	double		rot_angle;
	double		move_speed;
	double		rot_speed;

}				t_player;

typedef struct s_map
{
	int			rows;
	int			cols;
	char		**map;
}				t_map;

typedef struct s_mlx
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_length;
	int			win_width;
	int			win_height;
	int			endian;
}				t_mlx;

typedef struct s_data
{
	t_player	*player;
	t_map		*map;
	t_mlx		*mlx;
	t_texture	*textures[4];
	t_colors	*colors;
}				t_data;

void			draw_map(t_data *data, char **map, int rows, int cols);
void			mlx_start(t_data *data);
int				close_window(t_data *data);
void			my_mlx_pixel_put(t_data *data, int x, int y, int color);
char			*get_next_line(int fd);
t_game			*parser(int argc, char **argv);
void			print_gamevar(const t_game *g);

#endif