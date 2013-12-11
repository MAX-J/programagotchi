#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsdl2.h"

#define Blu 51, 204, 255
#define Grn 51, 255, 102
#define Red 255, 10, 51

#define HEIGHT 70
#define WIDTH 110

/* Prototypes */
void clean();
int SDL (char board[][HEIGHT]);

void display(){

  	char board[WIDTH][HEIGHT];

  	for (int h = 0; h <HEIGHT; h++){
    	for (int w = 0; w <WIDTH; w++){	
      	board[h][w] = '#';
    	}
  	}
  	board[12][97] = 'G';
  	
  	SDL(board);
}

int SDL (char board[][HEIGHT]){

    SDL_Simplewin sw;

    /* Initialisation*/
	Neill_SDL_Init(&sw);

	int SQH = (WHEIGHT/HEIGHT);
	SDL_Rect rectangle;
   	rectangle.w = SQH;
   	rectangle.h = SQH;
	do{
	for (int h = 0; h <HEIGHT; h++){
		for (int w = 0; w <WIDTH; w++){

			switch(board[h][w]){

				case '#':
				Neill_SDL_SetDrawColour(&sw, 134, 24, 123);
      			rectangle.x = (h*SQH);
      			rectangle.y = (w*SQH);
      			SDL_RenderDrawRect(sw.renderer, &rectangle);
				break;

				case 'G':
				Neill_SDL_SetDrawColour(&sw, 245, 2, 56);
      			rectangle.x = (h*SQH);
      			rectangle.y = (w*SQH);
      			SDL_RenderDrawRect(sw.renderer, &rectangle);
				break;

				case '.':
				Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
      			rectangle.x = (h*SQH);
      			rectangle.y = (w*SQH);
      			SDL_RenderFillRect(sw.renderer, &rectangle);
				break;

				default:
				/* render default */
				break;
			}
		}
	}
	// Updates window - no graphics appear on some devices until this is finished
	SDL_RenderPresent(sw.renderer);
	SDL_UpdateWindowSurface(sw.win);
	// Checks if anyone has triggered the SDL window to end
	Neill_SDL_Events(&sw);
	}while(!sw.finished);

	SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win); 

	SDL_Delay(100);

}

// void clean(){

//     /* Free images */
//     SDL_FreeSurface(fore);
//     SDL_FreeSurface(back);
    
//     /* Quit SDL */
//     SDL_Quit();    
// }
