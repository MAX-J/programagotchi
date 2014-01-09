#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <ctype.h>
#include <SDL.h>
#include "neillsdl2_inc.h"
#include "incdisplay.h"

#define STRLEN 400
#define MAX 32
#define HEIGHT 70
#define WIDTH 110
#define EXIT exit(2)

//
#define QUIT_COMMAND -2
#define BAD_COMMAND -1
#define NO_ACTION 0
#define ON_EXIT 1
#define ON_HAZARD 2
#define ATE_CANDY 3

//display
int SDL (char board[][WIDTH], char String[MAX], SDL_Simplewin sw);

//interpreter
int runcommand(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *inputstr);
int parsefcn(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *filestr, char *commandstr);
