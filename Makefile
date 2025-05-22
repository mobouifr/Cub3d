NAME = cub3D

SRC = src/main.c\
		src/2d_map.c\
		src/init_data.c\
		src/handle_input.c\
		src/player_movement.c\
		src/redering.c\
		src/mlx.c\
		src/parser/parse.c\
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

CC = cc $(INC)
CFLAGS = -Werror -Wextra -Wall -g3 -fsanitize=address
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
  