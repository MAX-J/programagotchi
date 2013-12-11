#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsdl2.h"

#define HEIGHT 70
#define WIDTH 110

/* Prototypes */
void clean();
int SDL (char board[][WIDTH]);

int main (void){

    char board[HEIGHT][WIDTH];

  //   for (int w = 0; w <WIDTH; w++){
  //     for (int h = 0; h <HEIGHT; h++){  
  //       board[h][w] = '.';
  //     }
  //   }

  // for(int i=0; i<6; i++){
  // board[69-i][52+i] = '#';
  // board[69][52+i] = 'y';
  // }
    
  // for(int i=0; i<6; i++){
  // board[68][36+i] = '.';
  // board[68][36+i+1] = 'G';
  // }

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

        /*
        G Gotchi
        1 Barrier
        C Candy(each one get 10 score, when you get all of them,100,win)
        S Hazard
        E Exit
        . Free space
        # Border/wall
        Default Non-descript item
        */

        case '1':
        Neill_SDL_SetDrawColour(&sw, 134, 24, 123);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH);
        SDL_RenderDrawRect(sw.renderer, &rectangle);
        break;

        case '#':
        Neill_SDL_SetDrawColour(&sw, 245, 251, 255);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH);
        SDL_RenderDrawRect(sw.renderer, &rectangle);
        break;

        case 'G':
        Neill_SDL_SetDrawColour(&sw, 114, 148, 56);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case 'S':
        Neill_SDL_SetDrawColour(&sw, 245, 2, 56);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case 'E':
        Neill_SDL_SetDrawColour(&sw, 12, 201, 178);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case '.':
        Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        default:
        Neill_SDL_SetDrawColour(&sw, 189, 135, 201);
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
