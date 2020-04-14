#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct cell{
    int current;
    int future;
};

void init_board(void);
void draw_board(int xstart, int ystart);
void game_setup(void);
void next_step(void);
int calculate_neighbors(int i, int j);
void print_curses(void);
int main_menu(void);
void game_of_life(void);
void game_of_war(void);
void game_of_corona(void);
void choose_map(void);
void choose_seed(void);
void init_map(int map_number);
void draw_square(int size);
void init_seed(int seed);

struct cell board[69][255] = {0};

char map[150][40] = {' '};


/*VARIABLES*/
int gamemode, map_choice, seed_choice;

int x, y, i, j;
int yMax = 10, xMax = 10;
int ySize, xSize;

/*START OF MAIN*/

int main(void){

    initscr();
    cbreak();
    curs_set(0);

    srand(time(NULL));




    getmaxyx(stdscr, yMax, xMax);


    main_menu();


    x=170;
    y=60;

//    init_board();

//    game_setup();

//    print_curses();

}

/*END OF MAIN*/

/*INIT BOARD*/

void init_board(){

    for(i=0; i<y; i++){
        for(j=0; j<x; j++){
            board[i][j].current = 0;    //set the whole boards values to 0;
        }
    }

}

/*DRAW BOARD*/

void draw_board(int xstart, int ystart){

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);
    

    WINDOW *gameplaywin = newwin(40, 150, 20, 20);
    box(gameplaywin, 0, 0);


    if(map_choice==1){
        x=40+xstart;
        y=20+ystart;
    } else if (map_choice == 2) {
        x=148+xstart;
        y=38+ystart;
    }



    for(i=ystart; i<y; i++){
        for(j=xstart; j<x; j++){
            if(board[i][j].current == 1){
                wattron(gameplaywin, COLOR_PAIR(1));
                mvwprintw(gameplaywin, i-21, j-21, " ");    //if value is 1, print the cell
                wattroff(gameplaywin, COLOR_PAIR(1));
            } else {
                wattron(gameplaywin, COLOR_PAIR(2));
                mvwprintw(gameplaywin, i-21, j-21, " ");    //otherwise print empty
                wattroff(gameplaywin, COLOR_PAIR(2));
            }
        }
        wprintw(gameplaywin, "\n");
    }
    box(gameplaywin, 0, 0);
    wrefresh(gameplaywin);
}

/*GAME SETUP*/

void game_setup(){

    //setting up with predefined points

    board[1][2].current = 1;
    board[2][2].current = 1;
    board[3][0].current = 1;
    board[3][3].current = 1;
    board[3][4].current = 1;
    board[4][3].current = 1;
    board[4][4].current = 1;

}

/*NEXT STEP*/

void next_step(){


    for(i=0; i<60; i++){
        for(j=0; j<170; j++){ //setting the future state with following function
            board[i][j].future = calculate_neighbors(i,j);
        }
    }



    for(i=0; i<60; i++){
        for(j=0; j<170; j++){ //copying the future board on the current
            board[i][j].current = board[i][j].future;
        }
    }
}

/*CALCULATE NEIGHBORS*/


int calculate_neighbors(int i, int j){

    int neighbors = 0;
    int a, b;


    if(board[i][j].current == 0){ //if the current cell is empty
        for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){ //going through all the nine points around the point
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
                neighbors += board[i+a][j+b].current;
            }
        }
        if(neighbors>=4 || neighbors<2){ //if neighbors are 4 or over, OR 1 or 0
            return 0;                    //cell dies
        } else {
            return 1;                    //if neighbors are 2 or 3, cell survives
        }
    }
}

/*
void print_curses(void){

    initscr();

    start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLUE, COLOR_WHITE);

//    bkgd(COLOR_PAIR(1));


    for(int s = 0; s<20; s++){

        draw_board();
        next_step();
        refresh();

        Sleep(500);
        refresh();

    }

    getch();
    endwin();

}
*/
int main_menu(void){

    /* Separator */

    for(int s = 0; s<yMax; s++){
        mvprintw(s, xMax-30, "##");
    }

    
    /* Init Title */
    WINDOW * titlewin = newwin(12, 125, 5, xMax/2-80);
    wborder(titlewin, 35, 35, 35, 35, 35, 35, 35, 35);

    /* Init the game mode menu */
    WINDOW * gamemodewin = newwin(5, 20, 20, xMax-25);
    box(gamemodewin, 0, 0);
    char *gamemodes[3] = {"Game of Life", "Game of War", "Game of Corona"};
    int option;
    int highlight = 0;
    keypad(gamemodewin, true);


    refresh();

    /* Printing title */

    mvwprintw(titlewin, 2,2,"  ,ad8888ba,                                                                 ad88                                         ");
    mvwprintw(titlewin, 3,2," d8'     `'8b                                                               d8'                                           ");
    mvwprintw(titlewin, 4,2,"d8'                                                                         88                                            ");
    mvwprintw(titlewin, 5,2,"88             ,adPPYYba,  88,dPYba,,adPYba,    ,adPPYba,      ,adPPYba,  MM88MMM                                         ");
    mvwprintw(titlewin, 6,2,"88      88888  ''     `Y8  88P'   '88'    '8a  a8P_____88     a8'     '8a   88                                            ");
    mvwprintw(titlewin, 7,2,"Y8,        88  ,adPPPPP88  88      88      88  8PP*******     8b       d8   88                                            ");
    mvwprintw(titlewin, 8,2," Y8a.    .a88  88,    ,88  88      88      88  '8b,   ,aa     '8a,   ,a8'   88         __________________________________ ");
    mvwprintw(titlewin, 9,2,"  `*Y88888P*   `*8bbdP'Y8  88      88      88   `*Ybbd8*'      `*YbbdP*'    88         8888888888888888888888888888888888 ");

    wrefresh(titlewin);

    /* Game mode option functionality */

    while(1){
        for(int o = 0; o < 3; o++){
            if(o == highlight){
                wattron(gamemodewin, A_REVERSE);
            }
            mvwprintw(gamemodewin, o+1, 1, gamemodes[o]);
            wattroff(gamemodewin, A_REVERSE);
        }
        option = wgetch(gamemodewin);

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
            if(highlight == 3){
                highlight = 2;
            }
            break;
        default:
            break;
        }

        if(option == 10){
            if(highlight == 0){
                game_of_life();
                gamemode = 1;
            } else if (highlight == 1){
                game_of_war();
                gamemode = 2;
            } else if (highlight == 2){
                game_of_corona();
                gamemode = 3;
            }
            break;
        }
    }

    wrefresh(gamemodewin);
    getch();
}


void game_of_life(void){

    

    clear();
    

    for(int s = 0; s<yMax; s++){
        mvprintw(s, xMax-30, "##");
    }

    refresh();
    
    /* Init Title */
    WINDOW * titlewin = newwin(12, 125, 5, xMax/2-80);
    wborder(titlewin, 76, 76, 76, 76, 76, 76, 76, 76);

                                //font is univers, created with http://patorjk.com/software/taag/#p=display&f=Univers&t=Game%20of%20Life
    mvwprintw(titlewin, 2,2,"  ,ad8888ba,                                                                 ad88     88           88     ad88            ");
    mvwprintw(titlewin, 3,2," d8'     `'8b                                                               d8'       88           **    d8'              ");
    mvwprintw(titlewin, 4,2,"d8'                                                                         88        88                 88               ");
    mvwprintw(titlewin, 5,2,"88             ,adPPYYba,  88,dPYba,,adPYba,    ,adPPYba,      ,adPPYba,  MM88MMM     88           88  MM88MMM  ,adPPYba, ");
    mvwprintw(titlewin, 6,2,"88      88888  ''     `Y8  88P'   '88'    '8a  a8P_____88     a8'     '8a   88        88           88    88    a8P_____88 ");
    mvwprintw(titlewin, 7,2,"Y8,        88  ,adPPPPP88  88      88      88  8PP*******     8b       d8   88        88           88    88    8PP******* ");
    mvwprintw(titlewin, 8,2," Y8a.    .a88  88,    ,88  88      88      88  '8b,   ,aa     '8a,   ,a8'   88        88           88    88    '8b,   ,aa ");
    mvwprintw(titlewin, 9,2,"  `*Y88888P*   `*8bbdP'Y8  88      88      88   `*Ybbd8*'      `*YbbdP*'    88        88888888888  88    88     `*Ybbd8*' ");

    wrefresh(titlewin);


    

    choose_map();

    choose_seed();

    while(1){

        if(map_choice==1){
            draw_board(76,22);

        } else {
            draw_board(22,22);
        }

        next_step();
        Sleep(500);


    }
    


}

void game_of_war(void){

    clear();
    

    for(int s = 0; s<yMax; s++){
        mvprintw(s, xMax-30, "##");
    }

    refresh();
    
    /* Init Title */
    WINDOW * titlewin = newwin(12, 135, 5, xMax/2-80);
    wborder(titlewin, 87, 87, 87, 87, 87, 87, 87, 87);

                                //font is univers, created with http://patorjk.com/software/taag/#p=display&f=Univers&t=Game%20of%20Life
    mvwprintw(titlewin, 2,2,"  ,ad8888ba,                                                                 ad88     I8,        8        ,8I                      ");
    mvwprintw(titlewin, 3,2," d8'     `'8b                                                               d8'       `8b       d8b       d8'                      ");
    mvwprintw(titlewin, 4,2,"d8'                                                                         88         '8,     ,8'8,     ,8'                       ");
    mvwprintw(titlewin, 5,2,"88             ,adPPYYba,  88,dPYba,,adPYba,    ,adPPYba,      ,adPPYba,  MM88MMM       Y8     8P Y8     8P  ,adPPYYba,  8b,dPPYba,");
    mvwprintw(titlewin, 6,2,"88      88888  ''     `Y8  88P'   '88'    '8a  a8P_____88     a8'     '8a   88          `8b   d8' `8b   d8'  ''     `Y8  88P'   'Y8");
    mvwprintw(titlewin, 7,2,"Y8,        88  ,adPPPPP88  88      88      88  8PP*******     8b       d8   88           `8a a8'   `8a a8'   ,adPPPPP88  88        ");
    mvwprintw(titlewin, 8,2," Y8a.    .a88  88,    ,88  88      88      88  '8b,   ,aa     '8a,   ,a8'   88            `8a8'     `8a8'    88,    ,88  88        ");
    mvwprintw(titlewin, 9,2,"  `*Y88888P*   `*8bbdP'Y8  88      88      88   `*Ybbd8*'      `*YbbdP*'    88             `8'       `8'     `*8bbdP'Y8  88        ");

    wrefresh(titlewin);

    mvprintw(30, xMax/2-8 ,"Work in progress");

}

void game_of_corona(void){

    clear();
    
    for(int s = 0; s<yMax; s++){
        mvprintw(s, xMax-30, "##");
    }

    refresh();
    
    /* Init Title */
    WINDOW * titlewin = newwin(12, 166, 5, xMax/2-95);
    wborder(titlewin, 67, 67, 67, 67, 67, 67, 67, 67);

                                //font is univers, created with http://patorjk.com/software/taag/#p=display&f=Univers&t=Game%20of%20Life
    mvwprintw(titlewin, 2,2,"  ,ad8888ba,                                                                 ad88       ,ad8888ba,                                                                ");
    mvwprintw(titlewin, 3,2," d8'     `'8b                                                               d8'        d8*'    `'8b                                                               ");
    mvwprintw(titlewin, 4,2,"d8'                                                                         88        d8'                                                                         ");
    mvwprintw(titlewin, 5,2,"88             ,adPPYYba,  88,dPYba,,adPYba,    ,adPPYba,      ,adPPYba,  MM88MMM     88              ,adPPYba,   8b,dPPYba,   ,adPPYba,   8b,dPPYba,   ,adPPYYba,");
    mvwprintw(titlewin, 6,2,"88      88888  ''     `Y8  88P'   '88'    '8a  a8P_____88     a8'     '8a   88        88             a8'     '8a  88P'   'Y8  a8'     '8a  88P'   `*8a  ''     `Y8");
    mvwprintw(titlewin, 7,2,"Y8,        88  ,adPPPPP88  88      88      88  8PP*******     8b       d8   88        Y8,            8b       d8  88          8b       d8  88       88  ,adPPPPP88");
    mvwprintw(titlewin, 8,2," Y8a.    .a88  88,    ,88  88      88      88  '8b,   ,aa     '8a,   ,a8'   88         Y8a.    .a8P  '8a,   ,a8'  88          '8a,   ,a8'  88       88  88,    ,88");
    mvwprintw(titlewin, 9,2,"  `*Y88888P*   `*8bbdP'Y8  88      88      88   `*Ybbd8*'      `*YbbdP*'    88          `*Y8888Y*'    `*YbbdP*'   88           `*YbbdP*'   88       88  `*8bbdP'Y8");

    wrefresh(titlewin);

    mvprintw(30, xMax/2-8 ,"Work in progress");

}

void choose_map(void){

    WINDOW *gameplaywin = newwin(40, 150, 20, 20);
    box(gameplaywin, 0, 0);
    wrefresh(gameplaywin);

    char *maps[6] = {"Small Square", "Big Square", "Custom Square(WIP)", "World Map", "Europe", "Finalnd"};
    int option;
    int highlight = 0;
    

    WINDOW * mapwin = newwin(8, 20, 20, xMax-25);
    box(mapwin, 0, 0);
    keypad(mapwin, true);

       while(1){
        for(int o = 0; o < 6; o++){
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
            if(highlight == 6){
                highlight = 5;
            }
            break;
        default:
            break;
        }

        if(option == 10){
            if(highlight == 0){
                map_choice = 1;
                draw_square(1);
            } else if (highlight == 1){
                map_choice = 2;
                draw_square(2);
            } else if (highlight == 2){
                map_choice = 3;
                draw_square(3);
            } else if (highlight == 3){
                map_choice = 4;
                init_map(1);
            } else if (highlight == 4){
                map_choice = 5;
                init_map(2);
            } else if (highlight == 5){
                map_choice = 6;
                init_map(3);
            } 
            break;
        }
    }
}

void init_map(int map_number){

    WINDOW *gameplaywin = newwin(40, 150, 20, 20);
    box(gameplaywin, 0, 0);
    wrefresh(gameplaywin);

    char *map_name;

    switch (map_number)
    {
    case 1:
        map_name = "worldmap.txt";
        break;
    case 2:
        map_name = "europe.txt";
        break;
    case 3:
        map_name = "finland.txt";
        break;
    default:
        break;
    }

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);

    char line[145];
    FILE *file_pointer;

    file_pointer = fopen(map_name, "r");
    int a = 1;

    while(fgets(line, sizeof line, file_pointer) != NULL){
        wmove(gameplaywin, a, 1);
        j=0;
        for(int i=0; i<sizeof line; i++){
            if(line[i]=='~'){
                map[j][i] = '~';
                wattron(gameplaywin, COLOR_PAIR(1));
                wprintw(gameplaywin, "%c", line[i]);
                wattroff(gameplaywin, COLOR_PAIR(1));
                map[j][i] = '~';
            } else {
                map[j][i] = ' ';
                wattron(gameplaywin, COLOR_PAIR(2));
                wprintw(gameplaywin, "%c", line[i]);
                wattroff(gameplaywin, COLOR_PAIR(2));
                map[j][i] = ' ';
            }
            wrefresh(gameplaywin);
            j++;
        }
            
            a++;
    }
    
    fclose(file_pointer);

    box(gameplaywin, 0, 0);
    wrefresh(gameplaywin);
//    getch();
}

void draw_square(int size){

    int center=0;

    WINDOW *gameplaywin = newwin(40, 150, 20, 20);
    box(gameplaywin, 0, 0);
    wrefresh(gameplaywin);

    WINDOW * sizewin = newwin(8, 20, 20, xMax-25);
    box(sizewin, 0, 0);

    start_color();
    init_pair(2, COLOR_BLUE, COLOR_WHITE);

    if(size == 1){
        ySize = 20;
        xSize = 40;
        center = 55;
    } else if (size == 2){
        ySize = 38;
        xSize = 148;
        center = 1;
    } else if (size == 3){
        refresh();
        mvwprintw(sizewin, 1, 1, "Input height:");
        wrefresh(sizewin);
        wscanw(sizewin, "%d", ySize);
        wgetch(sizewin);
        wrefresh(sizewin);
        mvwprintw(sizewin, 1, 1, "Input width:");
        wrefresh(sizewin);
        wscanw(sizewin, "%d", xSize);
    }
        wmove(gameplaywin, 1, 1);

        for(i=0; i<ySize; i++){
            for(j=0; j<xSize; j++){
                wattron(gameplaywin, COLOR_PAIR(2));
                mvwprintw(gameplaywin, i+1,j+center, " ");  
                wrefresh(gameplaywin);  
                wattroff(gameplaywin, COLOR_PAIR(2));
            }
            wprintw(gameplaywin, "\n");
        }
        box(gameplaywin, 0, 0);
        wrefresh(gameplaywin);
        refresh();
        getch();

    
}

void choose_seed(void){

    WINDOW *gameplaywin = newwin(40, 150, 20, 20);
    box(gameplaywin, 0, 0);
//    wrefresh(gameplaywin);


    WINDOW *seedwin = newwin(8, 20, 20, xMax-25);
    box(seedwin, 0, 0);
    wrefresh(seedwin);

    int option;
    int highlight = 0;
    char *seeds[2] = {"Predetermined seed", "Random seed"};
    keypad(seedwin, true);

    while(1){
        for(int o = 0; o < 2; o++){
            if(o == highlight){
                wattron(seedwin, A_REVERSE);
            }
            mvwprintw(seedwin, o+1, 1, seeds[o]);
            wattroff(seedwin, A_REVERSE);
    }
    option = wgetch(seedwin);

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
            init_seed(1);
        } else if (highlight == 1){
            init_seed(2);
        }
        break;
    }
}
}

void init_seed(int seed){

    WINDOW *gameplaywin = newwin(40, 150, 20, 20);
    box(gameplaywin, 0, 0);
//    wrefresh(gameplaywin);

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);




    if(seed == 1){ //predetermined
        if(map_choice == 1){ //small square

            board[1][1].current = 1;
            board[22][22].current = 1;
            board[25][25].current = 1;
            board[50][50].current = 1;
            board[50][30].current = 1;
            board[4][3].current = 1;
            board[4][4].current = 1;
            board[11][12].current = 1;
            board[12][12].current = 1;
            board[13][10].current = 1;
            board[13][13].current = 1;
            board[13][14].current = 1;
            board[14][13].current = 1;
            board[14][14].current = 1;

        } else if(map_choice == 2){

            board[1][1].current = 1;
            board[22][22].current = 1;
            board[25][25].current = 1;
            board[50][50].current = 1;
            board[50][30].current = 1;
            board[4][3].current = 1;
            board[4][4].current = 1;
            board[11][12].current = 1;
            board[12][12].current = 1;
            board[13][10].current = 1;
            board[13][13].current = 1;
            board[13][14].current = 1;
            board[14][13].current = 1;
            board[50][50].current = 1;
            board[40][50].current = 1;
            board[30][50].current = 1;
            board[20][50].current = 1;
            board[50][20].current = 1;
            board[50][30].current = 1;
            board[50][40].current = 1;
            board[50][50].current = 1;
        }
    } else if (seed == 2){ //random int r = rand()%2;
        if(map_choice<=3){
            for(i=0; i<60; i++){
                for(j=0; j<170; j++){
                    int r = rand()%2;
                    board[i][j].current = r;
                }
            }
        } else if (map_choice>=4){
            for(i=0; i<60; i++){
                for(j=0; j<170; j++){

                    int r = rand()%2;
                    board[i][j].current = r;
                    /*
                    if(map[i][j]=='~'){
                        wattron(gameplaywin, COLOR_PAIR(1));
                        mvwprintw(gameplaywin, i, j, "~");
                        wattroff(gameplaywin, COLOR_PAIR(1));
                        Sleep(500);
                    } else {
//                        int r = rand()%2;
//                        board[i][j].current = r;
                        if(board[i][j].current = 1){
                            wattron(gameplaywin, COLOR_PAIR(3));
                            mvwprintw(gameplaywin, i, j, "x");
                            wattroff(gameplaywin, COLOR_PAIR(3));
                            wrefresh(gameplaywin);
                        } else {
                            wattron(gameplaywin, COLOR_PAIR(2));
                            mvwprintw(gameplaywin, i, j, " ");
                            wattroff(gameplaywin, COLOR_PAIR(2));
                            wrefresh(gameplaywin);
                        }
                    }
                    //int r = rand()%2;
                    //board[i][j].current = r;*/
                }
            }
            for(i=0; i<60; i++){
                for(j=0; j<170; j++){
                    if(map[i][j]=='~'){
                        wattron(gameplaywin, COLOR_PAIR(1));
                        /*mv*/wprintw(gameplaywin,/* i, j,*/ "~");
                        wattroff(gameplaywin, COLOR_PAIR(1));
                        Sleep(20);
                    } else {
                        if(board[i][j].current == 0){
                            wattron(gameplaywin, COLOR_PAIR(3));
                            /*mv*/wprintw(gameplaywin,/* i, j,*/ "x");
                            wattroff(gameplaywin, COLOR_PAIR(3));
                            wrefresh(gameplaywin);
                            
                        } else {
                            wattron(gameplaywin, COLOR_PAIR(2));
                            /*mv*/wprintw(gameplaywin,/* i, j,*/ " ");
                            wattroff(gameplaywin, COLOR_PAIR(2));
                            wrefresh(gameplaywin);
                            Sleep(20);
                        }
                    }
                }
            }
        }
        wrefresh(gameplaywin);
    }

}