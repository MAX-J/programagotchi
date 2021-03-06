#include "incdisplay.h"
#include "neillsdl2_inc.h"

SDL_Simplewin sw; 

int SDL_Events_newmanting(Display *d);

void SDL_Events_ting(Display *d);

void UpdateWindow(SDL_Simplewin sw);

void GotchiMovement(Display *d);

void Gametimewindow();

// Move the Gotchi in the incubator.
int main()
{
  //const char *leveltext = "Lvl: 1.";
  int W = 360, H = 480;
  Display *d = start(W, H, "./newinc.bmp", "./gotchipod.bmp");
  int count = 0;
  while(1) { 
    count++;
    GotchiMovement(d);
    if(count % 15 == 0) {
      Gametimewindow();
      do {
	GotchiMovement(d);
	Neill_SDL_Events(&sw);
      }while(!sw.finished);
    }
    SDL_Events_newmanting(d);
  }
  return 0;
}

// Gobble all events & ignore most
int SDL_Events_newmanting(Display *d)
{
  SDL_Event event;
   while(SDL_PollEvent(&event)) 
   {      
     switch (event.type){
     case SDL_QUIT:
       atexit(SDL_Quit);
       return 0;  
     case SDL_KEYDOWN:
     switch(event.key.keysym.sym){
        case SDLK_0:
          Gametimewindow();
	  do {
	    GotchiMovement(d);
	    Neill_SDL_Events(&sw);
	  }while(!sw.finished);
	  //Neill_SDL_Init(&sw);
	  return 1;
        case SDLK_1:
          Gametimewindow();
	  do {
	    GotchiMovement(d);
	    Neill_SDL_Events(&sw);
	  }while(!sw.finished);
	  //Neill_SDL_Init(&sw);
	  return 1;          
        case SDLK_2:
          Gametimewindow();
	  do {
	    GotchiMovement(d);
	    Neill_SDL_Events(&sw);
	  }while(!sw.finished);
	  //Neill_SDL_Init(&sw);
	  return 1;
        case SDLK_3:
	  do {
	    GotchiMovement(d);
	    Neill_SDL_Events(&sw);
	  }while(!sw.finished);
          Gametimewindow();
	  //Neill_SDL_Init(&sw);
	  return 1;          
        default:
          return 0;
      }
    }
  }
   return 0;
}

// Updates window - no graphics appear on some devices until this is finished
void UpdateWindow(SDL_Simplewin sw) 
{
  SDL_RenderPresent(sw.renderer);
  SDL_UpdateWindowSurface(sw.win);
  SDL_Delay(1000);
}

void GotchiMovement(Display *d)
{
  int x = 150, y = 200;
  paint(d, x, y);
  x = x;
  y = y - 1;
  paint(d, x, y);
  x = x + 1;
  y = y + 1;
  paint(d, x, y);
  x = x - 1;
  y = y + 1;
  paint(d, x, y);
  x = x - 1;
  y = y - 1;
  paint(d, x, y);
}

void Gametimewindow()
{
  SDL_Surface *hello = NULL;
  SDL_Surface *surf = NULL;
  const char *text = "Fancy a game?";
  const char *moretext = "Click y to play.";
  Neill_SDL_Init(&sw);
  surf = SDL_GetWindowSurface(sw.win);
  hello = SDL_LoadBMP("./gotchi.bmp");
  SDL_BlitSurface(hello, NULL, surf, NULL);
  Neill_SDL_SetDrawColour(&sw, 0, 255, 255);
  Neill_SDL_DrawText(&sw, text, 30, 166);
  UpdateWindow(sw);
  Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
  SDL_RenderClear(sw.renderer);
  SDL_RenderPresent(sw.renderer);
  SDL_BlitSurface(hello, NULL, surf, NULL);
  Neill_SDL_SetDrawColour(&sw, 0, 255, 255);
  Neill_SDL_DrawText(&sw, moretext, 25, 166);
  UpdateWindow(sw);
}
