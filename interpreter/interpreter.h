#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "neillsdl2_font.h"

#define GRID_HEIGHT 20
#define GRID_WIDTH 20
#define STR_LENGTH 400 

//UPDATE: MORE OF THESE IN FUTURE
//(statuses wil include stuff like 'HIT_SHARK' etc.)
#define SUCCESS 0
#define BAD_COMMAND 1

//CHANGEHERE! MAKE SURE EVERYTHING IN THIS FILE GOES TO 'PROGRAMAGOTCHI.H'!
int runcommand(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *inputstr);
int parsefcn(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *filestr, char *commandstr);

