/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 16:23:10 by cocummin          #+#    #+#             */
/*   Updated: 2019/02/28 17:21:26 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "HSVtoRGB.c"
#include <pthread.h>

#define MAX_ITERATIONS 300
# define Width 600

double zoom = 1;
double delta_y = 0;
double delta_x = 0;

int xx;
int yy;

unsigned int color;

int middle_mouse_pressed = 0;

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
    else if (button == 4)
    {
        zoom += zoom * 0.4;
        delta_x -= dx * 0.002 / zoom;
        delta_y -= dy * 0.00135 / zoom;
    }
    else if (button == 5)
    {
        zoom -= zoom * 0.4;
        delta_x += dx * 0.002 / zoom;
        delta_y += dy * 0.00135 / zoom;
    }
    xx = x;
    yy = y;
}

int mouse_release(int button, int x, int y, void *param)
{
    if (button == 3)
        middle_mouse_pressed = 0;
    //else if (key == 2)
    //    right_mouse_pressed = 0;
    main();
}

int mouse_move(int x, int y, void *param)
{
    int dx = xx - x;
    int dy = yy - y;

    if (middle_mouse_pressed)
    {
        if (dx > 0)
            delta_x += 0.005*dx / zoom;
        else if (dx < 0)
            delta_x += 0.005*dx / zoom ;
        if (dy > 0)
            delta_y += 0.005*dy / zoom;
        else if (dy < 0)
            delta_y += 0.005*dy / zoom;
        yy = y;
        xx = x;
    }
    main();
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
    main();
    return (0);
}

void	put_point_to_image(char *image_data, int x, int y, int color)
{
	int	index;

	if (x < 0 || y < 0 || x > Width - 1 || y > Width - 1)
		return ;
	else
	{
		index = Width * y * 4 + x * 4;
		image_data[index + 2] = color >> 16;
		image_data[index + 1] = (color & 0x00ff00) >> 8;
	}
}

void	clear_image_data(char *image_data)
{
	int index;

	index = 0;
	while (index < Width * Width * 4)
		image_data[index++] = 0;
}

void    *row_calculate(void *argv)
{

    int x;
    double newRe, newIm, oldRe, oldIm;
    double pr, pi;

    t_image_and_y *image_and_y;

    image_and_y = (t_image_and_y *)argv;

    int y = image_and_y->y;
    //printf("%i\n", y);
    int j = 0;
    while (j < Width / 10)
    {
        x = 0;
        while (x < Width)
        {
            pr = 1 * (x - Width / 2) / (0.5 * 1 * Width * zoom) + -0.5 + delta_x;
            pi = (y - Width / 2) / (0.5 * 1 * Width * zoom) + 0 + delta_y;
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
            //     int color = i % 255;
            //     printf("%i\n", color);
            //     mlx_pixel_put(mlx_ptr, win_ptr, x, y, color);
            // }
            // unsigned int color = i % 255;
            // if (color > 60)
            //     color += 123123123;
            // //unsigned int alpha = 255 * (i < MAX_ITERATIONS);
            // //alpha = alpha << 24;
            // //color = color + alpha;
            HsvColor hsv;
            hsv.h = i % 256 + color;
            hsv.s = 255;
            hsv.v = 255 * (i < MAX_ITERATIONS);
            put_point_to_image(image_and_y->image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
            x++;
   // printf("5\n");
        }
        y++;
        j++;
    }
}

int main()
{
    static void    *mlx_ptr;
    static void    *win_ptr;
    static void    *image;
    static char     *image_data;
    static pthread_t       pthreads[10];
    t_image_and_y image_and_y[10];

    if (!(win_ptr))
    {
        int bytes;
	    int len;
	    int endian;

	    bytes = 8;
	    len = Width;
	    endian = 0;

        mlx_ptr = mlx_init();
    win_ptr = mlx_new_window(mlx_ptr, Width, Width, "Mandelbrot");
        image = mlx_new_image(mlx_ptr, Width, Width);
        image_data = mlx_get_data_addr(image, &bytes, &len, &endian);
    }



    int x = 0;
    int y = 0;
               //real and imaginary part of the pixel p
    double newRe, newIm, oldRe, oldIm;
    clear_image_data(image_data);

    int j = 0;

    while (y < Width)
    {
        image_and_y[j].image_data = image_data;
        image_and_y[j].y = y;
        pthread_create(&(pthreads[j]), NULL, row_calculate, &(image_and_y[j]));
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

    mlx_hook(win_ptr, 2, 1L << 0, plus_clicked, (void*)0);
    mlx_hook(win_ptr, 4, 1L << 0, mouse_press, (void*)0);
    mlx_hook(win_ptr, 5, 1L << 0, mouse_release, (void*)0);
    mlx_hook(win_ptr, 6, 1L << 0, mouse_move, (void*)0);


    mlx_loop(mlx_ptr);
    return(0);
}