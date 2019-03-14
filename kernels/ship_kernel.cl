__kernel void ship(__global char *image_data, __global t_transform *transform)
{
    int y = get_global_id(0) / WIDTH;
    int x = get_global_id(0) % WIDTH;

    double zx, zy, real_x, real_y;

    real_x = 1 * (x - WIDTH / 2) / (0.5 *   transform->zoom * WIDTH) + transform->delta_x;// ship_struct->ship_delta_x;
    real_y = (y - WIDTH / 2) / (0.5 *  transform->zoom * WIDTH) + transform->delta_y; // ship_struct->ship_delta_y;

    zx = real_x;
    zy = real_y;
    int i = 0;
    while (i++ < transform->max_iterations)
    {
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
    kernel_put_point_to_image(image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
}
