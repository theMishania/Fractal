/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flourish_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 11:18:46 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/18 11:20:44 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

void	flourish_utils(t_transform *fractal_transform)
{
	fractal_transform->zoom = 0.65;
	fractal_transform->delta_x = 0;
	fractal_transform->delta_y = 0;
	fractal_transform->xx = 0;
	fractal_transform->yy = 0;
	fractal_transform->middle_mouse_pressed = 0;
	fractal_transform->color = 0;
	fractal_transform->c_re = 0.490668;
	fractal_transform->c_im = 0.725257;
	fractal_transform->right_mouse_pressed = 0;
}