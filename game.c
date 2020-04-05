#include <stdio.h>
#include <unistd.h>

struct cell{
    int current;
    int future;
};

void init_board(void);
void draw_board(void);
void game_setup(void);
void next_step(void);
int calculate_neighbors(int i, int j);

struct cell board[10][10] = {0};



/*VARIABLES*/

int x, y, i, j;

/*START OF MAIN*/

int main(void){


    x=7;
    y=7;

    init_board();

    game_setup();

    draw_board();
    printf("\n");
    
    next_step();
        printf("\n\n");
    draw_board();
    printf("\n");

    next_step();
        printf("\n\n");
    draw_board();
    printf("\n");

    next_step();
        printf("\n\n");
    draw_board();
    printf("\n");


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

    for(i=0; i<y; i++){
        for(j=0; j<x; j++){
            if(board[i][j].current == 1){
                printf("o");    //if value is 1, print the cell
            } else {
                printf(".");    //otherwise print empty (".")
            }
        }
        printf("\n");
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

