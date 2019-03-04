/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocummin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 18:42:57 by cocummin          #+#    #+#             */
/*   Updated: 2019/03/02 11:13:20 by cocummin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "HSVtoRGB.c"
#include <pthread.h>
#include "julia.c"
#include "thirdShit.c"
#include "mandelbrot.c"

int main()
{
    static void     *mlx_ptr;
    //pthread_t thread;

    mlx_ptr = mlx_init();
   // pthread_create(&thread, NULL, open_another_window, mlx_ptr);

    julia(mlx_ptr);
    //mishaniabrot(mlx_ptr);

    //pthread_join(thread, NULL);

    return (0);

}