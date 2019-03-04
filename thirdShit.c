/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thirdShit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 15:05:14 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/04 12:37:48 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "mlx.h"
// #include "HSVtoRGB.c"
// #include <pthread.h>
// #include <OpenCL/opencl.h>

// # define MAX_ITERATIONS 400
// # define Width 600

double zoom = 1;
double delta_y = 0;
double delta_x = 0;
double cRe = 0.167366;
double cIm = 0.689801;
int middle_mouse_pressed = 0;
int right_mouse_pressed = 0;
int xx;
int yy;

unsigned int color = 1312323123213213;

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
    mishaniabrot(param);
}

int mouse_release(int button, int x, int y, void *param)
{
    if (button == 3)
        middle_mouse_pressed = 0;
    else if (button == 2)
        right_mouse_pressed = 0;
    mishaniabrot(param);
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

    mishaniabrot(param);
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
    mishaniabrot(parse);
    return (0);
}



void    *row_calculate(void *argv)
{

    int x;
    double newRe, newIm, oldRe, oldIm;

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
            newRe = 1 * (x - Width / 2) / (0.5 * zoom * Width) + delta_x;
                newIm = (y - Width / 2) / (0.5 * zoom * Width) + delta_y;
                int i = 0;
                while (i++ < MAX_ITERATIONS)
                {
                    //remember value of previous iteration
                    oldRe = newRe;
                    oldIm = newIm;
                    //the actual iteration, the real and imaginary part are calculated
                    newRe = oldRe * oldRe * oldRe - 3 * oldRe * oldIm + cRe;
                    newIm = 3 * oldRe  * oldRe * oldIm - oldIm +  cIm;
                    if ((newRe * newRe + newIm * newIm) > 4)
                        break;
                }

                HsvColor hsv;
                hsv.h = i % 256 + color;
                hsv.s = 255;
                hsv.v = 255 * (i < MAX_ITERATIONS);
                put_point_to_image(image_and_y->image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
                //printf("x y %i %i\n", x , y);

                x++;
   // printf("5\n");
        }
        y++;
        j++;
    }
}



int mishaniabrot(void   *mlx_ptr)
{
    //printf("MishaniaBrot\n");
   // static void    *mlx_ptr;
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

        //mlx_ptr = mlx_init();
        win_ptr = mlx_new_window(mlx_ptr, Width, Width, "Mishaniabrot");
        image = mlx_new_image(mlx_ptr, Width, Width);
        image_data = mlx_get_data_addr(image, &bytes, &len, &endian);
    }

    int x = 0;
    int y = 0;
    double pr, pi;           //real and imaginary part of the pixel p
    double newRe, newIm, oldRe, oldIm;


    clear_image_data(image_data);
    int j = 0;
    //zoom = 15;
    while (y < Width)
    {
        image_and_y[j].image_data = image_data;
        image_and_y[j].y = y;
        pthread_create(&(pthreads[j]), NULL, row_calculate, &(image_and_y[j]));
        // while (x < Width)
        // {
        //     newRe = 1.5 * (x - Width / 2) / (0.5 * zoom * Width) + delta_x;
        //     newIm = (y - Width / 2) / (0.5 * zoom * Width) + delta_y;
        //     int i = 0;
        //     while (i++ < MAX_ITERATIONS)
        //     {
        //         //remember value of previous iteration
        //         oldRe = newRe;
        //         oldIm = newIm;
        //          //the actual iteration, the real and imaginary part are calculated
        //         newRe = oldRe * oldRe - oldIm * oldIm + cRe;
        //         newIm = 2 * oldRe * oldIm + cIm;
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
        y += Width / 10;
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

    mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);
    //
    mlx_hook(win_ptr, 2, 1L << 0, plus_clicked, mlx_ptr);
    mlx_hook(win_ptr, 4, 1L << 0, mouse_press, mlx_ptr);
    mlx_hook(win_ptr, 5, 1L << 0, mouse_release, mlx_ptr);
    mlx_hook(win_ptr, 6, 1L << 0, mouse_move, mlx_ptr);
    mandelbrot(mlx_ptr);
    //mlx_key_hook(win_ptr, plus_clicked, (void *)0);
    mlx_loop(mlx_ptr);
    return(0);
}

