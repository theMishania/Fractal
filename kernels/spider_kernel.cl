__kernel void spider(__global char *image_data, __global t_transform *transform)
{
    int y = get_global_id(0) / WIDTH;
    int x = get_global_id(0) % WIDTH;

    //int x = (y % 2 == 0) ? 0 : 300;
    //int julia_color = 0xFF0000;

    double newRe, newIm, oldRe, oldIm;

    // while (x < ((y % 2 == 0) ? 300 : 600))
    // {
    //     y = y / 2;
        newRe = 1 * (x - WIDTH / 2) / (0.5 *    transform->zoom * WIDTH) + transform->delta_x;//julia_struct->julia_delta_x;
        newIm = (y - WIDTH / 2) / (0.5 *    transform->zoom * WIDTH) + transform->delta_y;
        transform->c_re = newRe;
        transform->c_im = newIm;
        int i = 0;
        while (i++ < transform->max_iterations)
        {
            //remember value of previous iteration
            oldRe = newRe;
            oldIm = newIm;
            //the actual iteration, the real and imaginary part are calculated
            newRe = oldRe * oldRe - oldIm * oldIm + transform->c_re;
            newIm = 2 * oldRe * oldIm + transform->c_im;
            transform->c_re = transform->c_re / 2 + newRe;
            transform->c_im = transform->c_im / 2 + newIm;
            if ((newRe * newRe + newIm * newIm) > 4)
                break;
        }

        HsvColor hsv;
        hsv.h = i % 256 + transform->color;//julia_struct->julia_color;
        hsv.s = 255;
        hsv.v = 255 * (i < transform->max_iterations);
        kernel_put_point_to_image(image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
}
