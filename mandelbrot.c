/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 16:23:10 by cocummin          #+#    #+#             */
/*   Updated: 2019/02/28 19:43:54 by cocummin         ###   ########.fr       */
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

int man_xx;
int man_yy;

unsigned int man_color = 1312;

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

void	man_ckear_image_data(char *image_data)
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
            // if ((newRe * newRe + newIm * newIm) > 4)
            // {
            //     mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x000000);
            // }
            // else
            // {
            //     int man_color = i % 255;
            //     printf("%i\n", man_color);
            //     mlx_pixel_put(mlx_ptr, win_ptr, x, y, man_color);
            // }
            // unsigned int man_color = i % 255;
            // if (man_color > 60)
            //     man_color += 123123123;
            // //unsigned int alpha = 255 * (i < MAX_ITERATIONS);
            // //alpha = alpha << 24;
            // //man_color = man_color + alpha;
            HsvColor hsv;
            hsv.h = i % 256 + man_color;
            hsv.s = 255;
            hsv.v = 255 * (i < MAX_ITERATIONS);
            man_put_point_to_image(image_and_y->image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
            x++;
   // printf("5\n");
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
    }



    int x = 0;
    int y = 0;
               //real and imaginary part of the pixel p
    double newRe, newIm, oldRe, oldIm;
    man_ckear_image_data(image_data);

    int j = 0;

    while (y < Width)
    {
        image_and_y[j].image_data = image_data;
        image_and_y[j].y = y;
        pthread_create(&(pthreads[j]), NULL, man_row_calculations, &(image_and_y[j]));
        y += Width / 10;
        j++;
    }

    int i = 0;
    while (i < 10)
    {
        pthread_join(pthreads[i], NULL);
        i++;
    }
    mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);

    mlx_hook(win_ptr, 2, 1L << 0, man_plus_clicked, mlx_ptr);
    mlx_hook(win_ptr, 4, 1L << 0, man_mouse_pressed, mlx_ptr);
    mlx_hook(win_ptr, 5, 1L << 0, man_mouse_release, mlx_ptr);
    mlx_hook(win_ptr, 6, 1L << 0, man_mouse_move, mlx_ptr);


    mlx_loop(mlx_ptr);
    return(0);
}