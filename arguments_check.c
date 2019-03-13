/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 19:40:01 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/13 16:06:00 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

void arguments_check(int argc, char **argv)
{
    if (argc < 2)
    {
        ft_putendl("No arguments, please choose Fractals:");
        ft_putendl("mandelbrot, julia, burning_ship, spider or mishaniabrot");
        exit(-2);
    }
    else if (argc > 6)
    {
        ft_putendl("Too many arguments!");
        exit(-2);
    }
    else
    {
        is_names_correct(argc, argv);
    }

}

int is_names_correct(int argc, char **argv)
{
    int i;

    i = 1;
    while (i < argc)
    {
        if ((ft_strcmp(argv[i], "mandelbrot") != 0) && (ft_strcmp(argv[i], "julia") != 0) &&
         (ft_strcmp(argv[i], "burning_ship") != 0) && (ft_strcmp(argv[i], "spider") != 0) &&
         (ft_strcmp(argv[i], "mishaniabrot") != 0))
         {
            ft_putendl("Wrong fractal name in parameters, please choose Fractals:");
            ft_putendl("mandelbrot, julia, burning_ship, spider or mishaniabrot");
            exit(-2);
         }
         i++;
    }
}
