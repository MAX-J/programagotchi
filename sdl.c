//Using SDL and standard IO 
#include <SDL.h> 
#include <stdio.h>

//Screen dimension constants 
const int SCREEN_WIDTH = 360; 
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) 
{ 
  //char c;
  //int close = 0;
  //The window we'll be rendering to 
  SDL_Window* window = NULL; 
  //The surface contained by the window 
  SDL_Surface* screenSurface = NULL;
  SDL_Surface* image = NULL;
  //Initialize SDL 
  if(SDL_Init(SDL_INIT_VIDEO) < 0) { 
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()); 
  }
  else { 
    //Create window 
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN); 
    if(window == NULL) { 
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError()); 
    }
    else { 
      //Get window surface 
      screenSurface = SDL_GetWindowSurface(window); 
      //Fill the surface white 
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF)); 
      //Load splash image
      image = SDL_LoadBMP("./incubatorsmall.bmp");
      if(image == NULL) {
	printf("Unable to load image %s! SDL Error: %s\n", "./incubatorsmall.bmp", SDL_GetError());
	exit(1);
      }
      //Apply the image
      SDL_BlitSurface(image, NULL, screenSurface, NULL);
      //Update the surface 
      SDL_UpdateWindowSurface(window); 
      //Wait five seconds 
      SDL_Delay(5000); 
    } 
  }
  /*do {
    printf("Do you want to close the window? Y/N\n");
    scanf("%c", &c);
    if(c == 'Y') {
      close = 1;
    }
    }while(close != 1);*/
  //Destroy window 
  SDL_DestroyWindow(window); 
  //Quit SDL subsystems 
  SDL_Quit(); 
  return 0; 
}


 

