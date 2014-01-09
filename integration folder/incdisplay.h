// Display graphics using SDL
#include <SDL.h>

// to move foreground image over background
struct display;

typedef struct display Display;
// Create a display window
Display *start(int w, int h, char *bg, char *fg);
// Paint scene, handle events, delay for 1/50 second
void paint(Display *d, int x, int y);

void MAXJ_SDL_DrawText(Display *d, const char* text, int x, int y);

void Menu(Display *d, int x, int y);

void Incubator(Display *d, int x, int y);
