/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_source_str_gen.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 22:00:54 by chorange          #+#    #+#             */
/*   Updated: 2019/03/14 13:38:33 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

static	void	find_window_size_define(char **define)
{
	int	fd;

	if (!(fd = open("fractals.h", O_RDONLY)))
	{
		ft_putendl("Cannot open header files");
		exit(1);
	}
	while (get_next_line(fd, define))
	{
		if (ft_strstr(*define, "WIDTH"))
			break ;
		free(*define);
	}
	close(fd);
}

static	void	kernel_reading(char *file_name, char **dst)
{
	int	fd;

	if (!(*dst = (char *)malloc(4096)))
	{
		free(*dst);
		ft_putendl("Cannot open kernel files");
		exit(1);
	}
	if (!(fd = open(file_name, O_RDONLY)))
	{
		free(*dst);
		ft_putendl("Cannot open kernel files");
		exit(1);
	}
	(*dst)[read(fd, *dst, 4096)] = 0;
	close(fd);
}

int				cl_source_str_gen(char *file_name, char **source_str)
{
	char	*define;
	char	*main_kernel;
	char	*kernel;

	define = NULL;
	main_kernel = NULL;
	kernel = NULL;
	*source_str = (char *)malloc(4096);
	find_window_size_define(&define);
	ft_strcpy(*source_str, define);
	ft_strcat(*source_str, "\n");
	kernel_reading("kernels/main_kernel.cl", &main_kernel);
	kernel_reading(file_name, &kernel);
	ft_strcat(*source_str, main_kernel);
	ft_strcat(*source_str, kernel);
	free(define);
	free(main_kernel);
	free(kernel);
	return (ft_strlen(*source_str));
}
