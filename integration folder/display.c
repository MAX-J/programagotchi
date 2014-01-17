#include "programagotchi.h"

int SDL(char board[][WIDTH], char string[MAX], int score, int delay, SDL_Simplewin sw){

  int SQH = (WWIDTH/WIDTH);
  int RDS = 6;
  int OFF = 80;
  SDL_Rect rectangle;
  SDL_Rect scoreback;
  SDL_Rect stringback;
  SDL_Rect circle;
  rectangle.w = SQH;
  rectangle.h = SQH;
  scoreback.w = WWIDTH/3;
  scoreback.h = OFF;
  stringback.w = WWIDTH/3;
  stringback.h = OFF;
  int stringx = (WWIDTH/2)-strlen(string);
  char scorestring[MAX];
  int scorex = OFF/2;
  int stringy = OFF/2;

 
    if(strcmp(string, "") != 0){
        Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
        header.x = WWIDTH/3;
        header.y = 0;
        SDL_RenderDrawRect(sw.renderer, &stringback);
    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    Neill_SDL_DrawText(&sw, string, stringx, stringy);
    }

    if(score != NO_SCORE){
        Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
        header.x = 0;
        header.y = 0;
        SDL_RenderDrawRect(sw.renderer, &scoreback);
    Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
    sprintf(scorestring, "Score: %d", score);
    Neill_SDL_DrawText(&sw, scorestring, scorex, stringy);
    }

    for (int w = 0; w <WIDTH; w++){
      for (int h = 0; h <HEIGHT; h++){
    
        switch(board[h][w]){

        /*
        G Gotchi
        1 Barrier
        C Candy(each one get 10 score, when you get all of them,100,win)
        S Hazard
        B Ball
        W Net
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

        case 'W':
        Neill_SDL_SetDrawColour(&sw, 255, 255, 255);
        circle.x = (w*SQH);
        circle.y = (h*SQH+OFF);
        SDL_RenderDrawRect(sw.renderer, &rectangle);
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
	
	   for (int w = 0; w <WIDTH; w++){
      for (int h = 0; h <HEIGHT; h++){
	if(board[h][w] == 'G') {
	  Neill_SDL_SetDrawColour(&sw, 0, 255, 0);
	  circle.x = ((w+0.5)*SQH);
	  circle.y = ((h+0.5)*SQH+OFF);
	  Neill_SDL_RenderFillCircle(sw.renderer, circle.x, circle.y, RDS);
	  Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
	  circle.x = ((w+0.1)*SQH);
	  circle.y = ((h+0.25)*SQH+OFF);
	  Neill_SDL_RenderFillCircle(sw.renderer, circle.x, circle.y, RDS/3);
	  circle.x = ((w+0.95)*SQH);
	  circle.y = ((h+0.25)*SQH+OFF);
	  Neill_SDL_RenderFillCircle(sw.renderer, circle.x, circle.y, RDS/3);
	}
      }
    }

    SDL_Delay(delay);

    // if(strcmp(string, "") != 0){
    // Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
    // Neill_SDL_DrawText(&sw, string, stringx, stringy);
    // }

    // if(score != NO_SCORE){
    // Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
    // sprintf(scorestring, "Score: %d", score);
    // Neill_SDL_DrawText(&sw, scorestring, scorex, stringy);
    // }
	
    // Updates window - no graphics appear on some devices until this is finished
    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);

    //Neill_SDL_SetDrawColour(&sw, 0, 0, 0);
    //SDL_RenderClear(sw.renderer);
    return 0;
}
