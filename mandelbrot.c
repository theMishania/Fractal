/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 16:23:10 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/06 11:26:29 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "mlx.h"
// #include "HSVtoRGB.c"
// #include <pthread.h>

// #define MAX_ITERATIONS 300
// # define Width 600

double man_zoom = 1;
double man_delta_y = 0;
double man_delta_x = 0;
unsigned int man_color = 1312;

typedef struct s_man_struct
{
    double man_zoom;
    double man_delta_y;
    double man_delta_x;
    unsigned int man_color;
}               t_man_struct;

int man_xx;
int man_yy;



int man_middle_mouse_pressed = 0;

typedef struct main_image_And_y
{
    char *image_data;
    int y;
}          man_image_and_y;

int man_mouse_pressed(int button, int x, int y, void *param)
{
    int dx = Width/2 - x;
    int dy = Width/2 - y;

    if (button == 3)
        man_middle_mouse_pressed = 1;
    else if (button == 4)
    {
        man_zoom += man_zoom * 0.4;
        man_delta_x -= dx * 0.002 / man_zoom;
        man_delta_y -= dy * 0.00135 / man_zoom;
    }
    else if (button == 5)
    {
        man_zoom -= man_zoom * 0.4;
        man_delta_x += dx * 0.002 / man_zoom;
        man_delta_y += dy * 0.00135 / man_zoom;
    }
    man_xx = x;
    man_yy = y;
    mandelbrot(param);
}

int man_mouse_release(int button, int x, int y, void *param)
{
    if (button == 3)
        man_middle_mouse_pressed = 0;
    //else if (key == 2)
    //    right_mouse_pressed = 0;
    mandelbrot(param);
}

int man_mouse_move(int x, int y, void *param)
{
    int dx = man_xx - x;
    int dy = man_yy - y;

    if (man_middle_mouse_pressed)
    {
        if (dx > 0)
            man_delta_x += 0.005*dx / man_zoom;
        else if (dx < 0)
            man_delta_x += 0.005*dx / man_zoom ;
        if (dy > 0)
            man_delta_y += 0.005*dy / man_zoom;
        else if (dy < 0)
            man_delta_y += 0.005*dy / man_zoom;
        man_yy = y;
        man_xx = x;
    }
    mandelbrot(param);
}

int man_plus_clicked(int key, void *parse)
{
    if (key == 0x18)
        man_zoom += 2;
    else if (key == 0x1B)
        man_zoom -= 2;
    else if (key == 0x7C)
        man_delta_x += 0.1;
    else if (key == 0x7B)
        man_delta_x -= 0.1;
    else if (key == 0x7E)
        man_delta_y += 0.1;
    else if (key == 0x7D)
        man_delta_y -= 0.1;
    else if (key == 0x08)
        man_color += 0x000002;
    else if (key == 0x35)
        exit(-2);
    mandelbrot(parse);
    return (0);
}

void	man_put_point_to_image(char *image_data, int x, int y, int man_color)
{
	int	index;

	if (x < 0 || y < 0 || x > Width - 1 || y > Width - 1)
		return ;
	else
	{
		index = Width * y * 4 + x * 4;
		image_data[index + 2] = man_color >> 16;
		image_data[index + 1] = (man_color & 0x00ff00) >> 8;
	}
}

void	man_clear_image_data(char *image_data)
{
	int index;

	index = 0;
	while (index < Width * Width * 4)
		image_data[index++] = 0;
}

void    *man_row_calculations(void *argv)
{

    int x;
    double newRe, newIm, oldRe, oldIm;
    double pr, pi;

    man_image_and_y *image_and_y;

    image_and_y = (man_image_and_y *)argv;

    int y = image_and_y->y;
    //printf("%i\n", y);
    int j = 0;
    while (j < Width / 10)
    {
        x = 0;
        while (x < Width)
        {
            pr = 1 * (x - Width / 2) / (0.5 * 1 * Width * man_zoom) + -0.5 + man_delta_x;
            pi = (y - Width / 2) / (0.5 * 1 * Width * man_zoom) + 0 + man_delta_y;
            newRe = newIm = oldRe = oldIm = 0;
            int i = 0;
            while (i++ < MAX_ITERATIONS)
            {
                //remember value of previous iteration
                oldRe = newRe;
                oldIm = newIm;
                 //the actual iteration, the real and imaginary part are calculated
                newRe = oldRe * oldRe - oldIm * oldIm + pr;
                newIm = 2 * oldRe * oldIm + pi;
                if ((newRe * newRe + newIm * newIm) > 4)
                    break;
            }

            HsvColor hsv;
            hsv.h = i % 256 + man_color;
            hsv.s = 255;
            hsv.v = 255 * (i < MAX_ITERATIONS);
            man_put_point_to_image(image_and_y->image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
            x++;
        }
        y++;
        j++;
    }
}

int mandelbrot(void *mlx_ptr)
{
    //static void    *mlx_ptr;
    static void    *win_ptr;
    static void    *image;
    static char     *image_data;
    static pthread_t       pthreads[10];
    man_image_and_y image_and_y[10];


    //------OpenCL--------------------
    static cl_int          ret;
    static cl_platform_id  platform_id;
    static cl_uint         ret_num_platforms;

    static cl_device_id device_id;
    static cl_uint ret_num_devices;

    static cl_context context;
    static cl_command_queue command_queue;

    static cl_program program = NULL;
    static cl_kernel kernel = NULL;

     static FILE *fd;

    static const char fileName[] = "mandelbrot_kernel.cl";
    static size_t source_size;
    static char *source_str;

    static cl_mem memobj = NULL;

    static cl_mem man_memobj = NULL;

//-------------------------------------------------------

    t_man_struct man_struct;

    man_struct.man_zoom = man_zoom;
    man_struct.man_delta_x = man_delta_x;
    man_struct.man_delta_y = man_delta_y;
    man_struct.man_color = man_color;

    if (!(win_ptr))
    {
        int bytes;
	    int len;
	    int endian;

	    bytes = 8;
	    len = Width;
	    endian = 0;

        //mlx_ptr = mlx_init();
        win_ptr = mlx_new_window(mlx_ptr, Width, Width, "Mandelbrot");
        image = mlx_new_image(mlx_ptr, Width, Width);
        image_data = mlx_get_data_addr(image, &bytes, &len, &endian);

        //OpenCl_Init----------------------------------------------
        ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
        ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

        context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
        command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

        fd = fopen(fileName, "r");
        if (!fd)
        {
            printf("Cannot open kernel file\n");
            exit(1);
        }
        source_str = (char *)malloc(4000);
        source_size = fread(source_str, 1, 4000, fd);
        fclose(fd);

        program = clCreateProgramWithSource(context, 1,(const char **)&source_str, (const size_t *)&source_size,  &ret);
        ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
        kernel = clCreateKernel(program, "mandelbrot", &ret);


        memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, Width * Width * 4, NULL, &ret);
        man_memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(t_man_struct), NULL, &ret);

        ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, Width * Width * 4, image_data, 0, NULL, NULL);
       // ret = clEnqueueWriteBuffer(command_queue, julia_memobj, CL_TRUE, 0, sizeof(t_julia_struct), &julia_struct, 0, NULL, NULL);

        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
       // ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&julia_memobj);
    }


    ret = clEnqueueWriteBuffer(command_queue, man_memobj, CL_TRUE, 0, sizeof(t_man_struct), &man_struct, 0, NULL, NULL);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&man_memobj);


    size_t global_work_size = Width * Width;

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, Width * Width * 4, image_data, 0, NULL, NULL);

    mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);

    mlx_hook(win_ptr, 2, 1L << 0, man_plus_clicked, mlx_ptr);
    mlx_hook(win_ptr, 4, 1L << 0, man_mouse_pressed, mlx_ptr);
    mlx_hook(win_ptr, 5, 1L << 0, man_mouse_release, mlx_ptr);
    mlx_hook(win_ptr, 6, 1L << 0, man_mouse_move, mlx_ptr);


    mlx_loop(mlx_ptr);
    return(0);
}