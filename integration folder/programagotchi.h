
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsdl2_font.h"

#define HEIGHT 70
#define WIDTH 110

//display
void clean();
int SDL (char board[][WIDTH], SDL_Simplewin sw);

//interpreter
void runcommand(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *inputstr); 


