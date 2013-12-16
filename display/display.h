#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsdl2.h"

#define HEIGHT 70
#define WIDTH 110

/* Prototypes */
void clean();
int SDL (char board[][WIDTH], SDL_Simplewin sw);

#endif

/* 
 
 Use the folowing in your own files:
 
 #include "display.h" 
 
*/