/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ship.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:05:14 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/06 20:04:37 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "HSVtoRGB.c"
#include <pthread.h>
#include <stdio.h>
#include <OpenCL/opencl.h>

// # define MAX_ITERATIONS 400
 # define Width 1024

double zoom = 0.7;
double delta_y = -0.5;
double delta_x = -0.5;
double cRe = 0.167366;
double cIm = 0.689801;
int middle_mouse_pressed = 0;
int right_mouse_pressed = 0;
int xx;
int yy;

unsigned int color = 1312323123213213;

typedef struct s_ship_struct
{
    double  ship_zoom;
    double ship_delta_y;
    double ship_delta_x;
    unsigned int ship_color;
}                   t_ship_struct;

typedef struct image_andAnd_y
{
    char *image_data;
    int y;
}          t_image_and_y;

int mouse_press(int button, int x, int y, void *param)
{
    int dx = Width/2 - x;
    int dy = Width/2 - y;

    if (button == 3)
        middle_mouse_pressed = 1;
    else if (button == 2)
       right_mouse_pressed = 1;
    else if (button == 4)
    {
        zoom += zoom * 0.4;
        delta_x -= dx * 0.00135 / zoom;
        delta_y -= dy * 0.00135 / zoom;
    }
    else if (button == 5)
    {
        zoom -= zoom * 0.4;
        delta_x += dx * 0.00135 / zoom;
        delta_y += dy * 0.00135 / zoom;
    }
    xx = x;
    yy = y;
    burning_ship(param);
}

int mouse_release(int button, int x, int y, void *param)
{
    if (button == 3)
        middle_mouse_pressed = 0;
    else if (button == 2)
        right_mouse_pressed = 0;
    burning_ship(param);
}

int mouse_move(int x, int y, void *param)
{
    int dx = xx - x;
    int dy = yy - y;

    if (right_mouse_pressed)
    {
        if (dx > 0)
            cRe += 0.01f / zoom;
        else if (dx < 0)
            cRe -= 0.01f / zoom ;
        if (dy > 0)
            cIm += 0.01f / zoom;
        else if (dy < 0)
            cIm -= 0.01f / zoom;
        yy = y;
        xx = x;
    }
    if (middle_mouse_pressed)
    {
        if (dx > 0)
            delta_x += 0.00335*dx / zoom;
        else if (dx < 0)
            delta_x += 0.00335*dx / zoom ;
        if (dy > 0)
            delta_y += 0.00335*dy / zoom;
        else if (dy < 0)
            delta_y += 0.00335*dy / zoom;
        yy = y;
        xx = x;
    }

    burning_ship(param);
}

void	put_point_to_image(char *image_data, int x, int y, int color)
{
	int	index;

	if (x < 0 || y < 0 || x > Width - 1 || y > Width - 1)
		return ;
	else
	{
		index = Width * y * 4 + x * 4;
        image_data[index] = color & 0x0000ff;
		image_data[index + 2] = color >> 16; // red
		image_data[index + 1] = (color & 0x00ff00) >> 8; //green
	}
}

void	clear_image_data(char *image_data)
{
	int index;

	index = 0;
	while (index < Width * Width * 4)
		image_data[index++] = 0;
}

int plus_clicked(int key, void *parse)
{
    if (key == 0x18)
        zoom += 2;
    else if (key == 0x1B)
        zoom -= 2;
    else if (key == 0x7C)
        delta_x += 0.1;
    else if (key == 0x7B)
        delta_x -= 0.1;
    else if (key == 0x7E)
        delta_y += 0.1;
    else if (key == 0x7D)
        delta_y -= 0.1;
    else if (key == 0x08)
        color += 0x000002;
    else if (key == 0x35)
        exit(-2);
    burning_ship(parse);
    return (0);
}



// void    *row_calculate(void *argv)
// {

//     int x;
//     double newRe, newIm, oldRe, oldIm;

//     t_image_and_y *image_and_y;

//     image_and_y = (t_image_and_y *)argv;

//     int y = image_and_y->y;
//     //printf("%i\n", y);
//     int j = 0;
//     while (j < Width / 10)
//     {
//         x = 0;
//         while (x < Width)
//         {
//                 newRe = 1 * (x - Width / 2) / (0.5 * zoom * Width) + delta_x;
//                 newIm = (y - Width / 2) / (0.5 * zoom * Width) + delta_y;
//                 int i = 0;
//                 while (i++ < MAX_ITERATIONS)
//                 {
//                     //remember value of previous iteration
//                     oldRe = newRe;
//                     oldIm = newIm;
//                     //the actual iteration, the real and imaginary part are calculated
//                     newRe = oldRe * oldRe * oldRe - 3 * oldRe * oldIm + cRe;
//                     newIm = 3 * oldRe  * oldRe * oldIm - oldIm +  cIm;
//                     if ((newRe * newRe + newIm * newIm) > 4)
//                         break;
//                 }

//                 HsvColor hsv;
//                 hsv.h = i % 256 + color;
//                 hsv.s = 255;
//                 hsv.v = 255 * (i < MAX_ITERATIONS);
//                 put_point_to_image(image_and_y->image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
//                 //printf("x y %i %i\n", x , y);

//                 x++;
//    // printf("5\n");
//         }
//         y++;
//         j++;
//     }
// }



int burning_ship(void   *mlx_ptr)
{
    //printf("MishaniaBrot\n");
   // static void    *mlx_ptr;
    static void    *win_ptr;
    static void    *image;
    static char     *image_data;
    static pthread_t       pthreads[10];
    t_image_and_y image_and_y[10];

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

    static const char fileName[] = "ship_kernel.cl";
    static size_t source_size;
    static char *source_str;
    static cl_mem memobj = NULL;

    static cl_mem ship_memobj = NULL;

//-------------------------------------------------------

    t_ship_struct ship_struct;
    
    ship_struct.ship_zoom = zoom;
    ship_struct.ship_delta_y = delta_y;
    ship_struct.ship_delta_x = delta_x;
    ship_struct.ship_color = color;

    if (!(win_ptr))
    {
        int bytes;
	    int len;
	    int endian;

	    bytes = 8;
	    len = Width;
	    endian = 0;

        //mlx_ptr = mlx_init();
        win_ptr = mlx_new_window(mlx_ptr, Width, Width, "Burning Ship");
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
        kernel = clCreateKernel(program, "ship", &ret);


        memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, Width * Width * 4, NULL, &ret);
        ship_memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(t_ship_struct), NULL, &ret);

        ret = clEnqueueWriteBuffer(command_queue, memobj, CL_TRUE, 0, Width * Width * 4, image_data, 0, NULL, NULL);
       // ret = clEnqueueWriteBuffer(command_queue, julia_memobj, CL_TRUE, 0, sizeof(t_julia_struct), &julia_struct, 0, NULL, NULL);

        ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memobj);
       // ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&julia_memobj);

    }


    clear_image_data(image_data);

    ret = clEnqueueWriteBuffer(command_queue, ship_memobj, CL_TRUE, 0, sizeof(t_ship_struct), &ship_struct, 0, NULL, NULL);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&ship_memobj);


    size_t global_work_size = Width * Width;

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    ret = clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, Width * Width * 4, image_data, 0, NULL, NULL);



    mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);
    //
    mlx_hook(win_ptr, 2, 1L << 0, plus_clicked, mlx_ptr);
    mlx_hook(win_ptr, 4, 1L << 0, mouse_press, mlx_ptr);
    mlx_hook(win_ptr, 5, 1L << 0, mouse_release, mlx_ptr);
    mlx_hook(win_ptr, 6, 1L << 0, mouse_move, mlx_ptr);
    //mandelbrot(mlx_ptr);
    //mlx_key_hook(win_ptr, plus_clicked, (void *)0);
    //mlx_loop(mlx_ptr);
    return(0);
}

