/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:28:04 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/13 20:02:46 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

static void	names_init_2(t_fractal *fractal)
{
	if (fractal->fractal_type == ship)
	{
		ft_strcpy(fractal->file_name, "kernels/ship_kernel.cl\0");
		ft_strcpy(fractal->win_name, "Burning Ship\0");
		ft_strcpy(fractal->kernel_name, "ship\0");
	}
	else if (fractal->fractal_type == spider)
	{
		ft_strcpy(fractal->file_name, "kernels/spider_kernel.cl\0");
		ft_strcpy(fractal->win_name, "Spider\0");
		ft_strcpy(fractal->kernel_name, "spider\0");
	}
}

void		names_init(t_fractal *fractal)
{
	fractal->file_name = (char *)malloc(40);
	fractal->win_name = (char *)malloc(40);
	fractal->kernel_name = (char *)malloc(40);
	if (fractal->fractal_type == mandelbrot)
	{
		ft_strcpy(fractal->file_name, "kernels/mandelbrot_kernel.cl\0");
		ft_strcpy(fractal->win_name, "Mandelbrot\0");
		ft_strcpy(fractal->kernel_name, "mandelbrot\0");
	}
	else if (fractal->fractal_type == julia)
	{
		ft_strcpy(fractal->file_name, "kernels/julia_kernel.cl\0");
		ft_strcpy(fractal->win_name, "Julia\0");
		ft_strcpy(fractal->kernel_name, "julia\0");
	}
	else if (fractal->fractal_type == mishaniabrot)
	{
		ft_strcpy(fractal->file_name, "kernels/mishania_kernel.cl\0");
		ft_strcpy(fractal->win_name, "Mishaniabrot\0");
		ft_strcpy(fractal->kernel_name, "mishania\0");
	}
	else
	{
		names_init_2(fractal);
	}
}
