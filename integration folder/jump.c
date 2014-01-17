#include "./programagotchi.h"

int playJump(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	
	int score = 0;
	int state = 0;
	char str[STRLEN];
      
	SDL(gamearray, "Welcome to the jump game,collect all of candies to win!\n\nYou need to write a 'move' or 'jump' function.\n\n"
	"Try editing 'jump.gsn' in the text editor.\n\nType 'help' to get more information.\n\n",NO_SCORE,5000,gamewin);
	SDL(gamearray, "Type the first move,try to get candies",NO_SCORE,1000,gamewin); 	
	do{
		
	  	do{
	    		printf("Enter Command:");
	    		fgets(str,STRLEN,stdin);
	    
	   		if(str[0] == '\n') {
	       			state = BAD_COMMAND;
	        	}
	       		else{
	      			state = runcommand(gamewin,gamearray,str);
	      			printf("%d",state);
	      			if(state == ATE_CANDY){
					SDL(gamearray,"You got candy,you need 5 candies to win",score+=10,1000,gamewin);
					state = NO_ACTION;
	      			}
	      			if(score == 100){
					SDL(gamearray, "Level Completed. Congralutations", NO_SCORE,1000,gamewin);
					return WIN;
	      			}
				else if(state == ON_HAZARD){
					SDL(gamearray, "You died",  score,1000,gamewin);
					return LOSE;
				}
				else if(state == QUIT_COMMAND){
					SDL(gamearray, "Exiting Game", NO_SCORE,1000,gamewin);
	        			return LOSE;
				} 
	    		} 
		SDL(gamearray, "Type in next move,try to get candies",NO_SCORE,1000,gamewin); 
	}while(state != BAD_COMMAND);

	if(state == QUIT_COMMAND){
		SDL(gamearray, "Exiting Game", NO_SCORE,1000, gamewin);
	        return LOSE;
	} 	
	
	else if(state == ON_HAZARD){
		SDL(gamearray, "You died",  score,1000,gamewin);
		return LOSE;
	}
	
	else if(score == 100){
	        SDL(gamearray, "Level Completed. Congralutations", NO_SCORE,1000,gamewin);
	        return WIN;
	}
	  
	SDL(gamearray, "Type in next move,try to get candies",NO_SCORE,1000,gamewin);
	Neill_SDL_Events(&gamewin);
	    
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
}	


