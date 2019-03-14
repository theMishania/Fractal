void	kernel_put_point_to_image(__global char *image_data, int x, int y, int julia_color)
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

    if (region == 0)
    {
        rgb.r = hsv.v; rgb.g = t; rgb.b = p;
    }
    else if (region == 1)
    {
        rgb.r = q; rgb.g = hsv.v; rgb.b = p;
    }
    else if (region == 2)
    {
        rgb.r = p; rgb.g = hsv.v; rgb.b = t;
    }
    else if (region == 3)
    {
        rgb.r = p; rgb.g = q; rgb.b = hsv.v;
    }
    else if (region == 4)
    {
        rgb.r = t; rgb.g = p; rgb.b = hsv.v;
    }
    else
    {
        rgb.r = hsv.v; rgb.g = p; rgb.b = q;
    }
    return (rgb);
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

    return (result);
}
