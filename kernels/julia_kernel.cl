//# define MAX_ITERATIONS 400
# define WIDTH 1024

void	julia_kernel_put_point_to_image(__global char *image_data, int x, int y, int julia_color)
{
	int	index = 0;

	if (x < 0 || y < 0 || x > WIDTH - 1 || y > WIDTH - 1)
		return ;
	else
	{
		index = WIDTH * y * 4 + x * 4;
        image_data[index] = julia_color & 0x0000ff;
		image_data[index + 2] = julia_color >> 16; // red
		image_data[index + 1] = (julia_color & 0x00ff00) >> 8; //green
	}
}

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

typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6; 

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = hsv.v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v;
            break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}

int rgb_to_int(RgbColor rgb)
{
    int result = 0;
    int blue = 0;
    int green = 0;
    int red = 0;

    red = (int)rgb.r;
    red = red << 16;

    green = (int)rgb.g;
    green = green << 8;

    blue = (int)rgb.b;

    result = red + green + blue;

    return result;
}




__kernel void julia(__global char *image_data, __global t_transform *transform)
{
    int y = get_global_id(0) / WIDTH; 
    int x = get_global_id(0) % WIDTH;

    //int x = (y % 2 == 0) ? 0 : 300;
    //int julia_color = 0xFF0000;

    double newRe, newIm, oldRe, oldIm;

    // while (x < ((y % 2 == 0) ? 300 : 600))
    // {
    //     y = y / 2;
        newRe = 1 * (x - WIDTH / 2) / (0.5 *   transform->zoom  * WIDTH) + transform->delta_x;//julia_struct->julia_delta_x;
        newIm = (y - WIDTH / 2) / (0.5 *    transform->zoom  * WIDTH) + transform->delta_y;//julia_struct->julia_delta_y;
        int i = 0;
        while (i++ < transform->max_iterations)
        {
            //remember value of previous iteration
            oldRe = newRe;
            oldIm = newIm;
            //the actual iteration, the real and imaginary part are calculated
            newRe = oldRe * oldRe - oldIm * oldIm + transform->c_re;//julia_struct->julia_cRe;
            newIm = 2 * oldRe * oldIm + transform->c_im;//julia_struct->julia_cIm;
            if ((newRe * newRe + newIm * newIm) > 4)
                break;
        }

        HsvColor hsv;
        hsv.h = i % 256 + transform->color;//julia_struct->julia_color;
        hsv.s = 255;
        hsv.v = 255 * (i < transform->max_iterations);
        julia_kernel_put_point_to_image(image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
}

