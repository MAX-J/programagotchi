//#include "incdisplay.h"
#include"programagotchi.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct display
{
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Surface *bg;
  SDL_Surface *fg;
  SDL_Renderer *renderer;
};

static void report()
{
fprintf(stderr, "SDL error: %s\n", SDL_GetError());
SDL_Quit();
exit(1);
}

Display *start(int w, int h, char *bg, char *fg)
{
 Display *d = malloc(sizeof(Display));
 int err = SDL_Init(SDL_INIT_EVERYTHING);
 if (err != 0) report();
 d->window = SDL_CreateWindow("Incubator", 0, 0, w, h, SDL_WINDOW_BORDERLESS);
 if (d->window == NULL) report();
 d->surface = SDL_GetWindowSurface(d->window);
 if (d->surface == NULL) report();
 d->bg = SDL_LoadBMP(bg);
 if (d->bg == NULL) report();
 d->fg = SDL_LoadBMP(fg);
 if (d->fg == NULL) report();
 return d;
}

static void img(Display *d, SDL_Surface *i, int x, int y)
{
SDL_Rect rectData = { x, y, i->w, i->h };
SDL_Rect *rect = &rectData;
int err = SDL_BlitSurface(i, NULL, d->surface, rect);
if (err != 0) report();
}

void paint(Display *d, int x, int y)
{
img(d, d->bg, 0, 0);
img(d, d->fg, x, y);
int err = SDL_UpdateWindowSurface(d->window);
if (err != 0) report();
SDL_Event eventData;
SDL_Event *event = &eventData;
while (SDL_PollEvent(event) == 1)
{
if (event->type == SDL_QUIT) { SDL_Quit(); exit(0); }
}
SDL_Delay(200);
}

void Incubator(Display *d, int level)
{
  char *incname = malloc(20*sizeof(char));
  sprintf(incname, "./newinc%d.bmp", level);
  
  if(level == 1) {
    d->fg = SDL_LoadBMP("./gotchipod1.bmp");
  }
  if(level == 2) {
    d->fg = SDL_LoadBMP("./gotchipod2.bmp");
  }
  if(level > 2) {
    d->fg = SDL_LoadBMP("./gotchipod3.bmp");
  }
  d->bg = SDL_LoadBMP(incname);
  paint(d, 150, 200);
}

void Menu(Display *d)
{
  d->fg = SDL_LoadBMP("./fg.bmp");
  paint(d, 0, 0);
}

void SubMenu(Display *d, int level)
{
  if(level == 1) {
      d->fg = SDL_LoadBMP("./one unlocked.bmp");
      paint(d, 0, 0);
  }
  else if(level == 2) {
      d->fg = SDL_LoadBMP("./two unlocked.bmp");
      paint(d, 0, 0);
  }
  else {
      d->fg = SDL_LoadBMP("./all unlocked.bmp");
      paint(d, 0, 0);
  }
}

void MAXJ_SDL_DrawText(Display *d, const char* text, int ox, int oy)
{
   unsigned int i, x, y, col, row, len;
   unsigned char *fnt, chr;
   SDL_SetRenderDrawColor(d->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
   fnt = Neill_SDL_GetFont();
   col = row = 0;
   len = strlen(text);
   for(i = 0; i < len; i++){
      switch(text[i]){
         case '\n':
            row++;
            col = 0;
            chr = 0;
            break;

         case '\r':
            chr = 0;
            break;

         case '\t':
            chr = 0;
            col += 4 - col % 4;
            break;
   
         default:
            col++;
            chr = text[i];
            break;
      }

      if(chr != 0){
         for(y = 0; y < FNT_FONTHEIGHT; y++){
            for(x = 0; x < FNT_FONTWIDTH; x++){
               if(fnt[text[i] * FNT_FONTHEIGHT + y] >> (7 - x) & 1){
                        SDL_RenderDrawPoint(d->renderer, ((col - 1) * FNT_FONTWIDTH) + x + ox, (y + row * FNT_FONTHEIGHT) + oy);
               }
            }
         }
      }
   }
   SDL_RenderPresent(d->renderer);
   SDL_UpdateWindowSurface(d->window);
}
