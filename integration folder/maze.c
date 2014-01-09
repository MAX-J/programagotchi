/*#include "display.h"*/
#include "./programagotchi.h"



/* Take SDL window and empty array from AY */
/* Use SDL functions in own function to use graphics */
/* Make header file containing pointer to 2d array and SDL window */

/* Opens file, creates empty array, stores level in array, returns pointer to array */
char** loadLevel(FILE **ifp, int level);

/* Called by loadLevel, creates an empty 110 wide x 70 high array using calloc*/ 
char** createEmptystate (int width, int height);

/* Called by loadLevel, store game state from file into array */
void storeGamestate(FILE **ifp, char** gamestate);

void showGamestate(char gamearray[HEIGHT][WIDTH]);

void moveBaddies(char gamearray[HEIGHT][WIDTH], int counter);

void moveHorizontally(char gamearray[HEIGHT][WIDTH], int i, int j);

void checkExit(char gamearray[HEIGHT][WIDTH]);

void playMaze(char gamearray[HEIGHT][WIDTH], SDL_Simplewin sw){
	
	int counter = 0;
	int state = 0;
	char str[STRLEN];
	
	SDL(gamearray, "Welcome to Sewer City", gamewin);
	
	do{
		do{
			printf("Enter Command:");
			scanf("%s", &str); 
	
			state = runcommand(gamewin, displaygrid, str);
			
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
				moveHorizontally(gamearray, i , j);
			}
		}
	}
	
	return counter;
}

int checkExit(char gamearray[HEIGHT][WIDTH]){
	
	int i, j;
	
	for(j = 0; j < HEIGHT; j++){
		for(i = 0; i < WIDTH; i++){
			if(gamearray[i][j] == 'E' && gamearray[i-1][j] =='){
				moveHorizontally(gamearray, i , j);
			}
		}
	}
	
	return 0;
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
				
void showGamestate(char gamearray[HEIGHT][WIDTH]){
	int i, j;

	for(i = 0; i <  HEIGHT; i++){
		for(j = 0; j < WIDTH; j++){
			if(j%WIDTH == 0){
				printf("\n");
			}
			printf("%c", gamearray[i][j]);
		}
	}
}



char** loadLevel(FILE **ifp, int level){
	char** initialgamestate;
	int i, j;
	
	switch(level){
		case 1:
			*ifp = fopen("level1.txt", "r");
			break;
		
		case 2:
			*ifp = fopen("level2.txt", "r");
			break;
			
		default:
			getchar();
			*ifp = fopen("level1.txt", "r");
	}
	
    if (*ifp == NULL) {		
		printf("No such file found. Exiting...\n");
		return (char **)EXIT_FAILURE;
    }
	
	initialgamestate = createEmptystate (WIDTH, HEIGHT);
		
	storeGamestate(ifp, initialgamestate);
	
	return initialgamestate;
}

void storeGamestate(FILE **ifp, char** gamestate){
	int i;
	
	/*For each board row*/ 
    for (i = 0; i < HEIGHT; i++) {
    	if((fscanf(*ifp, " %s", (char*)gamestate[i]) != 1)) {						
    		printf("Correct values not found. Exiting...\n");
    		EXIT;
    	}
	}
	fclose(*ifp);
}

char** createEmptystate(int width, int height){
	char** initialgamestate;
	int i;
	
	initialgamestate = (char **) calloc(height, sizeof(char*));
	
	for( i = 0; i < height; i++){
		initialgamestate[i] = (char*) calloc(width, sizeof(char));
	}
	
	return initialgamestate;
}
