/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 17:18:41 by cocummin          #+#    #+#             */
/*   Updated: 2019/02/28 19:25:20 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "mlx.h"
// #include "HSVtoRGB.c"
// #include <pthread.h>
#include <stdio.h>

# define MAX_ITERATIONS 400
# define Width 600

double  julia_zoom = 1;
double julia_delta_y = 0;
double julia_delta_x = 0;
double julia_cRe = -0.7f;
double julia_cIm = 0.29015f;
int julia_middle_mouse_pressed = 0;
int julia_right_mouse_pressed = 0;
int julia_xx;
int julia_yy;

unsigned int julia_color;

typedef struct julia_imageAnd_y
{
    char *image_data;
    int y;
}          julia_amage_and_y;

int julia_mouse_press(int button, int x, int y, void *param)
{
    int dx = Width/2 - x;
    int dy = Width/2 - y;

    if (button == 3)
        julia_middle_mouse_pressed = 1;
    else if (button == 2)
       julia_right_mouse_pressed = 1;
    else if (button == 4)
    {
        julia_zoom += julia_zoom * 0.4;
        julia_delta_x -= dx * 0.00135 /   julia_zoom;
        julia_delta_y -= dy * 0.00135 /   julia_zoom;
    }
    else if (button == 5)
    {
        julia_zoom -= julia_zoom * 0.4;
        julia_delta_x += dx * 0.00135 /   julia_zoom;
        julia_delta_y += dy * 0.00135 /   julia_zoom;
    }
    julia_xx = x;
    julia_yy = y;
    julia(param);
}

int julia_mouse_release(int button, int x, int y, void *param)
{
    if (button == 3)
        julia_middle_mouse_pressed = 0;
    else if (button == 2)
        julia_right_mouse_pressed = 0;
    julia(param);
}

int julia_mouse_move(int x, int y, void *param)
{
    int dx = julia_xx - x;
    int dy = julia_yy - y;

    if (julia_right_mouse_pressed)
    {
        if (dx > 0)
            julia_cRe += 0.01f /  julia_zoom;
        else if (dx < 0)
            julia_cRe -= 0.01f /  julia_zoom ;
        if (dy > 0)
            julia_cIm += 0.01f /  julia_zoom;
        else if (dy < 0)
            julia_cIm -= 0.01f /  julia_zoom;
        julia_yy = y;
        julia_xx = x;
    }
    if (julia_middle_mouse_pressed)
    {
        if (dx > 0)
            julia_delta_x += 0.00335*dx / julia_zoom;
        else if (dx < 0)
            julia_delta_x += 0.00335*dx / julia_zoom ;
        if (dy > 0)
            julia_delta_y += 0.00335*dy / julia_zoom;
        else if (dy < 0)
            julia_delta_y += 0.00335*dy / julia_zoom;
        julia_yy = y;
        julia_xx = x;
    }

    julia(param);
}

void	julia_put_point_to_image(char *image_data, int x, int y, int julia_color)
{
	int	index;

	if (x < 0 || y < 0 || x > Width - 1 || y > Width - 1)
		return ;
	else
	{
		index = Width * y * 4 + x * 4;
        image_data[index] = julia_color & 0x0000ff;
		image_data[index + 2] = julia_color >> 16; // red
		image_data[index + 1] = (julia_color & 0x00ff00) >> 8; //green
	}
}

void	julia_clear_image_data(char *image_data)
{
	int index;

	index = 0;
	while (index < Width * Width * 4)
		image_data[index++] = 0;
}

int julia_plus_clicked(int key, void *parse)
{
    if (key == 0x18)
        julia_zoom += 2;
    else if (key == 0x1B)
        julia_zoom -= 2;
    else if (key == 0x7C)
        julia_delta_x += 0.1;
    else if (key == 0x7B)
        julia_delta_x -= 0.1;
    else if (key == 0x7E)
        julia_delta_y += 0.1;
    else if (key == 0x7D)
        julia_delta_y -= 0.1;
    else if (key == 0x08)
        julia_color += 0x000002;
    else if (key == 0x35)
        exit(-2);
    julia(parse);
    return (0);
}



void    *julia_row_calculate(void *argv)
{

    int x;
    double newRe, newIm, oldRe, oldIm;

    julia_amage_and_y *image_and_y;

    image_and_y = (julia_amage_and_y *)argv;

    int y = image_and_y->y;
    //printf("%i\n", y);
    int j = 0;
    while (j < 60)
    {
        x = 0;
        while (x < Width)
        {
            newRe = 1 * (x - Width / 2) / (0.5 *    julia_zoom * Width) + julia_delta_x;
                newIm = (y - Width / 2) / (0.5 *    julia_zoom * Width) + julia_delta_y;
                int i = 0;
                while (i++ < MAX_ITERATIONS)
                {
                    //remember value of previous iteration
                    oldRe = newRe;
                    oldIm = newIm;
                    //the actual iteration, the real and imaginary part are calculated
                    newRe = oldRe * oldRe - oldIm * oldIm + julia_cRe;
                    newIm = 2 * oldRe * oldIm + julia_cIm;
                    if ((newRe * newRe + newIm * newIm) > 4)
                        break;
                }

                HsvColor hsv;
                hsv.h = i % 256 + julia_color;
                hsv.s = 255;
                hsv.v = 255 * (i < MAX_ITERATIONS);
                julia_put_point_to_image(image_and_y->image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
                //printf("x y %i %i\n", x , y);

                x++;
   // printf("5\n");
        }
        y++;
        j++;
    }
}



int julia(void *mlx_ptr)
{
    //printf("Julia\n");
    //static void    *mlx_ptr;
    static void    *win_ptr;
    static void    *image;
    static char     *image_data;
    static pthread_t       pthreads[10];
    julia_amage_and_y image_and_y[10];

    if (!(win_ptr))
    {
        int bytes;
	    int len;
	    int endian;

	    bytes = 8;
	    len = Width;
	    endian = 0;

        //mlx_ptr = mlx_init();
        win_ptr = mlx_new_window(mlx_ptr, Width, Width, "Julia");
        image = mlx_new_image(mlx_ptr, Width, Width);
        image_data = mlx_get_data_addr(image, &bytes, &len, &endian);
    }

    int x = 0;
    int y = 0;
    double pr, pi;           //real and imaginary part of the pixel p
    double newRe, newIm, oldRe, oldIm;


    julia_clear_image_data(image_data);
    int j = 0;
    //julia_zoom = 15;
    while (y < Width)
    {
        image_and_y[j].image_data = image_data;
        image_and_y[j].y = y;
        pthread_create(&(pthreads[j]), NULL, julia_row_calculate, &(image_and_y[j]));
        // while (x < Width)
        // {
        //     newRe = 1.5 * (x - Width / 2) / (0.5 *   julia_zoom * Width) + julia_delta_x;
        //     newIm = (y - Width / 2) / (0.5 * julia_zoom * Width) + julia_delta_y;
        //     int i = 0;
        //     while (i++ < MAX_ITERATIONS)
        //     {
        //         //remember value of previous iteration
        //         oldRe = newRe;
        //         oldIm = newIm;
        //          //the actual iteration, the real and imaginary part are calculated
        //         newRe = oldRe * oldRe - oldIm * oldIm + julia_cRe;
        //         newIm = 2 * oldRe * oldIm + julia_cIm;
        //         if ((newRe * newRe + newIm * newIm) > 4)
        //             break;
        //     }
        //     // if ((newRe * newRe + newIm * newIm) > 4)
        //     // {
        //     //     //mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0x000000);
        //     //     put_point_to_image(image_data, x, y, 0x000000);
        //     // }
        //     // else
        //     // {
        //     //     //mlx_pixel_put(mlx_ptr, win_ptr, x, y, 0xff0000);
        //     //     put_point_to_image(image_data, x, y, 0xff0000);
        //     // }
        //     HsvColor hsv;
        //     hsv.h = i % 256 - 223;
        //     hsv.s = 255;
        //     hsv.v = 255 * (i < MAX_ITERATIONS);
        //     put_point_to_image(image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
        //     x++;
        // }
        ///y += 200;
        y += 60;
        j++;
    }

    int i = 0;
    while (i < 10)
    {
        pthread_join(pthreads[i], NULL);
        i++;
    }
    // i = 0;
    // while (i < 10)
    // {
    //     pthread_join(pthreads[i], NULL);
    //     i++;
    // }
    // pthread_join(pthreads[0], NULL);
    // pthread_join(pthreads[1], NULL);
    // pthread_join(pthreads[2], NULL);
    // pthread_join(pthreads[3], NULL);
    // pthread_join(pthreads[4], NULL);
    // pthread_join(pthreads[5], NULL);
    // pthread_join(pthreads[6], NULL);
    // pthread_join(pthreads[7], NULL);
    // pthread_join(pthreads[8], NULL);
    // pthread_join(pthreads[9], NULL);
    //mishaniabrot(mlx_ptr);
    mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);
    
    mlx_hook(win_ptr, 2, 1L << 0, julia_plus_clicked, mlx_ptr);
    mlx_hook(win_ptr, 4, 1L << 0, julia_mouse_press, mlx_ptr);
    mlx_hook(win_ptr, 5, 1L << 0, julia_mouse_release, mlx_ptr);
    mlx_hook(win_ptr, 6, 1L << 0, julia_mouse_move, mlx_ptr);
    mishaniabrot(mlx_ptr);
    //mlx_key_hook(win_ptr, plus_clicked, (void *)0);
    mlx_loop(mlx_ptr);
    return(0);
}