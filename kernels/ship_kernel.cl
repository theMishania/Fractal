//# define MAX_ITERATIONS 400
# define WIDTH 1024
//# define abs (a) return (a >= 0) ? (a) : -(a)

void	ship_kernel_put_point_to_image(__global char *image_data, int x, int y, int ship_color)
{
	int	index = 0;

	if (x < 0 || y < 0 || x > WIDTH - 1 || y > WIDTH - 1)
		return ;
	else
	{
		index = WIDTH * y * 4 + x * 4;
        image_data[index] = ship_color & 0x0000ff;
		image_data[index + 2] = ship_color >> 16; // red
		image_data[index + 1] = (ship_color & 0x00ff00) >> 8; //green
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




__kernel void ship(__global char *image_data, __global t_transform *transform)
{
    int y = get_global_id(0) / WIDTH; 
    int x = get_global_id(0) % WIDTH;

    //int x = (y % 2 == 0) ? 0 : 300;
    //int julia_color = 0xFF0000;

    double zx, zy, real_x, real_y;

    // while (x < ((y % 2 == 0) ? 300 : 600))
    // {
    //     y = y / 2;
        real_x = 1 * (x - WIDTH / 2) / (0.5 *   transform->zoom * WIDTH) + transform->delta_x;// ship_struct->ship_delta_x;
        real_y = (y - WIDTH / 2) / (0.5 *  transform->zoom * WIDTH) + transform->delta_y; // ship_struct->ship_delta_y;

        zx = real_x;
        zy = real_y;
        int i = 0;
        while (i++ < transform->max_iterations)
        {
            //remember value of previous iteration
            
            //the actual iteration, the real and imaginary part are calculated
            double temp_x = zx * zx - zy * zy + real_x;
            zy = (2 * zx * zy >= 0) ? (2 * zx * zy + real_y) : (-2 * zx * zy + real_y);
            zx = (temp_x >= 0) ? temp_x : -temp_x;
            if ((zx * zx + zy * zy) > 4)
                break;
        }

        HsvColor hsv;
        hsv.h = i % 256 + transform->color; // ship_struct->ship_color;
        hsv.s = 255;
        hsv.v = 255 * (i < transform->max_iterations);
        ship_kernel_put_point_to_image(image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
}

