/** @file init.c */

#include "newgame.h"

/*********************************************************************/
/**
 \fn int map_choice(void)
 \brief Prints the game title, and lets player choose from the 2 maps available
	\param void
	\return 1 or 2, depending on the map choice
 */
/*********************************************************************/


int map_choice(void){
    int yMax,xMax;
    getmaxyx(stdscr, yMax, xMax);

    /* Init the map menu */
    WINDOW * mapwin = newwin(4, 20, yMax-30, xMax/2-10);
    box(mapwin, 0, 0);
    char *maps[2] = {"Square", "World Map"};
    int option;
    int highlight = 0;
    
    keypad(mapwin, true);

    


    refresh();

    /* Printing title */

    mvprintw(2,xMax/2-81,"  ,ad8888ba,                                                                 ad88       ,ad8888ba,                                                                ");
    mvprintw(3,xMax/2-81," d8'     `'8b                                                               d8'        d8*'    `'8b                                                               ");
    mvprintw(4,xMax/2-81,"d8'                                                                         88        d8'                                                                         ");
    mvprintw(5,xMax/2-81,"88             ,adPPYYba,  88,dPYba,,adPYba,    ,adPPYba,      ,adPPYba,  MM88MMM     88              ,adPPYba,   8b,dPPYba,   ,adPPYba,   8b,dPPYba,   ,adPPYYba,");
    mvprintw(6,xMax/2-81,"88      88888  ''     `Y8  88P'   '88'    '8a  a8P_____88     a8'     '8a   88        88             a8'     '8a  88P'   'Y8  a8'     '8a  88P'   `*8a  ''     `Y8");
    mvprintw(7,xMax/2-81,"Y8,        88  ,adPPPPP88  88      88      88  8PP*******     8b       d8   88        Y8,            8b       d8  88          8b       d8  88       88  ,adPPPPP88");
    mvprintw(8,xMax/2-81," Y8a.    .a88  88,    ,88  88      88      88  '8b,   ,aa     '8a,   ,a8'   88         Y8a.    .a8P  '8a,   ,a8'  88          '8a,   ,a8'  88       88  88,    ,88");
    mvprintw(9,xMax/2-81,"  `*Y88888P*   `*8bbdP'Y8  88      88      88   `*Ybbd8*'      `*YbbdP*'    88          `*Y8888Y*'    `*YbbdP*'   88           `*YbbdP*'   88       88  `*8bbdP'Y8");

    refresh();

    /* map option functionality */

    while(1){
        for(int o = 0; o < 2; o++){
            if(o == highlight){
                wattron(mapwin, A_REVERSE);
            }
            mvwprintw(mapwin, o+1, 1, maps[o]);
            wattroff(mapwin, A_REVERSE);
        }
        option = wgetch(mapwin);

        switch (option)
        {
        case KEY_UP:
            highlight--;
            if(highlight == -1){
                highlight = 0;
            }
            break;
        case KEY_DOWN:
            highlight++;
            if(highlight == 2){
                highlight = 1;
            }
            break;
        default:
            break;
        }

        if(option == 10){
            if(highlight == 0){
                return 1;
            } else if (highlight == 1){
                return 2;
            }
            break;
        }
    }

    wrefresh(mapwin);
}

/*********************************************************************/
/**
 \fn void initgame(int mapchoice, int map[200][200], struct cell board[60][200])
 \brief initializes the game, depending on the map and seed choises
	\param int mapchoice
    \param int map[200][200]
    \param struct cell board[60][200]
	\return void
*/
/*********************************************************************/


void initgame(int mapchoice, int map[200][200], struct cell board[60][200]){
    int i = 0;
    int j = 0;

    clear();

    char line[205];
    FILE *file_pointer;

    file_pointer = fopen("worldmaplarge1.txt", "r");
    int a = 0;

    if(mapchoice==1){                   //square map
        for(i=0; i<59; i++){
            for(j=0; j<200; j++){
                map[i][j]=1;            //set the whole square as playable
                int r = rand()%2;       //50% chance of creating a cell
                board[i][j].current = r;
            }
            refresh();
        } 
    } else if (mapchoice==2) {          //world map

        while(fgets(line, sizeof line, file_pointer) != NULL){
            move(a,0);
            int i=0;
            for(int j=0; j<sizeof line; j++){
                if(line[j]=='~'){
                    map[a][j] = 0;      //if map is 0, that means water
                    attron(COLOR_PAIR(3));
                    printw("%c", line[j]);
                    attroff(COLOR_PAIR(3));
                } else {
                    map[a][j] = 1;      //if map is 1, that means land
                    attron(COLOR_PAIR(2));
                    printw("%c", line[j]);
                    attroff(COLOR_PAIR(2));
                }
                refresh();
                i++;
            }
            a++;
        }

        for(i=0; i<60; i++){
            for(j=0; j<200; j++){
                board[i][j].infection = false; //set infection initially to 0
                if(map[i][j]==0){              //cannot live on water
                    board[i][j].current = 0;
                } else if (map[i][j]==1){      //if theres land, cells can be made
                    int r = rand()%2;
                    board[i][j].current = r;
                }
            }
        }
    }

}