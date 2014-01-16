#include "./programagotchi.h"

int playJump(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	
	int score = 0;
	int state = 0;
	char str[STRLEN];
	char command;
      
	SDL(gamearray, "Let's Jump",  NO_SCORE,gamewin);
	SDL_Delay(1000);
        SDL(gamearray,"G for play the game, E for exit game",  NO_SCORE,gamewin);
        	scanf("%c",&command);
        	if(command == 'E'){
	  		EXIT;
       		 }
        	else if(command == 'G'){
	  		SDL(gamearray, "Type in next move, try to get candies ",NO_SCORE,gamewin);
		}
	
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
					SDL(gamearray,"You got candy,you need 5 candies to win",score+=10,gamewin);
					state = NO_ACTION;
	      			}
	      			if(score == 50){
					SDL(gamearray, "Level Completed. Congralutations", NO_SCORE, gamewin);
					SDL_Delay(1000);
					return WIN;
	      			}
				else if(state == ON_HAZARD){
					SDL(gamearray, "You died",  score,gamewin);
					SDL_Delay(1000);
					return LOSE;
				}
				else if(state == QUIT_COMMAND){
					SDL(gamearray, "Exiting Game", NO_SCORE, gamewin);
					SDL_Delay(1000);
	        			return LOSE;
				} 
			
	    		}  
	}while(state != BAD_COMMAND);

	if(state == QUIT_COMMAND){
		SDL(gamearray, "Exiting Game", NO_SCORE, gamewin);
		SDL_Delay(1000);
	        return LOSE;
	} 	
	
	else if(state == ON_HAZARD){
		SDL(gamearray, "You died",  score,gamewin);
		SDL_Delay(1000);
		return LOSE;
	}
	
	else if(score == 50){
	        SDL(gamearray, "Level Completed. Congralutations", NO_SCORE, gamewin);
		SDL_Delay(1000);
	        return WIN;
	}
	  
	SDL(gamearray, "Type in next move,try to get candies",NO_SCORE,gamewin);
	Neill_SDL_Events(&gamewin);
	    
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
}	


