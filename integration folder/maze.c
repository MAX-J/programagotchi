/*#include "display.h"*/
#include "./programagotchi.h"

#define GOLEFT 0
#define GORIGHT 1
#define CHANGEDIRECTION (direction+ 1)%2
#define COMPLETED 0
#define DEATH SDL(gamearray, "You died",  NO_SCORE, 300, gamewin)
#define WINNER SDL(gamearray, "Level Completed.\nCongratulations",score, 1000, gamewin)
#define ERROR(PHRASE) {fprintf(stderr,"Fatal Error %s occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); exit(2); }

int moveBaddies(char gamearray[HEIGHT][WIDTH], int direction, SDL_Simplewin gamewin);

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int direction, SDL_Simplewin gamewin);

void displayDeathscreen(char gamearray[HEIGHT][WIDTH],  SDL_Simplewin gamewin);

void displayWinscreen(char gamearray[HEIGHT][WIDTH],  SDL_Simplewin gamewin);

int playMaze(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	int direction = GOLEFT, firstmove = 1, score = 0;
	int state = 0;
	char str[STRLEN];
	
	
	SDL(gamearray, "      Welcome to Maze City.\nTry moving your Gotchi by using the 'move' command.\n Feeling more adventurous, try editing the findCandy.gfn", NO_SCORE, 3000, gamewin);
	
	//SDL(gamearray, "Try moving your Gotchi by using the 'move' command", NO_SCORE, gamewin);
	  
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
					score+=10;
					SDL(gamearray, "You got candy",score, 1000, gamewin);			
					state = NO_ACTION;
				}

				if(state == ON_EXIT){
					WINNER;
					displayWinscreen(gamearray,gamewin);
					if(score == 0) {
						return WIN;
					}
					else {
						return score;
					}
				}

				else if(state == ON_HAZARD){
					DEATH;
					displayDeathscreen(gamearray, gamewin);
					return LOSE;
				}
			}

		}while(state == BAD_COMMAND);
		if(firstmove){
			SDL(gamearray, "You did it. Congratulations",score, 200, gamewin);
			firstmove = COMPLETED;		
		}		

		if(state == QUIT_COMMAND){
			SDL(gamearray, "Exiting Game",NO_SCORE, 300, gamewin);
			return LOSE;
		}
		
		else if(state == ON_EXIT){
			WINNER;
			displayWinscreen(gamearray,gamewin);
			return WIN;
		}
		
		else if(state == ON_HAZARD){
			DEATH;
			displayDeathscreen(gamearray, gamewin);
			return LOSE;
		}
		
		if(direction == 0 || direction == 1){
			direction = moveBaddies(gamearray, direction, gamewin);
		}
		
		if(direction == ON_HAZARD){
			DEATH;
			displayDeathscreen(gamearray, gamewin);
			return LOSE;
		}		
		
		SDL(gamearray, "Type in next move", score, 1000,gamewin);
	
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
			if(j == ON_HAZARD){
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
					return ON_HAZARD; 
				}
				if(gamearray[i][j - 1] != FREE_SPACE){
					return j;
				}	
				gamearray[i][j] = FREE_SPACE;
				gamearray[i][j-1] = HAZARD;
				SDL(gamearray, "", NO_SCORE, 0,gamewin);
				j--;
				count++;
			}while(count != 5);
		}
		
	else if((gamearray[i][j + 1] == GOTCHI || gamearray[i][j + 1] == FREE_SPACE) && direction == GORIGHT){
		do{
			if(gamearray[i][j + 1] == GOTCHI){
				return ON_HAZARD; 
			}
			if(gamearray[i][j + 1] != FREE_SPACE){
				return j;
			}

			gamearray[i][j] = FREE_SPACE;
			gamearray[i][j+1] = HAZARD;	
			SDL(gamearray, "", NO_SCORE, 0, gamewin);
			j++;
			count++;
		}while(count!= 5);
	}

	return j;
}

void displayDeathscreen(char gamearray[HEIGHT][WIDTH],  SDL_Simplewin gamewin)
{
	SDL_Rect stringback;
	char string[10] = "GAME OVER";
    int m, n;
	int stringx = (WWIDTH/2)-strlen(string);
	int stringy = WHEIGHT/2;//80/2;
	
	stringback.w = WWIDTH;
  	stringback.h = 80;
    for (m = 0; m < HEIGHT; m++) {
		for (n = 0; n < WIDTH; n++) {
	    	gamearray[m][n] = HAZARD;
		}
		SDL(gamearray, "You died",  NO_SCORE, 0, gamewin);	
    }
	
	SDL(gamearray, "        ",  NO_SCORE, 0, gamewin);

	SDL_Delay(1000);	

    stringback.x = WWIDTH/3;
    stringback.y = WHEIGHT/2;
    SDL_RenderFillRect(gamewin.renderer, &stringback);
    Neill_SDL_SetDrawColour(&gamewin, 255, 255, 255);
    Neill_SDL_DrawText(&gamewin, string, stringx, stringy);

	SDL_Delay(1000);
	
	SDL_RenderPresent(gamewin.renderer);
    SDL_UpdateWindowSurface(gamewin.win);

	SDL_Delay(1000);
}

void displayWinscreen(char gamearray[HEIGHT][WIDTH],  SDL_Simplewin gamewin){

	SDL_Rect stringback;
	char string[200] = "           CONGRATULATIONS!!!\n                 YOU WON!\n You have tamed your Gotchi to the next level";
    int m, n;
	int stringx = (WWIDTH/2)-1.5*strlen(string);
	int stringy = WHEIGHT/2;
	
	stringback.w = WWIDTH;
  	stringback.h = 80;
    for (m = 0; m < HEIGHT; m++) {
		for (n = 0; n < WIDTH; n++) {
	    	gamearray[m][n] = EXIT_POINT;
		}
			
    }
	SDL(gamearray, "YOU WON!",  NO_SCORE, 0, gamewin);	
	
	SDL_Delay(1000);

	SDL(gamearray, "        ",  NO_SCORE, 0, gamewin);

	SDL_Delay(1000);	

    stringback.x = WWIDTH/3;
    stringback.y = WHEIGHT/2;
    SDL_RenderFillRect(gamewin.renderer, &stringback);
    Neill_SDL_SetDrawColour(&gamewin, 255, 255, 255);
    Neill_SDL_DrawText(&gamewin, string, stringx, stringy);

	SDL_Delay(1000);
	
	SDL_RenderPresent(gamewin.renderer);
    SDL_UpdateWindowSurface(gamewin.win);

	SDL_Delay(1000);

}



				
