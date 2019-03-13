/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fractal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:22:53 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/13 17:22:41 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

void    draw_fractal(t_fractal *fractal)
{
    static int windows_count;
    
    if (!fractal->win_ptr)
    {
        transform_init(fractal);
        fractal_init(fractal);
        windows_count++;
    }

    fractal->ret = clEnqueueWriteBuffer(fractal->command_queue, fractal->utils_memobj, CL_TRUE, 0, sizeof(t_transform), &fractal->transform, 0, NULL, NULL);
    //printf("Recall3\n");
    fractal->ret = clSetKernelArg(fractal->kernel, 1, sizeof(cl_mem), (void *)&fractal->utils_memobj);
   // printf("Recall3.5\n");
    size_t global_work_size = WIDTH * WIDTH;
    
    fractal->ret = clEnqueueNDRangeKernel(fractal->command_queue, fractal->kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    fractal->ret = clEnqueueReadBuffer(fractal->command_queue, fractal->memobj, CL_TRUE, 0, WIDTH * WIDTH * 4, fractal->image_data, 0, NULL, NULL);



    mlx_put_image_to_window(fractal->mlx_ptr, fractal->win_ptr, fractal->image, 0, 0);
    mlx_string_put(fractal->mlx_ptr, fractal->win_ptr, 50, 50, 0x000000, "Iterations Count:");
    mlx_string_put(fractal->mlx_ptr, fractal->win_ptr, 230, 50, 0x000000, ft_itoa(fractal->transform.max_iterations));
    
    mlx_hook(fractal->win_ptr, 2, 1L << 0, plus_clicked,fractal);
    mlx_hook(fractal->win_ptr, 4, 1L << 0, mouse_press, fractal);
    mlx_hook(fractal->win_ptr, 5, 1L << 0, mouse_release, fractal);
    mlx_hook(fractal->win_ptr, 6, 1L << 0, mouse_move, fractal);

    //printf("draw %i\n",fractal->is_last);
    mlx_hook(fractal->win_ptr, 17, 1L << 0, close_window, &windows_count);
}