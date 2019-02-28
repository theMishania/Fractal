#include "mlx.h"
#include "HSVtoRGB.c"
# define MAX_ITERATIONS 400
# define Width 1000

double zoom = 1;
double delta_y = 0;
double delta_x = 0;
int left_mouse_pressed = 0;
int right_mouse_pressed = 0;
int xx;
int yy;

double x,y,newx,newy,t,p;

int mouse_press(int button, int x, int y, void *param)
{
    xx = x;
    yy = y;
    if (button == 1)
        left_mouse_pressed = 1;
    else if (button == 2)
       right_mouse_pressed = 1;
    else if (button == 4)
        zoom += zoom * 0.4;
    else if (button == 5)
        zoom -= zoom * 0.4;
    main();
}

int mouse_release(int button, int x, int y, void *param)
{
    if (button == 1)
        left_mouse_pressed = 0;
    else if (button == 2)
        right_mouse_pressed = 0;
    main();
}

int mouse_move(int x, int y, void *param)
{
    int dx = xx - x;
    int dy = yy - y;
/*
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
    }*/
    if (left_mouse_pressed)
    {
        if (dx > 0)
            delta_x += 0.5*dx / zoom;
        else if (dx < 0)
            delta_x += 0.5*dx / zoom ;
        if (dy > 0)
            delta_y += 0.5*dy / zoom;
        else if (dy < 0)
            delta_y += 0.5*dy / zoom;
        yy = y;
        xx = x;
		main();
    }


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
		image_data[index] = color & 0x0000ff;
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


int main()
{
    static void    *mlx_ptr;
    static void    *win_ptr;
    static void    *image;
    static char     *image_data;

    if (!(win_ptr))
    {
        int bytes;
	    int len;
	    int endian;

	    bytes = 8;
	    len = Width;
	    endian = 0;

        mlx_ptr = mlx_init();
        win_ptr = mlx_new_window(mlx_ptr, Width, Width, "Julia");
        image = mlx_new_image(mlx_ptr, Width, Width);
        image_data = mlx_get_data_addr(image, &bytes, &len, &endian);
		double pr, pi;           //real and imaginary part of the pixel p
    	double newRe, newIm, oldRe, oldIm;

		int i=0,randnum;


		double k;
		int mid_x,mid_y,radius;
		mid_x=640/2;
		mid_y=480/2;
		radius=(int)(0.1*mid_y);
		x=1.0;y=1.0;





    }

    //int x = 0;
    //int y = 0;

    clear_image_data(image_data);
    //zoom = 15;





int i = 0;

	while (i<190000)
	{
		p=(rand()%100)/(100*1.0);
		t=x;
		if (p<=0.85)
		{  //vysota
			x=0.84*x+0.04*y;
			y=0.04*t+0.85*y+1.6;
		}
		else if (p<0.93)
		{             //simm sleva
			x=0.20*x-0.26*y;
			y=0.26*t+0.212*y+0.44;
		}
		else if (p<0.99)
		{        //simmetr sprava
			x=-0.15*x+0.28*y;
			y=0.26*t+0.24*y+0.44;
		}
		else
		{   //stvol
			x=0.0;
			y=0.16*y;
		}
		/*HsvColor hsv;
		hsv.h = i % 256 - 223;
		hsv.s = 255;
		hsv.v = 255 * (i < MAX_ITERATIONS);*/
		int x_draw;
		int y_draw;
		x_draw = -delta_x + 320+floor(61*x)*zoom+1;
		y_draw = -delta_y + 470-(floor(40*y)*zoom+1);
		if (x_draw >= 0 && x_draw <= Width & y_draw >= 0 && y_draw <= Width)
		{
			put_point_to_image(image_data, x_draw, y_draw,0xFFFFFF);
			i++;
		}
		//mlx_pixel_put(mlx_ptr, win_ptr, 320+floor(61*x)+1,470-(floor(40*y)+1),0xFFFFFF);

		//
		mlx_hook(win_ptr, 2, 1L << 0, plus_clicked, (void*)0);
		mlx_hook(win_ptr, 4, 1L << 0, mouse_press, (void*)0);
		mlx_hook(win_ptr, 5, 1L << 0, mouse_release, (void*)0);
		mlx_hook(win_ptr, 6, 1L << 0, mouse_move, (void*)0);
		mlx_key_hook(win_ptr, plus_clicked, (void *)0);
	}
	mlx_put_image_to_window(mlx_ptr, win_ptr, image, 0, 0);
	mlx_loop(mlx_ptr);
    return(0);
}