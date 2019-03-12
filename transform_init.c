/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:39:41 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/12 14:38:06 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

void    mandelbrot_utils(t_transform *fractal_transform)
{
    fractal_transform->zoom = 1;
    fractal_transform->delta_x = 0;
    fractal_transform->delta_y = 0;
    fractal_transform->xx = 0;
    fractal_transform->yy = 0;
    fractal_transform->middle_mouse_pressed = 0;
    fractal_transform->color = 1312;

    fractal_transform->c_re = -0.7f;
    fractal_transform->c_im = 0.29015f;
    fractal_transform->right_mouse_pressed = 0;
}

void    julia_utils(t_transform *fractal_transform)
{
    fractal_transform->zoom = 1;
    fractal_transform->delta_x = 0;
    fractal_transform->delta_y = 0;
    fractal_transform->xx = 0;
    fractal_transform->yy = 0;
    fractal_transform->middle_mouse_pressed = 0;
    fractal_transform->color = 0;
    fractal_transform->c_re = -0.7f;
    fractal_transform->c_im = 0.29015f;
    fractal_transform->right_mouse_pressed = 0;
}

void    mishania_utils(t_transform *fractal_transform)
{
    fractal_transform->zoom = 1;
    fractal_transform->delta_x = 0;
    fractal_transform->delta_y = 0;
    fractal_transform->xx = 0;
    fractal_transform->yy = 0;
    fractal_transform->middle_mouse_pressed = 0;
    fractal_transform->color = 1312323123213213;
    fractal_transform->c_re = 0.167366;
    fractal_transform->c_im = 0.689801;
    fractal_transform->right_mouse_pressed = 0;
}

void    ship_utils(t_transform *fractal_transform)
{
    fractal_transform->zoom = 0.7;
    fractal_transform->delta_x = -0.5;
    fractal_transform->delta_y = -0.5;
    fractal_transform->xx = 0;
    fractal_transform->yy = 0;
    fractal_transform->middle_mouse_pressed = 0;
    fractal_transform->color = 1312323123213213;
    fractal_transform->c_re = 0.167366;
    fractal_transform->c_im = 0.689801;
    fractal_transform->right_mouse_pressed = 0;
}

void    spider_utils(t_transform *fractal_transform)
{
    fractal_transform->zoom = 1;
    fractal_transform->delta_x = -0.5;
    fractal_transform->delta_y = 0;
    fractal_transform->xx = 0;
    fractal_transform->yy = 0;
    fractal_transform->middle_mouse_pressed = 0;
    fractal_transform->color = 0;
    fractal_transform->c_re = -0.7f;
    fractal_transform->c_im = 0.29015f;
    fractal_transform->right_mouse_pressed = 0;
}