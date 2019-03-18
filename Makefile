# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cocummin <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/18 18:53:25 by chorange          #+#    #+#              #
#    Updated: 2019/03/18 11:35:21 by cocummin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractol

FLAGS = ##-Wextra -Wall -Werror

OBJECTS = main.o draw_fractal.o fractal_init.o names_init.o \
			keys_handling.o arguments_check.o transform_init.o\
			cl_source_str_gen.o get_next_line/get_next_line.o\
			transform_change.o flourish_init.o

LIBS = -L./minilibx -lmlx -L./libft -lft -lm -framework OpenGL -framework AppKit -framework OpenCL

INCLUDES = -I./libft/libft.h -I./minilibx/mlx.h

MLXPATH = ./minilibx
FTPATH = ./libft

all: $(NAME)



$(NAME): $(OBJECTS)
	make -C $(FTPATH)
	make -C $(MLXPATH)

	gcc $(FLAGS) $(OBJECTS) $(INCLUDES) $(LIBS) -o $(NAME)

$(OBJECTS): %.o: %.c
	gcc $(FLAGS) -c  $< -o $@

clean:
	make -C $(FTPATH) clean
	make -C $(MLXPATH) clean
	/bin/rm -f $(OBJECTS)

fclean: clean
	make -C $(FTPATH) fclean
	make -C $(MLXPATH) clean
	/bin/rm -f $(NAME)

re: fclean all
