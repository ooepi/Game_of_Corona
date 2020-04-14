#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void map_choice(void);
void seed_choice(void);
void initgame(int mapchoice, int seedchoice);
void next_step(void);
int calculate_future(int i, int j);
void date_and_infection(void);
void infect(int rounds);

struct cell{
    int current;
    int future;
    bool infection;
};

int y,x,i,j,yMax,xMax;
int mapchoice = 0;
int seedchoice = 0;
int day, month, year;
int infected = 0;
int healthy = 0;
int rounds = 0;

struct cell board[60][200] = {0,0};
int map[200][200] = {0,0};


int main (void){

    initscr();
    cbreak();
    curs_set(0);
    noecho();
    nodelay(stdscr, true);

    srand(time(NULL));

    start_color();

    //color 1 = life
    //color 2 = ground
    //color 3 = water
    //color 4 = infection

    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_RED, COLOR_GREEN);


    getmaxyx(stdscr, yMax, xMax);

    

    day = 8;
    month = 12;
    year = 2019;


    map_choice();
//    seed_choice();

    seedchoice = 1;
    initgame(mapchoice, seedchoice);


    while(getch()!='x'){
        next_step();
        Sleep(300);
    }

    getch();
    endwin();
}

void map_choice(void){

    /* Init the game mode menu */
    WINDOW * mapwin = newwin(4, 20, yMax-50, xMax/2-10);
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

    /* Game mode option functionality */

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
                mapchoice = 1;
            } else if (highlight == 1){
                mapchoice = 2;
            }
            break;
        }
    }

    wrefresh(mapwin);
}

void seed_choice(void){

    /* Init the game mode menu */
    WINDOW * seedwin = newwin(4, 20, yMax-50, xMax/2-10);
    box(seedwin, 0, 0);
    char *seeds[2] = {"Random", "Predetermined"};
    int option;
    int highlight = 0;
    
    keypad(seedwin, true);


    /* Game mode option functionality */

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
                seedchoice = 1;
            } else if (highlight == 1){
                seedchoice = 2;
            }
            break;
        }
    }

    wrefresh(seedwin);
}

void initgame(int mapchoice, int seedchoice){

    clear();

    char line[205];
    FILE *file_pointer;

    file_pointer = fopen("worldmaplarge1.txt", "r");
    int a = 0;

    if(seedchoice==1){
        if(mapchoice==1){
            for(i=0; i<59; i++){
                for(j=0; j<200; j++){
                    map[i][j]=1;
                    int r = rand()%2;
                    board[i][j].current = r;                
                }
                refresh();
            }
            
        } else if (mapchoice==2) {

            while(fgets(line, sizeof line, file_pointer) != NULL){
                move(a,0);
                int i=0;
                for(int j=0; j<sizeof line; j++){
                    if(line[j]=='~'){
                        map[a][j] = 0;
                        attron(COLOR_PAIR(3));
                        printw("%c", line[j]);
                        attroff(COLOR_PAIR(3));
                    } else {
                        map[a][j] = 1;
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
                board[i][j].infection = false;
                if(map[i][j]==0){
                    board[i][j].current = 0;
                } else if (map[i][j]==1){
                    int r = rand()%2;
                    board[i][j].current = r;
                }
            }
        }

        //set random infection


/*        for(i=14; i<20; i++){
            for(j=150; j<170; j++){
                if(board[i][j].current==1){
                    if(rand()%100<=50){
                        board[i][j].infection = true;
                    }
                }
            }
        }
*/


        }
    }

    if(seedchoice==2){

    }

    /* NEXT STEP */

}


void next_step(){


    for(i=0; i<59; i++){
        for(j=0; j<200; j++){ //setting the future state with following function
            board[i][j].future = calculate_future(i,j);
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

            infect(rounds);

    rounds++;

    date_and_infection();

    refresh();
    
}

int calculate_future(int i, int j){

    int neighbors = 0;
    int a, b;

    if(map[i][j]==1){

        if(board[i][j].current == 0){ //if the current cell is empty
            for(a=-1; a<2; a++){
                for(b=-1; b<2; b++){ //going through all the nine points around the point
                    if(board[i+a][j+b].infection == true){
                        if(mapchoice==1){
                            if(rand()%100<25){
                                board[i][j].infection = true;
                            }
                        } else {
                            if(rand()%100<50){
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
                    if(board[i+a][j+b].infection == true){
                        if(mapchoice==1){
                            if(rand()%100<20){
                                board[i][j].infection = true;
                            }
                        } else {
                            if(rand()%100<50){
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

void date_and_infection(void){

    int currently_infected = infected;


    for(i=0; i<59; i++){
        for(j=0; j<200; j++){
            if(board[i][j].current==1 && board[i][j].infection == true){
                infected++;
            } 
        }
    }

    for(i=0; i<59; i++){
        for(j=0; j<200; j++){
            if(board[i][j].current==1 && board[i][j].infection == false){
                healthy++;
            } 
        }
    }

    currently_infected = infected - currently_infected;


    if(infected<0){
        infected = 0;
    }



    mvprintw(0,0, "%d.%d.%d", day, month, year);
    mvprintw(1,0, "Infected (cumulative): %d", infected);
//    mvprintw(2,0, "rounds: %d", rounds);
//    mvprintw(3,0, "healthy c: %d", healthy);
    mvprintw(2,0, "Currently infected: %d", currently_infected);
    mvprintw(4,0,"Press x to exit");

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

void infect(int rounds){

if(rounds<=1){
    for(i=14; i<20; i++){
        for(j=150; j<170; j++){
            if(board[i][j].current==1){
                if(rand()%100<=50){
                    board[i][j].infection = true;
                }
            }
        }
    }
} else if (rounds > 30 && rounds < 40){
    for(i=18; i<23; i++){
        for(j=32; j<48; j++){
            if(board[i][j].current==1){
                if(rand()%100<=50){
                    board[i][j].infection = true;
                }
            }
        }
    }
}

}