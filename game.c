/** @file game.c */

#include "newgame.h"

/*********************************************************************/
/**
 \fn void next_step(int map[200][200], struct cell board[60][200], int rounds, int mapchoice)
 \brief takes the next step in the game. prints the board according to the current status of cells
	\param int map[200][200]
    \param struct cell board[60][200]
    \param int rounds
    \param int mapchoice
	\return void
 */
/*********************************************************************/

void next_step(int map[200][200], struct cell board[60][200], int rounds, int mapchoice){
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

    infect(rounds, board); //set the random infection

    mvprintw(1,1,"%d", rounds);
    mvprintw(3,1,"Press x to exit");

    refresh();
    
}

/*********************************************************************/
/**
 \fn int calculate_future(int i, int j, int map[200][200], struct cell board[60][200], int mapchoice)
 \brief calculates the future status of cells, according to the rules set in game of life. also added the functionality for the infection.
	\param int i
    \param int j
    \param int map[200][200]
    \param struct cell board[60][200]
    \param int mapchoice
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
 \fn void infect(int rounds, struct cell board[60][200])
 \brief sets infections depending on the round
	\param int rounds
    \param struct cell board[60][200]
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