#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

#define Blu 51, 204, 255
#define Grn 51, 255, 102
#define Red 255, 10, 51


int main (char board[][HEIGHT]){

	SDL_Surface* fore = NULL;
    SDL_Surface* back = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);

	back = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE);

	for (int h = 0; h <HEIGHT; h++){
		for (int w = 0; w <WIDTH; w++){

			switch(board[h][w]){

				case '#':
				/* render wall */
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

	SDL_Delay(2000);

	SDL_Quit();

}
