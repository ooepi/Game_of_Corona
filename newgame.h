/** @file newgame.h */

#ifdef WIN32
    #include <windows.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct cell{
    int current;        /**< Sets the current cell to dead or alive */
    int future;         /**< Sets the future cell to dead or alive */
    bool infection;     /**< Shows the current status of the infection */
};

int map_choice(void);
void initgame(int mapchoice, int map[200][200], struct cell board[60][200]); 
void next_step(int map[200][200], struct cell board[60][200], int rounds, int mapchoice);
int calculate_future(int i, int j, int map[200][200], struct cell board[60][200], int mapchoice);
void infect(int rounds, struct cell board[60][200]);