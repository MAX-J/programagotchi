#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <SDL.h>
//#include "neillsdl2_font.h"

#define SDL_8BITCOLOUR 256
#define FONTCHARS 256
#define FONTBYTESCHAR 8
#define FNT_FONTHEIGHT 8
#define FNT_FONTWIDTH 8
#define HEIGHT 70
#define WIDTH 110
#define EXIT exit(2)
#define WWIDTH 171
#define WHEIGHT 175

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

//display
void clean();
int SDL (char board[][WIDTH], SDL_Simplewin sw);

//interpreter
void runcommand(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *inputstr); 

// Display graphics using SDL

// to move foreground image over background
struct display;

typedef struct display Display;
// Create a display window
Display *start(int w, int h, char *bg, char *fg);
// Paint scene, handle events, delay for 1/50 second
void paint(Display *d, int x, int y);

void MAXJ_SDL_DrawText(Display *sw, const char* text, int ox, int oy);
unsigned char* MAXJ_SDL_GetFont();

void MAXJ_SDL_SetDrawColour(Display *sw, Uint8 r, Uint8 g, Uint8 b);

// All info required for windows / renderer & event loop
struct SDL_Simplewin {
   SDL_bool finished;
   SDL_Window *win;
   SDL_Renderer *renderer;
};
typedef struct SDL_Simplewin SDL_Simplewin;

void Neill_SDL_Init(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);

void Neill_SDL_DrawText(SDL_Simplewin *sw, const char* text, int x, int y);
unsigned char* Neill_SDL_GetFont();
