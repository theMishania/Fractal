/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_change.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 20:11:35 by chorange          #+#    #+#             */
/*   Updated: 2019/03/13 20:25:54 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

void	zoom_plus(t_fractal *fractal, int dx, int dy)
{
	fractal->transform.zoom += fractal->transform.zoom * 0.4;
	fractal->transform.delta_x -= dx *
		0.8044 / fractal->transform.zoom / WIDTH;
	fractal->transform.delta_y -= dy *
		0.8044 / fractal->transform.zoom / WIDTH;
}

void	zoom_minus(t_fractal *fractal, int dx, int dy)
{
	if (fractal->transform.zoom < 0.1)
		return ;
	fractal->transform.zoom -= fractal->transform.zoom * 0.4;
	fractal->transform.delta_x += dx *
		0.8044 / fractal->transform.zoom / WIDTH;
	fractal->transform.delta_y += dy *
		0.8044 / fractal->transform.zoom / WIDTH;
}

void	change_consts(t_fractal *fractal, int dx, int dy)
{
	if (dx > 0)
		fractal->transform.c_re += 0.01f / fractal->transform.zoom;
	else if (dx < 0)
		fractal->transform.c_re -= 0.01f / fractal->transform.zoom;
	if (dy > 0)
		fractal->transform.c_im += 0.01f / fractal->transform.zoom;
	else if (dy < 0)
		fractal->transform.c_im -= 0.01f / fractal->transform.zoom;
}

void	move(t_fractal *fractal, int dx, int dy)
{
	if (dx > 0)
		fractal->transform.delta_x += 2 *
			dx / fractal->transform.zoom / WIDTH;
	else if (dx < 0)
		fractal->transform.delta_x += 2 *
			dx / fractal->transform.zoom / WIDTH;
	if (dy > 0)
		fractal->transform.delta_y += 2 *
			dy / fractal->transform.zoom / WIDTH;
	else if (dy < 0)
		fractal->transform.delta_y += 2 *
			dy / fractal->transform.zoom / WIDTH;
}
