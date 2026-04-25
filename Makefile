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

# Detect OS for miniLibX
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	MLX_DIR = minilibx-linux
	MLX_LIB = -lmlx_Linux
	LIBS = -L$(MLX_DIR) $(MLX_LIB) -lXext -lX11 -lm -lz
	INC_MLX = -I$(MLX_DIR)
	MLX_TARGET = $(MLX_DIR)/libmlx_Linux.a
else
	ifeq ($(METAL), 1)
		MLX_DIR = minilibx_macos_metal/minilibx_mms_20200219
		LIBS = -L$(MLX_DIR) -lmlx -framework Metal -framework AppKit
		INC_MLX = -I$(MLX_DIR)
		MLX_TARGET = $(MLX_DIR)/libmlx.dylib
	else
		MLX_DIR = minilibx_macos_opengl/minilibx_opengl_20191021
		LIBS = -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
		INC_MLX = -I$(MLX_DIR)
		MLX_TARGET = $(MLX_DIR)/libmlx.a
	endif
endif

all: $(MLX_TARGET) $(NAME)

$(MLX_TARGET):
	$(MAKE) -C $(MLX_DIR)
	@if [ "$(METAL)" = "1" ] && [ "$(UNAME)" != "Linux" ]; then cp $(MLX_DIR)/libmlx.dylib libmlx.dylib; fi

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC_MLX) -O3 -c $< -o $@

clean:
	rm -rf $(OBJ)
	@if [ -d "minilibx-linux" ]; then $(MAKE) -C minilibx-linux clean || true; fi
	@if [ -d "minilibx_macos_opengl/minilibx_opengl_20191021" ]; then $(MAKE) -C minilibx_macos_opengl/minilibx_opengl_20191021 clean || true; fi
	@if [ -d "minilibx_macos_metal/minilibx_mms_20200219" ]; then $(MAKE) -C minilibx_macos_metal/minilibx_mms_20200219 clean || true; fi

fclean: clean
	rm -rf $(NAME)
	rm -rf libmlx.dylib

re: fclean all

.SECONDARY: $(OBJ)
  