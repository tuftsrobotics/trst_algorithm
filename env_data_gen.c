/*   File: env_data_gen.c
 *   By: Joshua Pfosi, Date: Mon Mar 24
 *   Last Updated: Mon Sep 15 11:12:26
 *
 *   Data generation engine for navigation simulations
 *   Limitation: Without boat heading, cannot calculate apparent wind vector
 *               Outputs 0.0 for both components, this can be calculated in
 *               in navigate.c for the DATA_GEN phase
 *
 *   Args: a number of a lines,
 *         a wind direction, 
 *         a wind speed about which the generated speed fluctuates, and
 *         a measure of "shiftiness" (a number, s, between 0-10
 *         that causes the wind speed and direction to shift by 1 * s, and 
 *         10 * s respectively
 *
 *   Ex) ./data_gen 15 300 10 3 outputs 15 lines w/ wind speed shifting
 *   around 7-13 mph blowing from dead south to east south east (270 - 330)
 *
 *   All units assumed to be in miles per hour
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_LINES_DEFAULT    100
#define WIND_DIR_DEFAULT     0
#define WIND_SPEED_DEFAULT   10
#define SHIFT_DEFAULT        0
#define SHIFT_SEVERITY_DIR   10
#define SHIFT_SEVERITY_SPEED 1
#define STEADY_DUR           10
#define SHIFT_DUR            4

void output_data(int num_lines, float wind_dir, float wind_speed, int shift) {
    /* allow fluctuations less than shift */
    int rand_num = 0, s = 0;
    float wind_veer = 0.0, wind_shift = 0.0;
    srand(time(NULL)); /* seed random number generator */
    int shifting = 0, time = 0;

    for (; num_lines > 0; --num_lines, ++time) {
        if (time == STEADY_DUR) {
            shifting = 1;
            time = 0;
        }
        float temp_dir = wind_dir, temp_speed = wind_speed;
        if (shifting) {
            if (time == SHIFT_DUR) {
                shifting = 0;
                time = 0;
            }
            /* model environmental randomness by taking shift number
             * (0 => no shifting), and getting a random value to shift wind
             * speed and direction by (s) and subtracting that from the max shift
             * that's allowed (shift) */
            rand_num = rand();
            s = (shift != 0) ? rand_num % shift : 0;
            s = shift - s;

            /* randomly choose change in direction and speed
             * direction_change / speed change
             * e.g. 0 / 0 => 0 change in direction and speed
             * or   + / - => wind goes right, and looses speed
             */
            switch (rand_num % 9) {
                /* 0 / 0 */
                case 0: break;
                        /* 0 / + */
                case 1: wind_shift = s; break;
                        /* + / 0 */
                case 2: wind_veer = s; break;
                        /* 0 / - */
                case 3: wind_shift = -s; break;
                        /* - / 0 */
                case 4: wind_veer = -s; break;
                        /* + / - */
                case 5: wind_veer = s; wind_shift = -s; break;
                        /* - / + */
                case 6: wind_veer = -s; wind_shift = s; break;
                        /* + / + */
                case 7: wind_veer = s; wind_shift = s; break;
                        /* - / - */
                case 8: wind_veer = -s; wind_shift = -s; break;
            }

            /* calculate shifts */
            temp_dir += wind_veer * SHIFT_SEVERITY_DIR;
            temp_speed += wind_shift * SHIFT_SEVERITY_SPEED;
            /* TODO: should be temp_speed not wind_speed */
            if (wind_speed < 0) {
                wind_speed = 0;
            }

        }
        fprintf(stdout, "%f;%f;%f;%f\n", temp_dir, temp_speed, 0.0, 0.0);
    }
}

int main(int argc, char **argv) {
    int num_lines = NUM_LINES_DEFAULT, shift = SHIFT_DEFAULT;
    float wind_dir = WIND_DIR_DEFAULT, 
          wind_speed = WIND_SPEED_DEFAULT;

    /* cannot handle more than 3 argument */
    if (argc > 5) { 
        fprintf(stderr, "%s: usage: [num_lines] [wind dir] [wind speed] "
                "[shiftiness]\n", argv[0]);
        exit(1);
    }

    /* overwrite each default by num of args */
    if (argc > 1)
        num_lines = atoi(argv[1]);
    if (argc > 2)
        wind_dir = atof(argv[2]);
    if (argc > 3)
        wind_speed = atof(argv[3]);
    if (argc > 4)
        shift = atoi(argv[4]);

    output_data(num_lines, wind_dir, wind_speed, shift);

    return 0;
}

