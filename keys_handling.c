/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 21:15:50 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/13 20:22:10 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

int	mouse_press(int button, int x, int y, void *param)
{
	int			dx;
	int			dy;
	t_fractal	*fractal;

	dx = WIDTH / 2 - x;
	dy = WIDTH / 2 - y;
	fractal = (t_fractal *)param;
	if (button == 3)
		fractal->transform.middle_mouse_pressed = 1;
	else if (button == 2)
		fractal->transform.right_mouse_pressed = 1;
	else if (button == 4)
		zoom_plus(fractal, dx, dy);
	else if (button == 5)
		zoom_minus(fractal, dx, dy);
	fractal->transform.xx = x;
	fractal->transform.yy = y;
	draw_fractal(fractal);
	return (0);
}

int	mouse_release(int button, int x, int y, void *param)
{
	t_fractal *fractal;

	fractal = (t_fractal *)param;
	(void)x;
	(void)y;
	if (button == 3)
		fractal->transform.middle_mouse_pressed = 0;
	else if (button == 2)
		fractal->transform.right_mouse_pressed = 0;
	draw_fractal(fractal);
	return (0);
}

int	mouse_move(int x, int y, void *param)
{
	t_fractal	*fractal;
	int			dx;
	int			dy;

	fractal = (t_fractal *)param;
	dx = fractal->transform.xx - x;
	dy = fractal->transform.yy - y;
	if (fractal->transform.right_mouse_pressed)
	{
		change_consts(fractal, dx, dy);
	}
	if (fractal->transform.middle_mouse_pressed)
	{
		move(fractal, dx, dy);
	}
	fractal->transform.yy = y;
	fractal->transform.xx = x;
	draw_fractal(fractal);
	return (0);
}

int	key_pressed(int key, void *param)
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

int	close_window(void *param)
{
	int *windows_count;

	windows_count = (int *)param;
	if (*windows_count == 1)
		exit(-2);
	(*windows_count)--;
	return (0);
}
