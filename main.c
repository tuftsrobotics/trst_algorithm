/*   File: main.c
 *   By: Alex Tong, Date: Tue Mar 11
 *   Last Updated: Sat Mar 22 10:52:01
 *
 *   main file for TRST project
 *   Arg: File w/ semicolon delimited list of doubles
 *   Format: Parses list into two partitions:
 *           1) General goal information (i.e. waypoints)
 *           2) Sailing data (second by second data)
 *   Environment and boat data passed to high level decision maker
 */

/* test engine will be file w/ ENVIRONMENTAL data and FIRST line with 
 * waypoint(s), and SECOND line with rudder, sail position, and 
 * current heading  - them env data is read and the boat state is 
 * mutated
 */

/* If you want a vimrc script to automatically place my style of heading at 
 * the top of your files and keep the "Last Updated" up to date on a save, 
 * let me know, I found my old script on Github! 
 */

#include <stdlib.h>
#include <stdio.h>
#include "input.h"

extern int read_data(FILE *input);

int main(int argc, char **argv) {

    FILE *fp = NULL;
    int exit_status = 0;

    /* reads in data until there is an error */
    if (argc > 2) { /* cannot handle more than 1 argument */
        fprintf(stderr, "%s: usage: [filename]\n", argv[0]);
        exit(1);
    }
    else if (argc == 1) /* no filename was given so use stdin */
        fp = stdin; 
    else /* num_args == 2 so use filename */
        fp = open_or_abort(argv[1], "r");

    /* start main program */
    exit_status = read_data(fp);
    fclose(fp);

    if (exit_status != 0) {
        fprintf(stderr, "Aborting due to unresolvable input error\n");
        return exit_status;
    }
    return exit_status;
}

