/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <cocummin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 18:42:57 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/06 17:12:39 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
//#include "HSVtoRGB.c"
#include <pthread.h>
//#include "julia.c"
//#include "mishaniabrot.c"
//#include "mandelbrot.c"
#include "ship.c"

int main()
{
    static void     *mlx_ptr;
    //pthread_t thread;

    mlx_ptr = mlx_init();
   // pthread_create(&thread, NULL, open_another_window, mlx_ptr);

    //julia(mlx_ptr);
    //mandelbrot(mlx_ptr);
    //mishaniabrot(mlx_ptr);
    burning_ship(mlx_ptr);

    //pthread_join(thread, NULL);

    return (0);

}