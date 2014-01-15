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
#define NO_SCORE -1
#define FREE_SPACE '.'
#define BORDER '#'
#define BARRIER '1'
#define EXIT_POINT 'E'
#define HAZARD 'S'
#define CANDY 'C'
#define NET 'W'
#define GOTCHI 'G'
#define BALL 'B'

//
#define NO_ACTION 0
#define GENERAL_STOP -1
#define BAD_COMMAND -2
#define QUIT_COMMAND -3
#define HIT_OBSTACLE -4
#define ON_EXIT -5
#define ON_HAZARD -6
#define ATE_CANDY -7
#define HIT_BASKET -8
//
#define WIN 1
#define LOSE 0

//display
int SDL (char board[][WIDTH], char string[MAX], int score, SDL_Simplewin sw);
//interpreter
int runcommand(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *inputstr);
// Game functions
int playMaze(char gamearray[HEIGHT][WIDTH], SDL_Simplewin sw);
int playJump(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin);