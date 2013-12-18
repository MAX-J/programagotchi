// Display graphics using SDL
#include <SDL2/SDL.h>
#define SDL_8BITCOLOUR 256

#define FONTCHARS 256
#define FONTBYTESCHAR 8
#define FNT_FONTHEIGHT 8
#define FNT_FONTWIDTH 8

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
