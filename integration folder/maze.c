/*#include "display.h"*/
#include "./programagotchi.h"

int moveBaddies(char gamearray[HEIGHT][WIDTH], int counter);

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int counter);

int playMaze(char gamearray[HEIGHT][WIDTH], SDL_Simplewin sw){
	
	int counter = 0;
	int state = 0;
	char str[STRLEN];
	
	SDL(gamearray, "Welcome to Sewer City", gamewin);
	
	do{
		do{
			printf("Enter Command:");
			scanf("%s", &str); 
	
			state = runcommand(gamewin, gamearray, str);
			
		}while(state != -1);
		
		if(state == QUIT_COMMAND){
			SDL(gamearray, "Exiting Game", gamewin);
			return LOSE;
		}
		
		else if(state == ON_EXIT){
			SDL(gamearray, "Level Completed. Congralutations", gamewin);
			return WIN;
		}
		
		else if(state == ON_HAZARD){
			SDL(gamearray, "Level Completed. Congralutations", gamewin);
			return LOSE;
		}

		SDL(gamearray, "Type in next move", gamewin);
		
		counter = moveBaddies(gamearray, counter);
		
		counter++;
		
		SDL(gamearray, "", gamewin);
		
	}while(state == NO_ACTION);
	
	return LOSE;
	
}

int moveBaddies(char gamearray[HEIGHT][WIDTH], int counter){
	int i,j;
	for(j = 0; j < HEIGHT; j++){
		for(i = 0; i < WIDTH; i++){
			if(gamearray[i][j] == 'S'){
				moveHorizontally(gamearray, i , j, counter);
			}
		}
	}
	
	return counter;
}

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int counter){
	
	if(gamearray[i][j - 1] == '.' && counter >= 0){
		gamearray[i][j] = '.';
		gamearray[i][j-1] = 'S';
		counter++;
		if(counter == 5){
			counter = -5;
		}
	}
	
	else if(gamearray[i][j - 1] == '.' && counter < 0){
		gamearray[i][j] = '.';
		gamearray[i][j+1] = 'S';
		counter++;
	}
	return counter;
}
				
