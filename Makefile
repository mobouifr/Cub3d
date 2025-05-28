NAME = cub3D

SRC = src/main.c\
		src/ft_malloc.c\
		src/2d_map.c\
		src/init_data.c\
		src/handle_input.c\
		src/player_movement.c\
		src/rendering.c\
		src/rays.c\
		src/ray_helper_1.c\
		src/ray_helper_2.c\
		src/mlx.c\
		src/parser/parse.c\
		src/parser/vars_init.c\
		src/parser/utils.c\
		src/parser/parse_checks.c\
		src/parser/map_row_checks.c\
		src/parser/map_and_config_parser.c\
		src/parser/line_parser.c\
		src/parser/fill_map.c\
		src/parser/fill_map_utils.c\
		src/textures/texture_utils.c\
		utils/get_next_line/get_next_line.c\
		utils/libft/ft_atoi.c \
		utils/libft/ft_isspace.c \
		utils/libft/ft_strchr.c \
		utils/libft/ft_strdup.c \
		utils/libft/ft_strlen.c \
		utils/libft/ft_strnstr.c \
		utils/libft/ft_strrchr.c \
		utils/libft/ft_strjoin.c \
		utils/libft/ft_split.c \
		utils/libft/ft_strcmp.c \
		utils/libft/ft_strtrim.c \
		utils/debug/parsing/print_gamevar.c

CC = cc $(INC)
CFLAGS = -Werror -Wextra -Wall #-g3 -fsanitize=address
OBJ = $(SRC:.c=.o)
INC = -I./includes/

# Directories for miniLibX
MLX_DIR = mlx_linux
MLX_LIB = -lmlx_Linux

# Libraries
LIBS = -L$(MLX_DIR) $(MLX_LIB) -L/usr/lib -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I/usr/include -I$(MLX_DIR) -O3 -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.SECONDARY: $(OBJ)
  