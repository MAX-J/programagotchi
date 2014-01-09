#include "programagotchi.h"

#define STR_LENGTH 100
#define JLVL 0
#define MLVL 1
#define BLVL 2
#define LVL  3

int SDL_Events_Games(Display *d, int level[4]);

void SDL_Events_ting(Display *d);

void UpdateWindow(SDL_Simplewin sw);

void GotchiMovement(Display *d, int level[4]);

void YourLevel(int level);

void read2array(FILE * file, char gamearray[HEIGHT][WIDTH]);

int IncubatorButtonClicked(int x, int y);

int XYInMenu(int x, int y);

// Move the Gotchi in the incubator.
int main()
{
  int W = 360, H = 480;
  Display *d; 
  int count = 0; 
  int level[4] = {0};
  FILE *lvlfile;
  lvlfile = fopen("level.txt", "r");
  fscanf(lvlfile, "%d %d %d", &level[JLVL], &level[MLVL], &level[BLVL]);
  fclose(lvlfile);
  level[LVL] = level[JLVL] +  level[MLVL] + level[BLVL];
  /*level++;*/
  lvlfile = fopen("level.txt", "w");
  fprintf(lvlfile, "%d %d %d", level[JLVL], level[MLVL], level[BLVL]);
  fclose(lvlfile);
  //YourLevel(level[LVL]);
  d = start(W, H, "./newinc.bmp", "./gotchipod.bmp");
  while(1) { 
    //MAXJ_SDL_DrawText(d, "level", 10, 10);
    GotchiMovement(d, level);  
    if(count % 15 == 0) {
      YourLevel(level[LVL]);
     }
    SDL_Events_Games(d, level);
    count++;
  }
  return 0;
}

// Gobble all events & ignore most
int SDL_Events_Games(Display *d, int level[4])
{
  char commandstr[STR_LENGTH];
  char gamearray[HEIGHT][WIDTH];
  SDL_Simplewin gamewin;
  SDL_Event event;
  SDL_Event event2;
  FILE *file;
  int a = 0, loop = 0;
  while(SDL_PollEvent(&event)) {      
     switch (event.type){
     case SDL_MOUSEBUTTONDOWN:
       if(IncubatorButtonClicked(event.button.x, event.button.y) == 1)  {
	 Menu(d, 0, 0);
	 loop = 1;
       }
       while(loop > 0) {
       while(SDL_PollEvent(&event2)) {   
	 switch (event2.type){
	 case SDL_MOUSEBUTTONDOWN:
	   if((a = XYInMenu(event2.button.x, event2.button.y)) > 0 && a <= 3) {
	     if(a == 1) {
		     file = fopen("jump.txt", "r");
		     Neill_SDL_Init(&gamewin, "Jump Game Level 1");
	     }
	     if(a == 2) {
	       file = fopen("level1.txt", "r");
	       Neill_SDL_Init(&gamewin, "Maze Game Level 1");
	     }
	     if(a == 3) {
	       file = fopen("level2.txt", "r");
		     Neill_SDL_Init(&gamewin, "Maze Game Level 2");
	     }
	     //YourLevel(level[LVL]);
	     Incubator(d, 0, 0);
	     read2array(file, gamearray);
	     SDL(gamearray, "", gamewin);
	     do {
	       printf("\nEnter Command: ");
	       fgets(commandstr,STR_LENGTH,stdin);
	       //remove spaces to simplify processing 
	       //parse the command line
	       runcommand(gamewin, gamearray, commandstr); 	    
	       Neill_SDL_Events(&gamewin);
	     }while(!gamewin.finished);
	     loop = 0;
	   }
	   if(a == 0) {
	     Incubator(d, 0, 0);
	     loop = 0;
	   }
	 }
       }
       }
       return 0;
     case SDL_KEYDOWN:
       switch(event.key.keysym.sym){
       case SDLK_m:
	 Menu(d, 0, 0);
	 loop = 1;
	 while(loop > 0) {
	 while(SDL_PollEvent(&event2)) {   
	   switch (event2.type){
	   case SDL_MOUSEBUTTONDOWN:
	     if((a = XYInMenu(event2.button.x, event2.button.y)) > 0 && a <= 3) {
	       if(a == 1) {
		 file = fopen("jump.txt", "r");
		 Neill_SDL_Init(&gamewin, "Jump Game Level 1");
	       }
	       if(a == 2) {
		 file = fopen("level1.txt", "r");
		 Neill_SDL_Init(&gamewin, "Maze Game Level 1");
	       }
	       if(a == 3) {
		 file = fopen("level2.txt", "r");
		 Neill_SDL_Init(&gamewin, "Maze Game Level 2");
	       }
	       //YourLevel(level[LVL]);
	       Incubator(d, 0, 0);
	       read2array(file, gamearray);
	       SDL(gamearray, "", gamewin);
	       do {
		 printf("\nEnter Command: ");
		 fgets(commandstr,STR_LENGTH,stdin);
		 //remove spaces to simplify processing 
		 //parse the command line
		 runcommand(gamewin, gamearray, commandstr); 	    
		 Neill_SDL_Events(&gamewin);
	       }while(!gamewin.finished);
	       loop = 0;
	     }
	     if(a == 0) {
	       Incubator(d, 0, 0);
	       loop = 0;
	     }
	   }
	 }
	 }
	 return 0;
       default:
	 exit(1);
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

void GotchiMovement(Display *d, int level[4])
{
  int x, y;
  x = 150;
  y = 200;
  paint(d, x, y);
  SDL_Events_Games(d, level);
  x = x + 10;
  y = y + 10;
  paint(d, x, y);
}

void YourLevel(int level)
{
  SDL_Simplewin sw;
  SDL_Surface *hello = NULL;
  SDL_Surface *surf = NULL;
  char text[30]; 
  sprintf(text, "I am level %d!", level);
  const char *moretext = "Play to level me up!";
  Mini_SDL_Init(&sw, text);
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
  Neill_SDL_DrawText(&sw, moretext, 5, 171);
  UpdateWindow(sw);
  SDL_Delay(1000);
  atexit(SDL_Quit);
  SDL_DestroyWindow(sw.win);
}

void read2array(FILE * file, char gamearray[HEIGHT][WIDTH])
{
    int m, n;
    char c;
    for (m = 0; m < HEIGHT; m++) {
	for (n = 0; n < WIDTH; n++) {
	    c = fgetc(file);
	    gamearray[m][n] = c;
	}
    }
}

int IncubatorButtonClicked(int x, int y)
{
  if((x <= 120 && x >= 40) && (y <= 460 && y >= 380)) {
    return 1;
  }
  if((x <= 350 && x >= 270) && (y <= 460 && y >= 380)) {
    exit(1);
  }
  
  return 0;
}

int XYInMenu(int x, int y)
{
  if((x <= 325 && x >= 45) && (y <= 120 && y >= 30)) {
    return 1;
  }
  if((x <= 325 && x >= 45) && (y <= 230 && y >= 140)) {
     return 2;
  }
  if((x <= 325 && x >= 45) && (y <= 340 && y >= 250)) {
    return 3;
  }
  if((x <= 325 && x >= 45) && (y <= 450 && y >= 360)) {
    return 0;
  }
  return 4;
}


