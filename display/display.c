#include "display.h"

int SDL (char board[][WIDTH], SDL_Simplewin sw)){

  int SQH = (WWIDTH/WIDTH);
  int RDS = (WHEIGHT/HEIGHT);
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
        B Ball
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

        case 'B':
        Neill_SDL_SetDrawColour(&sw, 121, 134, 9);
        circle.x = (w*SQH);
        circle.y = (h*SQH);
        Neill_SDL_RenderFillCircle(sw.renderer, circle.x, circle.y, RDS);
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

  return 0;
}

// void clean(){

//     /* Free images */
//     SDL_FreeSurface(fore);
//     SDL_FreeSurface(back);
    
//     /* Quit SDL */
//     SDL_Quit();    
// }
