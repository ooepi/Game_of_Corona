/** @file main.c */

#include "newgame.h"

int main (void){

    struct cell board[60][200] = {0,0};
    int map[200][200] = {0,0};
    
    int i = 0;
    int j = 0;

    int rounds = 0;

    initscr();
    cbreak();
    curs_set(0);
    noecho();
    nodelay(stdscr, true);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    srand(time(NULL));

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_GREEN); //color 1 = life
    init_pair(2, COLOR_GREEN, COLOR_GREEN); //color 2 = ground
    init_pair(3, COLOR_WHITE, COLOR_BLUE);  //color 3 = water
    init_pair(4, COLOR_RED, COLOR_GREEN);   //color 4 = infection

    int mapchoice = map_choice();

    initgame(mapchoice, map, board);


    while(getch()!='x'){
        next_step(map, board, rounds, mapchoice);
        rounds++;
        #ifdef linux
            usleep(300000);
        #elif WIN32
            Sleep(300);
        #endif
    }
    endwin();

}   /* end of main */