#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** loadLevel(FILE **ifp, int argc, char* argv[], int level);













char** loadLevel(FILE **ifp, int argc, char* argv[], int level){

	*ifp = fopen(argv[1], "r");

    if (*ifp == NULL) {		
		printf("No such file found. Exiting...\n");
		return (char **)EXIT_FAILURE;
    }

}