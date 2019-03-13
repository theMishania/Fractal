/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 18:42:57 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/13 17:24:29 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

int main(int argc, char **argv)
{
    void     *mlx_ptr;

    arguments_check(argc, argv);
    mlx_ptr = mlx_init();
    initiate_fractals(mlx_ptr, argc, argv);
    mlx_loop(mlx_ptr);
    return (0);

}

void    initiate_fractals(void *mlx_ptr, int argc, char **argv)
{
    int i;
    static t_fractal fractals[6];

    i = 1;
    utils_create(mlx_ptr, argc, fractals);
    while (i < argc)
    {
        if (ft_strcmp(argv[i], "mandelbrot") == 0)
            fractals[i - 1].fractal_type = mandelbrot;
        else if (ft_strcmp(argv[i], "julia") == 0)
            fractals[i - 1].fractal_type = julia;
        else if (ft_strcmp(argv[i], "burning_ship") == 0)
            fractals[i - 1].fractal_type = ship;
        else if (ft_strcmp(argv[i], "spider") == 0)
            fractals[i - 1].fractal_type = spider;
        else if (ft_strcmp(argv[i], "mishaniabrot") == 0)
            fractals[i - 1].fractal_type = mishaniabrot;
        draw_fractal(&fractals[i - 1]);
        i++;
    }
}

void utils_create(void *mlx_ptr, int argc, t_fractal *fractals)
{
    int i;

    i = 1;
    while (i < argc)
    {
        fractals[i - 1].mlx_ptr = mlx_ptr;
        i++;
    }
}