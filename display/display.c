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
int SDL (char board[][WIDTH]);

int main (void){

  	char board[HEIGHT][WIDTH];

  	for (int w = 0; w <WIDTH; w++){
    	for (int h = 0; h <HEIGHT; h++){	
      	board[h][w] = '.';
    	}
  	}

	for(int i=0; i<6; i++){
  board[69-i][52+i] = '#';
  board[69][52+i] = '#';
  }
  	
  for(int i=0; i<6; i++){
  board[68][36+i] = '.';
  board[68][36+i+1] = 'G';
  }

  SDL(board);

	return 0;
}

int SDL (char board[][WIDTH]){

  SDL_Simplewin sw;

  /* Initialisation*/
	Neill_SDL_Init(&sw);

	int SQH = (WWIDTH/WIDTH);
	SDL_Rect rectangle;
  rectangle.w = SQH;
  rectangle.h = SQH;

  do{
    for (int w = 0; w <WIDTH; w++){
			for (int h = 0; h <HEIGHT; h++){
		
			switch(board[h][w]){

				case '#':
				Neill_SDL_SetDrawColour(&sw, 134, 24, 123);
      	rectangle.x = (w*SQH);
      	rectangle.y = (h*SQH);
      	SDL_RenderDrawRect(sw.renderer, &rectangle);
				break;

				case 'G':
				Neill_SDL_SetDrawColour(&sw, 245, 2, 56);
        for (int i = 0; i<10; i++){
      	rectangle.x = (w*SQH+i);
      	rectangle.y = (h*SQH);
      	SDL_RenderFillRect(sw.renderer, &rectangle);
        }
				break;

				case '.':
				Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
      	rectangle.x = (w*SQH);
      	rectangle.y = (h*SQH);
      	SDL_RenderFillRect(sw.renderer, &rectangle);
				break;

				default:
				Neill_SDL_SetDrawColour(&sw, 25, 135, 201);
      	rectangle.x = (w*SQH);
      	rectangle.y = (h*SQH);
      	SDL_RenderFillRect(sw.renderer, &rectangle);
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

	Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
    SDL_RenderClear(sw.renderer);
    Neill_SDL_Events(&sw);

	SDL_Delay(100);

	return 0;
}

// void clean(){

//     /* Free images */
//     SDL_FreeSurface(fore);
//     SDL_FreeSurface(back);
    
//     /* Quit SDL */
//     SDL_Quit();    
// }
