# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/04 22:45:06 by obamzuro          #+#    #+#              #
#    Updated: 2018/10/01 18:35:30 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = opencl

SRCNAME = 	main.c

FLAGS = -g #-Wall -Wextra -Werror

SRC = $(addprefix source/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

HDRDIR = libft/include\
		 fprintf/include

#HDR = include/fractol.h

all: lib $(NAME)

$(NAME): $(OBJ) libft/libft.a
	gcc -o $(NAME) $(FLAGS) $(addprefix -I, $(HDRDIR)) -L fprintf -lftprintf -L libft -lft $(SRC) -lmlx -framework OpenGL -framework AppKit -framework OpenCL

%.o: %.c #$(HDR)
	gcc $(FLAGS) $(addprefix -I, $(HDRDIR)) -c $< -o $@

lib:
	make -C libft
	make -C fprintf

clean:
	make -C libft clean
	rm -rf $(OBJ)

fclean: clean
	make -C libft fclean
	rm -rf $(NAME)

re: fclean all
