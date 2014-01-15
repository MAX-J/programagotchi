/*#include "display.h"*/
#include "./programagotchi.h"

int moveBaddies(char gamearray[HEIGHT][WIDTH], int counter, SDL_Simplewin gamewin);

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int counter, SDL_Simplewin gamewin);

int playMaze(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	
	int counter = 0;
	int state = 0;
	char str[STRLEN];
	
	SDL(gamearray, "Welcome to Sewer City", NO_SCORE,gamewin);
	  
	do{
		do{
			printf("Enter Command:");
			
			fgets(str,STRLEN,stdin);
			
			if(str[0] == '\n'){
				state = BAD_COMMAND;
			}
			else{
				state = runcommand(gamewin, gamearray, str);
				state = 0; /* Will be removed in future */
			}
		}while(state == BAD_COMMAND);
		
		if(state == QUIT_COMMAND){
			SDL(gamearray, "Exiting Game",NO_SCORE, gamewin);
			return LOSE;
		}
		
		else if(state == ON_EXIT){
			SDL(gamearray, "Level Completed. Congralutations",NO_SCORE, gamewin);
			return WIN;
		}
		
		else if(state == ON_HAZARD){
			SDL(gamearray, "You died",NO_SCORE, gamewin);
			return LOSE;
		}
		
		SDL(gamearray, "Type in next move", NO_SCORE, gamewin);
		
		if(counter == 0 || counter == 1){
			counter = moveBaddies(gamearray, counter, gamewin);
		}
		
		if(counter == ON_HAZARD){
			SDL(gamearray, "You died", NO_SCORE, gamewin);
			return LOSE;
		}
		
		SDL(gamearray, "", NO_SCORE, gamewin);
		
		Neill_SDL_Events(&gamewin);
	
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
	
}

int moveBaddies(char gamearray[HEIGHT][WIDTH], int counter, SDL_Simplewin gamewin){
	int i,j;
	
	for(j = 0; j < HEIGHT; j++){
		for(i = 0; i < WIDTH; i++){
			if(gamearray[i][j] == 'S'){
				j = moveHorizontally(gamearray, i , j, counter, gamewin);
				i++;
			}
			if( j < 0){
				return ON_HAZARD;
			}
		}
	}
	counter = (counter+ 1)%2;
	
	return counter;
}

int moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j, int counter, SDL_Simplewin gamewin){
	int count = 0;
		if((gamearray[i][j - 1] == 'G' || gamearray[i][j - 1] == '.') && counter == 0){
			do{
				if(gamearray[i][j - 1] == 'G'){
					return -5; 
				}	
				gamearray[i][j] = '.';
				gamearray[i][j-1] = 'S';
				SDL(gamearray, "", NO_SCORE,gamewin);
				j--;
				count++;
			}while(count != 5);
		}
		
	else if((gamearray[i][j + 1] == 'G' || gamearray[i][j + 1] == '.') && counter == 1){
		do{
			if(gamearray[i][j + 1] == 'G'){
				return -5; 
			}
			gamearray[i][j] = '.';
			gamearray[i][j+1] = 'S';	
			SDL(gamearray, "", NO_SCORE, gamewin);
			j++;
			count++;
		}while(count!= 5);
	}

	return j;
}

/*void Incubator(Display *d)
{
  d->fg = SDL_LoadBMP("./exit.bmp");
  paint(d, 150, 200);
}*/

				
