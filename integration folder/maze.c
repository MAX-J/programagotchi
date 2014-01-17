/*#include "display.h"*/
#include "./programagotchi.h"

#define GOLEFT 0
#define GORIGHT 1
#define CHANGEDIRECTION (direction+ 1)%2
#define COMPLETED 0

int moveBaddies(char gamearray[HEIGHT][WIDTH], int direction, SDL_Simplewin gamewin);

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int direction, SDL_Simplewin gamewin);

int playMaze(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	
	int direction = GOLEFT, firstmove = 1, score = 0;
	int state = 0;
	char str[STRLEN];
	
	SDL(gamearray, "Welcome to Sewer City", NO_SCORE, gamewin);

	SDL_Delay(1000);

	SDL(gamearray, "Try moving your Gotchi by using the 'move' command", NO_SCORE, gamewin);
	  
	do{
		do{
			printf("Enter Command:");
			
			fgets(str,STRLEN,stdin);
			
			if(str[0] == '\n'){
				state = BAD_COMMAND;
			}

			else{
				state = runcommand(gamewin, gamearray, str);
			
				if(state == ATE_CANDY){
				SDL(gamearray, "You got candy",score++, gamewin);			
				state = NO_ACTION;
			}

			if(state == ON_EXIT){
				SDL(gamearray, "Level Completed. Congratulations",score, gamewin);
				SDL_Delay(1000);
				return WIN;
			}

			else if(state == ON_HAZARD){
				SDL(gamearray, "You died",  score,gamewin);
				SDL_Delay(1000);
				return LOSE;
			}
			}

		}while(state == BAD_COMMAND);
		if(firstmove){
			SDL(gamearray, "You did it. Congratulations",score, gamewin);
			firstmove = COMPLETED;		
		}		

		if(state == QUIT_COMMAND){
			SDL(gamearray, "Exiting Game",NO_SCORE, gamewin);
			SDL_Delay(1000);
			return LOSE;
		}
		
		else if(state == ON_EXIT){
			SDL(gamearray, "Level Completed. Congratulations",score, gamewin);
			SDL_Delay(1000);
			return WIN;
		}
		
		else if(state == ON_HAZARD){
			SDL(gamearray, "You died",NO_SCORE, gamewin);
			SDL_Delay(1000);
			return LOSE;
		}
		
		if(direction == 0 || direction == 1){
			direction = moveBaddies(gamearray, direction, gamewin);
		}
		
		if(direction == ON_HAZARD){
			SDL(gamearray, "You died", NO_SCORE, gamewin);
			SDL_Delay(1000);
			return LOSE;
		}
		
		SDL(gamearray, "Type in next move", score, gamewin);
		
		Neill_SDL_Events(&gamewin);
	
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
	
}

int moveBaddies(char gamearray[HEIGHT][WIDTH], int direction, SDL_Simplewin gamewin){
	int i,j;
	
	for(j = 0; j < HEIGHT; j++){
		for(i = 0; i < WIDTH; i++){
			if(gamearray[i][j] == HAZARD){
				j = moveHorizontally(gamearray, i , j, direction, gamewin);
				i++;
			}
			if( j < 0){
				return ON_HAZARD;
			}
		}
	}
	direction = CHANGEDIRECTION;
	
	return direction;
}

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int direction, SDL_Simplewin gamewin){
	int count = 0;
		if((gamearray[i][j - 1] == GOTCHI || gamearray[i][j - 1] == FREE_SPACE) && direction == GOLEFT){
			do{
				if(gamearray[i][j - 1] == GOTCHI){
					return -5; 
				}
				if(gamearray[i][j - 1] != FREE_SPACE){
					return j;
				}	
				gamearray[i][j] = FREE_SPACE;
				gamearray[i][j-1] = HAZARD;
				/*SDL(gamearray, "", NO_SCORE,gamewin);*/
				j--;
				count++;
			}while(count != 5);
		}
		
	else if((gamearray[i][j + 1] == GOTCHI || gamearray[i][j + 1] == FREE_SPACE) && direction == GORIGHT){
		do{
			if(gamearray[i][j + 1] == GOTCHI){
				return -5; 
			}
			if(gamearray[i][j + 1] != FREE_SPACE){
				return j;
			}

			gamearray[i][j] = FREE_SPACE;
			gamearray[i][j+1] = HAZARD;	
			/*SDL(gamearray, "", NO_SCORE, gamewin);*/
			j++;
			count++;
		}while(count!= 5);
	}

	return j;
}
				
