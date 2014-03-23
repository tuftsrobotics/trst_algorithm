/*   File: state_rep.h
 *   By: Alex Tong, Date: Fri Mar 21
 *   Last Updated: Sun Mar 23 09:27:48
 *
 *  Input formatting and general struct definitions
 */

#ifndef STATE_REP_H
#define STATE_REP_H

/* enviroment input defines */
#ifndef DATA_GEN
    #define NUM_MEMS           9
    #define DATA_FORMAT_STRING "%f;%f;%f;%f;%f;%f;%f;%f;%f\n"
    #define DATA_ARGS          &(env->wind_dir), &(env->wind_speed),\
                           &(env->app_wind_dir), &(env->app_wind_speed),\
                           &(boat->rud_pos), &(boat->sail_pos),\
                           &(boat->pos.lat), &(boat->pos.lon), &(boat->heading)
#endif

#ifdef DATA_GEN
    #define NUM_MEMS           4
    #define DATA_FORMAT_STRING "%f;%f;%f;%f;\n"
    #define DATA_ARGS          &(env->wind_dir), &(env->wind_speed),\
                                    &(env->app_wind_dir), &(env->app_wind_speed)

    #define OUTPUT_FORMAT      "%f;%f;%f;%f;%f;%f;%f;%f;%f\n"
    #define OUTPUT_ARGS        env->wind_dir, env->wind_speed,\
                           env->app_wind_dir, env->app_wind_speed,\
                           boat->rud_pos, boat->sail_pos,\
                           boat->pos.lat, boat->pos.lon, boat->heading
#endif

/* meanigful name */
typedef float Angle;

/* stores lat/lon info in convenient package */
typedef struct Position {
    float lat;
    float lon;
} Position;

/* may be useful */
typedef struct Vector {
    Angle theta;
    Position head;
} Vector;

/* stores environmental data */
typedef struct Env_data {
    /* may replace w/ 2 vectors */
    float wind_dir, wind_speed, app_wind_dir, app_wind_speed;
} *Env_data;

/* stores boat related data */
typedef struct Boat_data {
    Angle rud_pos, sail_pos, heading;
    Position pos;
} *Boat_data;

/* bundles complete state of navigator */
typedef struct Navigator {
    Env_data env;
    Boat_data boat;
    Position *waypts;
    unsigned current_waypt;
    unsigned num_waypts;
} *Navigator;


#endif /* STATE_REP_H */
