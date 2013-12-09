#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

#define Blu 51, 204, 255
#define Grn 51, 255, 102
#define Red 255, 10, 51

/* Prototypes */
void clean();

int SDL (char screen[][HEIGHT]){

	/* Screen attributes */
	const int WIDTH = 640;
	const int HEIGHT = 480;
	const int BPP = 32;

	/* Surfaces */
	SDL_Surface *fore = NULL;
    SDL_Surface *back = NULL;

    /* Initialisation*/
	SDL_Init(SDL_INIT_EVERYTHING);

	for (int h = 0; h <HEIGHT; h++){
		for (int w = 0; w <WIDTH; w++){

			/* Renders appropriate image from character */
			switch(board[h][w]){

				case '#':
				/* render wall, set colour, x, y and render square */
				break;

				case 'G':
				/* render gotchi */
				break:

				case '.';
				/* render clear */

				default:
				/* render default */
			}
		}
	}

	SDL_Delay(200);

	clean();

	return 0;
}

void clean(){

    /* Free images */
    SDL_FreeSurface(fore);
    SDL_FreeSurface(back);
    
    /* Quit SDL */
    SDL_Quit();    
}
