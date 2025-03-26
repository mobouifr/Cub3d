NAME = cub3D

SRC = main.c\
		get_next_line/get_next_line.c\
		get_next_line/get_next_line_utils.c\
		map.c\
		mlx.c\

CC = cc -g3 -fsanitize=address
CFLAGS = #-Werror -Wextra -Wall

OBJ = $(SRC:.c=.o)

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
