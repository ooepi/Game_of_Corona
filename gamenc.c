#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <windows.h>

struct cell{
    int current;
    int future;
};

void init_board(void);
void draw_board(void);
void game_setup(void);
void next_step(void);
int calculate_neighbors(int i, int j);
void print_curses(void);

struct cell board[10][10] = {0};



/*VARIABLES*/

int x, y, i, j;

/*START OF MAIN*/

int main(void){


    x=7;
    y=7;

    init_board();

    game_setup();

    print_curses();

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

void draw_board(){

    move(0,0);

    for(i=0; i<y; i++){
        for(j=0; j<x; j++){
            if(board[i][j].current == 1){
                attron(COLOR_PAIR(1));
                printw(" ");    //if value is 1, print the cell
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                printw(" ");    //otherwise print empty
                attroff(COLOR_PAIR(2));
            }
        }
        printw("\n");
    }
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


    for(i=0; i<y; i++){
        for(j=0; j<x; j++){ //setting the future state with following function
            board[i][j].future = calculate_neighbors(i,j);
        }
    }



    for(i=0; i<y; i++){
        for(j=0; j<x; j++){ //copying the future board on the current
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
