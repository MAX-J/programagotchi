/*#include "display.h"*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 70
#define WIDTH 110
#define EXIT exit(2)

char** loadLevel(FILE **ifp, int argc, char* argv[], int level);

char** createInitialgamestate (int width, int height);

int main(int argc, char* argv[]){
	FILE *ifp;

	int level;

	loadLevel(&ifp, argc, argv, level);

	return 0;
}


char** loadLevel(FILE **ifp, int argc, char* argv[], int level){
	char** initialgamestate;
	
	
	*ifp = fopen(argv[1], "r");

    if (*ifp == NULL) {		
		printf("No such file found. Exiting...\n");
		return (char **)EXIT_FAILURE;
    }
	
	initialgamestate = createInitialgamestate (WIDTH, HEIGHT);

}

void storeGamestate(FILE **ifp, char** gamestate){
	int i;
		
	/*For each board row*/ 
    for (i = 0; i < HEIGHT; i++) {
    	if((fscanf(*ifp, " %s", (char*)&gamestate[i]) != 1)) {						
    		printf("Correct values not found. Exiting...\n");
    		EXIT;
    	}
		/* Check that the length of each row is as expected */	
		if(strlen(gamestate[i])!= WIDTH){
			printf("Board not as expected. Exiting......\n");
			EXIT;
		}	
	}
		
	fclose(*ifp);		
}

char** createInitialgamestate (int width, int height){
	char** initialgamestate;
	int i;
	
	initialgamestate = (char **) malloc(height*sizeof(char*));
	
	for( i = 0; i < height; i++){
		initialgamestate[i] = (char*) malloc(width*sizeof(char));
	}
	return initialgamestate;
}