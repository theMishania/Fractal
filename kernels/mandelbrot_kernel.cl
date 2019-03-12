__kernel void mandelbrot(__global char *image_data, __global t_transform *transform)
{
    int y = get_global_id(0) / WIDTH;
    int x = get_global_id(0) % WIDTH;

    //int x = (y % 2 == 0) ? 0 : 300;
    //int julia_color = 0xFF0000;

    double newRe, newIm, oldRe, oldIm;
    double pr, pi;

    // while (x < ((y % 2 == 0) ? 300 : 600))
    // {
    //     y = y / 2;
        pr = 1 * (x - WIDTH / 2) / (0.5 * 1 * WIDTH * transform->zoom) + -0.5 + transform->delta_x;// man_struct->man_delta_x;
        pi = (y - WIDTH / 2) / (0.5 * 1 * WIDTH * transform->zoom) + 0 + transform->delta_y; //man_struct->man_delta_y;
        newRe = newIm = oldRe = oldIm = 0;
        int i = 0;
        while (i++ < transform->max_iterations)
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
        HsvColor hsv;
        hsv.h = i % 256 + transform->color;// man_struct->man_color;
        hsv.s = 255;
        hsv.v = 255 * (i < transform->max_iterations);
        kernel_put_point_to_image(image_data, x, y, rgb_to_int(HsvToRgb(hsv)));
}
