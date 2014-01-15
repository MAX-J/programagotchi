#include "./programagotchi.h"

int gameturn(char gamearray[HEIGHT][WIDTH], int score, int rowshift,int colshift,SDL_Simplewin gamewin);
int gamestate(char gamearray[HEIGHT][WIDTH],int score,int rowshift,int colshift,SDL_Simplewin gamewin);
int playJump(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	
	int score = 0;
	int state = 0;
	int rowshift = 0;
	int colshift = 0;
	char str[STRLEN];
	
	SDL(gamearray, "Welcome to Jump",  NO_SCORE,gamewin);
	  
	do{
		do{
			printf("Enter Command:");
			fgets(str,STRLEN,stdin);
	
			if(str[0] == '\n') {
				state = BAD_COMMAND;
			}
			else{
				state = runcommand(gamewin,gamearray,str);
				state = 0;
			}
		}while(state != BAD_COMMAND);
		
		if(state == QUIT_COMMAND){
			SDL(gamearray, "Exiting Game", NO_SCORE, gamewin);
			return LOSE;
		}
		
		else if(state == ON_EXIT){
			SDL(gamearray, "Level Completed. Congralutations", NO_SCORE, gamewin);
			return WIN;
		}
		
		else if(state == ON_HAZARD){
			SDL(gamearray, "game over",  NO_SCORE,gamewin);
			return LOSE;
		}
		SDL(gamearray, "Type in next move", NO_SCORE, gamewin);
		
		if(score != 100 ) {
			score = gameturn(gamearray,score,rowshift,colshift,gamewin);	
 		}
		
		if(score == 100 ) {
                        SDL(gamearray, "You win", NO_SCORE, gamewin);
                        return WIN;
                }
		SDL(gamearray, "",  NO_SCORE,gamewin);
		
		Neill_SDL_Events(&gamewin);
		
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
}	

int gameturn(char gamearray[HEIGHT][WIDTH], int score, int rowshift,int colshift,SDL_Simplewin gamewin) {
 	int j = 0;
        int k = 0;
        char command;
      
        SDL(gamearray,"G for take a turn E for exit game",  NO_SCORE,gamewin);
        scanf("%c",&command);
        if(command == 'E') {
                    EXIT;
        }
        else if(command == 'G') {
        	SDL(gamearray, "Type in next move",NO_SCORE,gamewin);
		k = gamestate(gamearray,score,rowshift,colshift,gamewin);
		if(k == 0) {
                	for( j = 0;j < HEIGHT;j++) {
                        	SDL(gamearray,"%2d:%s\n",j,gamewin);
                        }
                        SDL(gamearray,"Your score is: ",score,gamewin);
                }
		else if( k == -1) {
                        getchar();
			EXIT;
		}
		getchar();
        } 
	SDL(gamearray, "Game over!",  NO_SCORE,gamewin);
        SDL(gamearray, "Your score is: ",score,gamewin);
        return score;
}


int gamestate(char gamearray[HEIGHT][WIDTH],int score,int rowshift,int colshift,SDL_Simplewin gamewin) {

        char laststatus;
	int row = 0;
	int col = 0;
        int newrow = row + rowshift;
        int newcol = col + colshift;
        
        if( newrow < HEIGHT &&  newcol < WIDTH 
        && newrow >= 1 &&  newcol >= 1)
        {
                if(gamearray[newrow][newcol] == 'C')
                {
                        score += 10;
                        gamearray[newrow][newcol] = 'G';
                        gamearray[row][col] = laststatus;
                        SDL(gamearray,"You get a candy", NO_SCORE,gamewin);
                }
                else if(gamearray[newrow][newcol] == '1' ||
                        gamearray[newrow][newcol] == '#')
                {
                        SDL(gamearray,"Ouch!you meet a barrier!", NO_SCORE,gamewin);
                        SDL(gamearray,"Please input a valid value!", NO_SCORE,gamewin);
                        return 1;
                }
		else if(gamearray[newrow][newcol] == 'X')
		{
			SDL(gamearray,"oh my god!you jump to a hole!", NO_SCORE,gamewin);
			return -1;
		}
                else if(gamearray[newrow][newcol] == '.')
                {
                        gamearray[row][col] = laststatus;
                        laststatus = '.';
                        gamearray[newrow][newcol] = 'G';
                }
                else
                {
                        gamearray[row][col] = laststatus;
                        laststatus = gamearray[newrow][newcol];
                        gamearray[newrow][newcol] = 'G';
                }
                row = newrow;
                col = newcol;
        }
        else
        {
                SDL(gamearray,"Oops!you are trying to jump out of the wall!", NO_SCORE,gamewin);
        }
        return 0;
}
