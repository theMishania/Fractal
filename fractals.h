/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/07 21:08:08 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/12 14:38:15 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <OpenCL/opencl.h>
#include "minilibx/mlx.h"
#include "libft/libft.h"
#define WIDTH 1024

typedef enum e_fractal_type
{
    julia,
    mandelbrot,
    mishaniabrot,
    ship,
    spider,
}           t_fractal_type;

typedef struct  s_transform
{

    double  zoom;
    double delta_y;
    double delta_x;
    double c_re;
    double c_im;
    unsigned int color;
    unsigned int max_iterations;
    int     middle_mouse_pressed;
    int right_mouse_pressed;
    int xx;
    int yy;
}               t_transform;

typedef struct s_fractal
{
    void    *mlx_ptr;
    void    *win_ptr;
    void    *image;
    char     *image_data;

    t_fractal_type fractal_type;

    t_transform transform;

    cl_int          ret;
    cl_platform_id  platform_id;
    cl_uint         ret_num_platforms;

    cl_device_id device_id;
    cl_uint ret_num_devices;

    cl_context context;
    cl_command_queue command_queue;

    cl_program program;
    cl_kernel kernel;

    FILE *fd;

    char *file_name;
    char *win_name;
    char *kernel_name;

    size_t source_size;
    char *source_str;
    cl_mem memobj;

    cl_mem utils_memobj;
}           t_fractal;

void arguments_check(int argc, char **argv);
int is_names_correct(int argc, char **argv);

void    initiate_fractals(void *mlx_ptr, int argc, char **argv);
void    utils_create(void *mlx_ptr, int argc, t_fractal *fractals);

void    draw_fractal(t_fractal *fractal);

void    transform_init(t_fractal *fractal);

void    mandelbrot_utils(t_transform *fractal_transform);
void    julia_utils(t_transform *fractal_transform);
void    mishania_utils(t_transform *fractal_transform);
void    ship_utils(t_transform *fractal_transform);
void    spider_utils(t_transform *fractal_transform);

void    fractal_init(t_fractal *fractal);
void    names_init(t_fractal *fractal);

int mouse_press(int button, int x, int y, void *param);
int mouse_release(int button, int x, int y, void *param);
int mouse_move(int x, int y, void *param);
int plus_clicked(int key, void *param);