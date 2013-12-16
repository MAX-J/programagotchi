/*#include "display.h"*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 70
#define WIDTH 110
#define EXIT exit(1)

/* Take SDL window and empty array from AY */
/* Use SDL functions in own function to use graphics */
/* Make header file containing pointer to 2d array and SDL window */

/* Opens file, creates empty array, stores level in array, returns pointer to array */
char** loadLevel(FILE **ifp, int argc, char* argv[], int level);

/* Called by loadLevel, creates an empty 110 wide x 70 high array using calloc*/ 
char** createEmptystate (int width, int height);

/* Called by loadLevel, store game state from file into array */
void storeGamestate(FILE **ifp, char** gamestate);

/* Show game state*/
void showGamestate(char** gamestate);


int main(int argc, char* argv[]){
	
	FILE *ifp= NULL;
	int level;
	char** initialgamestate;

	initialgamestate = loadLevel(&ifp, argc, argv, level);
	
	
	
	showGamestate(initialgamestate);
	
	
	return 0;
}

void showGamestate(char** gamestate){
	int i, j;

	for(i = 0; i <  HEIGHT; i++){
		for(j = 0; j < WIDTH; j++){
			if(j%WIDTH == 0){
				printf("\n");
			}
			printf("%c", gamestate[i][j]);
		}
	}
}



char** loadLevel(FILE **ifp, int argc, char* argv[], int level){
	char** initialgamestate;
	int i, j;
	
	*ifp = fopen(argv[1], "r");

    if (*ifp == NULL) {		
		printf("No such file found. Exiting...\n");
		return (char **)EXIT_FAILURE;
    }
	
	initialgamestate = createEmptystate (WIDTH, HEIGHT);
		
	storeGamestate(ifp, initialgamestate);
	
	return initialgamestate;
}

void storeGamestate(FILE **ifp, char** gamestate){
	int i, j;
	
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