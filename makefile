newgame: main.o init.o game.o
	gcc -o Game_of_Corona main.o init.o game.o -lncurses
main.o: main.c 
	gcc -c main.c
init.o: init.c 
	gcc -c init.c
game.o: game.c 
	gcc -c game.c
clean:
	rm game.o main.o init.o