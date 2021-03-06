#include "./programagotchi.h"
#include "./maze.h"

int playJump(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin){
	
	int score = 0;
	int state = 0;
	int gravity_applied = 0; /*--JT EDIT--*/
	char str[STRLEN];
      
	SDL(gamearray, "Welcome to the jump game,collect all of candies to win!\nYou need to write a 'jump' function.\n"
	"Try editing 'jump.gfn' in the text editor.\nType 'help' to get more information.\n\n",NO_SCORE,5000,gamewin);
	SDL(gamearray, "Type the first move,try to get candies",NO_SCORE,0,gamewin); 	
	do{
		
	  	do{

			/*----JT EDIT --- added a switch to apply 'gravity' after each runcommand-----*/
			/*-- moved 'Enter Command', '\n' catch and RunCommand into the switch--*/
			if (gravity_applied == 0) {
				state = apply_gravity(gamearray,gamewin);
				/*if any 'collision' on way down, run another gravity loop,
				 *otherwise move on to the next 'run command'*/
				if (state == NO_ACTION) {
					gravity_applied = 1;
				}
			}
			else {
				printf("Enter Command:");
				fgets(str,STRLEN,stdin);
				if(str[0] == '\n') {
					state = BAD_COMMAND;
				} else {
					state = runcommand(gamewin,gamearray,str);
				}
				gravity_applied = 0;
			}
			/*--------------------------------------------------------------------------*/
			
			if(state == ATE_CANDY){
				SDL(gamearray,"You got candy,you need 5 candies to win",score+=10,1000,gamewin);
				state = NO_ACTION;
			}
			if(score == 100){
				SDL(gamearray, "Level Completed. Congralutations", NO_SCORE,1000,gamewin);
				displayWinscreen(gamearray,gamewin);
				return score;
			}
			else if(state == ON_HAZARD){
				SDL(gamearray, "You died",  score,1000,gamewin);
				displayDeathscreen(gamearray, gamewin);
				return LOSE;
			}
			else if(state == QUIT_COMMAND){
				SDL(gamearray, "Exiting Game", NO_SCORE,1000,gamewin);
				return LOSE;
			} 
			
		SDL(gamearray, "Type in next move,try to get candies",NO_SCORE,10,gamewin); 
	}while(state != BAD_COMMAND);

	if(state == QUIT_COMMAND){
		SDL(gamearray, "Exiting Game", NO_SCORE,1000, gamewin);
	        return LOSE;
	} 	
	
	else if(state == ON_HAZARD){
		SDL(gamearray, "You died",  score,1000,gamewin);
		displayDeathscreen(gamearray, gamewin);
		return LOSE;
	}
	
	else if(score == 100){
	        SDL(gamearray, "Level Completed. Congralutations", NO_SCORE,1000,gamewin);
		displayWinscreen(gamearray,gamewin);
	        return score;
	}
	  
	SDL(gamearray, "Type in next move,try to get candies",NO_SCORE,1000,gamewin);
	    
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
}	


