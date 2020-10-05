/** @file newgame.c */

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
void next_step(int map[200][200], struct cell board[60][200], int rounds, int mapchoice /*, int infected, int healthy, int day, int month, int year*/);
int calculate_future(int i, int j, int map[200][200], struct cell board[60][200], int mapchoice);
//void date_and_infection(struct cell board[60][200], int infected, int healthy, int day, int month, int year);
void infect(int rounds, struct cell board[60][200]);

int main (void){

    struct cell board[60][200] = {0,0};
    int map[200][200] = {0,0};
    
    int i = 0;
    int j = 0;
/*
    int infected = 0;
    int healthy = 0;

    int day = 8;
    int month = 12;
    int year = 2019;
*/
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
        next_step(map, board, rounds, mapchoice /*, infected, healthy, day, month, year*/);
        rounds++;
        #ifdef linux
            usleep(300000);
        #elif WIN32
            Sleep(300);
        #endif
    }
    endwin();

}   /* end of main */

/*********************************************************************/
/**
 \fn void map_choice(void)
 \brief Prints the game title, and lets player choose from the 2 maps available
	\param void
	\return void
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
 \fn void initgame(int mapchoice, int seedchoice)
 \brief initializes the game, depending on the map and seed choises
	\param int mapchoice
    \param int seedchoice
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

/*********************************************************************/
/**
 \fn void next_step(void)
 \brief takes the next step in the game. prints the board according to the current status of cells
	\param void
	\return void
 */
/*********************************************************************/

void next_step(int map[200][200], struct cell board[60][200], int rounds, int mapchoice /*, int infected, int healthy, int day, int month, int year*/){
    int i = 0;
    int j = 0;


    for(i=0; i<59; i++){
        for(j=0; j<200; j++){ //setting the future state with following function
            board[i][j].future = calculate_future(i,j, map, board, mapchoice);
        }
    }

    for(i=0; i<59; i++){
        for(j=0; j<200; j++){ //copying the future board on the current
            board[i][j].current = board[i][j].future;
            if(board[i][j].current == 0){
                board[i][j].infection = false; //empty cell cannot have infection
            }
        }
    }
    
    //printing the map according to the map, cells and infection

    for(i=0; i<59; i++){
        for(j=0; j<200; j++){
            if(map[i][j]==0){
                attron(COLOR_PAIR(3));
                mvprintw(i,j,"~");
                attroff(COLOR_PAIR(3));
            } else if(map[i][j]==1){
                attron(COLOR_PAIR(2));
                mvprintw(i,j," ");
                attroff(COLOR_PAIR(2));
            }

            if(board[i][j].current == 1){
                if(board[i][j].infection == true){
                    attron(COLOR_PAIR(4));
                    mvprintw(i,j,"@");
                    attroff(COLOR_PAIR(4));
                } else{
                    attron(COLOR_PAIR(1));
                    mvprintw(i,j,"@");
                    attroff(COLOR_PAIR(1));
                }
            } else if(board[i][j].current == 0 && map[i][j] == 0){
                attron(COLOR_PAIR(3));
                mvprintw(i,j,"~");
                attroff(COLOR_PAIR(3));
            }
        }
    }

    //date_and_infection(board, infected, healthy, day, month, year);
    infect(rounds, board); //set the random infection

    mvprintw(1,1,"%d", rounds);
    mvprintw(3,1,"Press x to exit");

    refresh();
    
}

/*********************************************************************/
/**
 \fn int calculate_future(int i, int j)
 \brief calculates the future status of cells, according to the rules set in game of life. also added the functionality for the infection.
	\param int i
    \param int j
	\return 1 or 0, depending on the status(dead or alive)
 */
/*********************************************************************/



int calculate_future(int i, int j, int map[200][200], struct cell board[60][200], int mapchoice){

    int neighbors = 0;
    int a, b;

    if(map[i][j]==1){

        if(board[i][j].current == 0){ //if the current cell is empty
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){ //going through all the nine points around the point
                    if(board[i+a][j+b].infection == true){ //checking if neighbors are infected
                        if(mapchoice==1){
                            if(rand()%100<25){ //lower spread chance in square map
                                board[i][j].infection = true;
                            }
                        } else {
                            if(rand()%100<50){ //higher chance in world map
                                board[i][j].infection = true;
                            }
                        }
                    }
                    neighbors += board[i+a][j+b].current; //adding up the values
                }
            }

            if(neighbors == 3){ //if there are 3 neigbors, 
                return 1;       //a new cell is born
            } else {
                return 0;       //otherwise stay empty
            }
        }

        if(board[i][j].current == 1){ //if the current cell is full
            neighbors--;              //deducting itself from the neigbors
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){
                    if(board[i+a][j+b].infection == true){ //checking if neighbors are infected
                        if(mapchoice==1){
                            if(rand()%100<20){  //lower spread chance in square map
                                board[i][j].infection = true;
                            }
                        } else {
                            if(rand()%100<50){ //higher chance in world map
                                board[i][j].infection = true;
                            }
                        }
                    }
                    neighbors += board[i+a][j+b].current;
                }
            }
            if(neighbors>=4 || neighbors<2){ //if neighbors are 4 or over, OR 1 or 0
                return 0;                    //cell dies
            } else {
                return 1;                    //if neighbors are 2 or 3, cell survives
            }
        }
    } else {
        return 0;
    }
}

/*********************************************************************/
/**
 \fn void date_and_infection(void)
 \brief keeps track, and prints the current date, and infections
	\param void
	\return void
 */
/*********************************************************************/
/*
void date_and_infection(struct cell board[60][200], int infected, int healthy, int day, int month, int year){

    int currently_infected = infected;


    for(int i=0; i<59; i++){
        for(int j=0; j<200; j++){
            if(board[i][j].current==1 && board[i][j].infection == true){
                infected++;
            } 
        }
    }

    for(int i=0; i<59; i++){
        for(int j=0; j<200; j++){
            if(board[i][j].current==1 && board[i][j].infection == false){
                healthy++;
            } 
        }
    }

    currently_infected = infected - currently_infected;

    if(infected < 0){
        infected = 0;
    }



    mvprintw(0,0, "%d.%d.%d", day, month, year);
    //mvprintw(1,0, "Infected (cumulative): %d", infected);
    mvprintw(1,0, "Infected: %d", currently_infected);
    mvprintw(4,0,"Press x to exit");
//    mvprintw(5,0, "rounds: %d", rounds);
//    mvprintw(6,0, "healthy c: %d", healthy);

    day++;

    if((month==1||month==3||month==5||month==7||month==8||month==10||month==12)&&day>31){
        month++;
        day = 1;
    } else if((month==4||month==6||month==9||month==11)&&day>30){
        month++;
        day = 1;
    } else if ((month==2)&&day>28){
        month++;
        day = 1;
    }

    if(month>12){
        month = 1;
        year++;
    }

}
*/

/*********************************************************************/
/**
 \fn void infect(int rounds)
 \brief sets infections depending on the round
	\param int rounds
	\return void
 */
/*********************************************************************/

void infect(int rounds, struct cell board[60][200]){

    if(rounds<=1){
        for(int i=14; i<20; i++){
            for(int j=150; j<170; j++){
                if(board[i][j].current==1){
                    if(rand()%100<=50){
                        board[i][j].infection = true;
                    }
                }
            }
        }
    } else if (rounds > 30 && rounds < 40){
        for(int i=18; i<23; i++){
            for(int j=32; j<48; j++){
                if(board[i][j].current==1){
                    if(rand()%100<=50){
                        board[i][j].infection = true;
                    }
                }
            }
        }
    }
}


/*********************************************************************
*********************************************************************/