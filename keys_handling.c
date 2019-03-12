/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 21:15:50 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/12 15:28:31 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

int mouse_press(int button, int x, int y, void *param)
{
    int dx = WIDTH/2 - x;
    int dy = WIDTH/2 - y;
    t_fractal *fractal;

    fractal = (t_fractal *)param;
    if (button == 3)
        fractal->transform.middle_mouse_pressed = 1;
    else if (button == 2)
       fractal->transform.right_mouse_pressed = 1;
    else if (button == 4)
    {
        fractal->transform.zoom += fractal->transform.zoom * 0.4;
        fractal->transform.delta_x -= dx * 0.00135 / fractal->transform.zoom;
        fractal->transform.delta_y -= dy * 0.00135 /  fractal->transform.zoom;
    }
    else if (button == 5)
    {
        fractal->transform.zoom -= fractal->transform.zoom * 0.4;
        fractal->transform.delta_x += dx * 0.00135 /   fractal->transform.zoom;
        fractal->transform.delta_y += dy * 0.00135 /   fractal->transform.zoom;
    }
    fractal->transform.xx = x;
    fractal->transform.yy = y;
    //julia(param);
    draw_fractal(fractal);
}

int mouse_release(int button, int x, int y, void *param)
{
    t_fractal *fractal;

    fractal = (t_fractal *)param;
    if (button == 3)
        fractal->transform.middle_mouse_pressed = 0;
    else if (button == 2)
        fractal->transform.right_mouse_pressed  = 0;
    draw_fractal(fractal);
}

int mouse_move(int x, int y, void *param)
{
    t_fractal *fractal;

    fractal = (t_fractal *)param;
    int dx = fractal->transform.xx - x;
    int dy = fractal->transform.yy - y;
    if (fractal->transform.right_mouse_pressed)
    {
        if (dx > 0)
            fractal->transform.c_re += 0.01f /  fractal->transform.zoom;
        else if (dx < 0)
            fractal->transform.c_re -= 0.01f /  fractal->transform.zoom ;
        if (dy > 0)
            fractal->transform.c_im += 0.01f /  fractal->transform.zoom;
        else if (dy < 0)
            fractal->transform.c_im -= 0.01f /  fractal->transform.zoom;
        fractal->transform.yy = y;
        fractal->transform.xx = x;
    }
    if (fractal->transform.middle_mouse_pressed)
    {
        if (dx > 0)
            fractal->transform.delta_x += 0.00335*dx / fractal->transform.zoom;
        else if (dx < 0)
            fractal->transform.delta_x += 0.00335*dx / fractal->transform.zoom;
        if (dy > 0)
            fractal->transform.delta_y += 0.00335*dy / fractal->transform.zoom;
        else if (dy < 0)
            fractal->transform.delta_y += 0.00335*dy / fractal->transform.zoom;
        fractal->transform.yy = y;
        fractal->transform.xx  = x;
    }
    draw_fractal(fractal);
    //julia(param);
}

int plus_clicked(int key, void *param)
{
    t_fractal *fractal;

    fractal = (t_fractal *)param;
    if (key == 0x18)
        fractal->transform.zoom += 2;
    else if (key == 0x1B)
        fractal->transform.zoom -= 2;
    else if (key == 0x7C)
        fractal->transform.delta_x += 0.1;
    else if (key == 0x7B)
        fractal->transform.delta_x -= 0.1;
    else if (key == 0x7E)
        fractal->transform.delta_y += 0.1;
    else if (key == 0x7D)
        fractal->transform.delta_y -= 0.1;
    else if (key == 0x08)
        fractal->transform.color += 0x000002;
    else if (key == 0x12 && fractal->transform.max_iterations > 50)
        fractal->transform.max_iterations -= 10;
    else if (key == 0x13 && fractal->transform.max_iterations < 1000)
        fractal->transform.max_iterations += 10;
    else if (key == 0x35)
        exit(-2);
    draw_fractal(fractal);
    return (0);
}