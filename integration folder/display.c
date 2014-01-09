#include "programagotchi.h"

int SDL(char board[][WIDTH], char string[MAX],  SDL_Simplewin sw){

  int SQH = (WWIDTH/WIDTH);
  int RDS = 6;
  int OFF = 40;
  SDL_Rect rectangle;
  SDL_Rect circle;
  rectangle.w = SQH;
  rectangle.h = SQH;
  int stringx = WWIDTH/2;
  int stringy = OFF/2;
 Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
 Neill_SDL_DrawText(&sw, string, stringx, stringy);
    
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
        rectangle.y = (h*SQH+OFF);
        SDL_RenderDrawRect(sw.renderer, &rectangle);
        break;

        case '#':
        Neill_SDL_SetDrawColour(&sw, 245, 251, 255);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderDrawRect(sw.renderer, &rectangle);
        break;

        case 'G':
        Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case 'S':
        Neill_SDL_SetDrawColour(&sw, 245, 2, 56);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case 'B':
        Neill_SDL_SetDrawColour(&sw, 121, 134, 9);
        circle.x = (w*SQH);
        circle.y = (h*SQH+OFF);
        Neill_SDL_RenderFillCircle(sw.renderer, circle.x, circle.y, RDS);
        break;

        case 'E':
        Neill_SDL_SetDrawColour(&sw, 12, 201, 178);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case '.':
        Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        case 'C':
        Neill_SDL_SetDrawColour(&sw, 134, 112, 255);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;

        default:
        Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
        rectangle.x = (w*SQH);
        rectangle.y = (h*SQH+OFF);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        break;
        }
      }
    }
    // Updates window - no graphics appear on some devices until this is finished
    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);
    return 0;
}
