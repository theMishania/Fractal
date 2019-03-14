/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractal_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chorange <chorange@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 12:30:28 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/14 13:42:05 by chorange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"

void		transform_init(t_fractal *fractal)
{
	if (fractal->fractal_type == mandelbrot)
		mandelbrot_utils(&fractal->transform);
	else if (fractal->fractal_type == julia)
		julia_utils(&fractal->transform);
	else if (fractal->fractal_type == mishaniabrot)
		mishania_utils(&fractal->transform);
	else if (fractal->fractal_type == ship)
		ship_utils(&fractal->transform);
	else if (fractal->fractal_type == spider)
		spider_utils(&fractal->transform);
	fractal->transform.max_iterations = 50;
}

static void	fractal_init_2(t_fractal *fractal)
{
	fractal->context = clCreateContext(NULL, 1, &fractal->device_id,
			NULL, NULL, &fractal->ret);
	fractal->command_queue = clCreateCommandQueue(fractal->context,
			fractal->device_id, 0, &fractal->ret);
	fractal->source_size = cl_source_str_gen(fractal->file_name,
			&(fractal->source_str));
	fractal->program = clCreateProgramWithSource(fractal->context, 1,
			(const char **)&fractal->source_str,
			(const size_t *)&fractal->source_size, &fractal->ret);
	fractal->ret = clBuildProgram(fractal->program, 1,
			&fractal->device_id, NULL, NULL, NULL);
	fractal->kernel = clCreateKernel(fractal->program,
			fractal->kernel_name, &fractal->ret);
	fractal->memobj = clCreateBuffer(fractal->context,
			CL_MEM_READ_WRITE, WIDTH * WIDTH * 4, NULL, &fractal->ret);
	fractal->utils_memobj = clCreateBuffer(fractal->context,
			CL_MEM_READ_WRITE, sizeof(t_transform), NULL, &fractal->ret);
	fractal->ret = clEnqueueWriteBuffer(fractal->command_queue,
			fractal->memobj, CL_TRUE, 0, WIDTH * WIDTH * 4,
			fractal->image_data, 0, NULL, NULL);
	fractal->ret = clSetKernelArg(fractal->kernel, 0,
			sizeof(cl_mem), (void *)&fractal->memobj);
}

void		fractal_init(t_fractal *fractal)
{
	int bytes;
	int len;
	int endian;

	names_init(fractal);
	bytes = 8;
	len = WIDTH;
	endian = 0;
	fractal->win_ptr = mlx_new_window(fractal->mlx_ptr,
			WIDTH, WIDTH, fractal->win_name);
	fractal->image = mlx_new_image(fractal->mlx_ptr,
			WIDTH, WIDTH);
	fractal->image_data = mlx_get_data_addr(fractal->image,
			&bytes, &len, &endian);
	fractal->ret = clGetPlatformIDs(1, &fractal->platform_id,
			&fractal->ret_num_platforms);
	fractal->ret = clGetDeviceIDs(fractal->platform_id,
			CL_DEVICE_TYPE_GPU, 1, &fractal->device_id,
			&fractal->ret_num_devices);
	fractal_init_2(fractal);
	free(fractal->source_str);
}
