#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "neillsdl2_inc.h"
#include "incdisplay.h"

#define HEIGHT 70
#define WIDTH 110
#define EXIT exit(2)

//display
int SDL (char board[][WIDTH], SDL_Simplewin sw);

//interpreter
void runcommand(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *inputstr); 

/* Opens file, creates empty array, stores level in array, returns pointer to array */
char** loadLevel(FILE **ifp, int level);
