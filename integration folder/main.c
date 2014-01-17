#include "programagotchi.h"

#define LVL  3
#define JUMP 0
#define MAZE 1
#define BBALL 2
#define LVLARR 4
#define GOBACK 3

#define J1 0
#define J2 1
#define J3 2
#define M1 3
#define M2 4
#define M3 5
#define B1 6
#define B2 7
#define B3 8
#define SCORES  9

#define W 360
#define H 480

int SDL_Events_Games(Display *d, int level[LVLARR], int score[SCORES]);

void SDL_Events_ting(Display *d);

void UpdateWindow(SDL_Simplewin sw);

void GotchiMovement(Display *d, int level[LVLARR], int score[SCORES]);

void YourLevel(int level);

void read2array(FILE * file, char gamearray[HEIGHT][WIDTH]);

int IncubatorButtonClicked(int x, int y);

int XYInMenu(int x, int y);

int SDL_Menu_Events(Display *d, char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin, int level[LVLARR], int score[SCORES]);

int SDL_SubMenu_Events(Display *d, char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin, int game, int level[LVLARR], int score[SCORES]);

void UpdateLevelFile(int level[LVLARR]);

void UpdateHiScores(int score[SCORES]);

int filechecker(FILE * file);

// Move the Gotchi in the incubator.
int main()
{
  Display *d; 
  char *incname = malloc(20*sizeof(char));
  int count = 0; 
  int level[4] = {0};
  int score[SCORES] = {0};
  FILE *lvlfile, *scoresfile;
  lvlfile = fopen("level.txt", "r");
  filechecker(lvlfile);
  fscanf(lvlfile, "%d %d %d", &level[JUMP], &level[MAZE], &level[BBALL]);
  fclose(lvlfile);
  level[LVL] = level[JUMP] +  level[MAZE] + level[BBALL] - 2;
  scoresfile = fopen("hiscores.txt", "r");
  filechecker(scoresfile);
  fscanf(scoresfile, "%d %d %d %d %d %d %d %d %d", &score[J1], &score[J2], &score[J3], &score[M1], &score[M2], &score[M3], &score[B1], &score[B2], &score[B3]);
  fclose(scoresfile);
  UpdateLevelFile(level);
  sprintf(incname, "./newinc%d.bmp", level[LVL]);
  if(level[LVL] == 1) {
    d = start(W, H, "./newinc1.bmp", "./gotchipod1.bmp");
  }
  else if(level[LVL] == 2) {
    d = start(W, H, "./newinc2.bmp", "./gotchipod2.bmp");
  }
  else {
    d = start(W, H, incname, "./gotchipod3.bmp");
  }
  while(1) { 
    GotchiMovement(d, level, score);  
    SDL_Events_Games(d, level, score);
    count++;
    UpdateLevelFile(level);  
  }
  return 0;
}

// Gobble all events & ignore most
int SDL_Events_Games(Display *d, int level[LVLARR], int score[SCORES])
{
  char gamearray[HEIGHT][WIDTH];
  SDL_Simplewin gamewin;
  SDL_Event event;
  while(SDL_PollEvent(&event)) {      
     switch (event.type){
     case SDL_MOUSEBUTTONDOWN:
       if(IncubatorButtonClicked(event.button.x, event.button.y) == 1)  {
	 Menu(d);
	 SDL_Menu_Events(d, gamearray, gamewin, level, score);
       }
       return 0;
     case SDL_KEYDOWN:
       switch(event.key.keysym.sym){
       case SDLK_m:
	 Menu(d);
	 SDL_Menu_Events(d, gamearray, gamewin, level, score);
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

void GotchiMovement(Display *d, int level[LVLARR], int score[SCORES])
{
  int x, y;
  x = 150;
  y = 200;
  paint(d, x, y);
  SDL_Events_Games(d, level, score);
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
  if(level == 1) {
    hello = SDL_LoadBMP("./gotchi1.bmp");
  }
  else if(level == 2) {
    hello = SDL_LoadBMP("./gotchi2.bmp");
  }
  else {
    hello = SDL_LoadBMP("./gotchi3.bmp");
  }
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
  if((x <= 120) && (y >= 380)) {
    return 1;
  }
  if((x >= 270) && (y >= 380)) {
    exit(1);
  }
  
  return 0;
}

int XYInMenu(int x, int y)
{
  if((x <= 325 && x >= 45) && (y <= 120 && y >= 30)) {
    return JUMP;
  }
  if((x <= 325 && x >= 45) && (y <= 230 && y >= 140)) {
     return MAZE;
  }
  if((x <= 325 && x >= 45) && (y <= 340 && y >= 250)) {
    return BBALL;
  }
  if((x <= 325 && x >= 45) && (y <= 450 && y >= 360)) {
    return GOBACK;
  }
  return 4;
}

int SDL_Menu_Events(Display *d, char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin, int level[LVLARR], int score[SCORES])
{
  int a, loop = 1, result = 0;
  SDL_Event MenuEvent;
  while(loop > 0) {
    while(SDL_PollEvent(&MenuEvent)) {   
      switch (MenuEvent.type){
      case SDL_MOUSEBUTTONDOWN:
	if((a = XYInMenu(MenuEvent.button.x, MenuEvent.button.y)) < 3) {
	  if(a == JUMP) {
	    SubMenu(d, level[JUMP]);
	    result =SDL_SubMenu_Events(d, gamearray, gamewin, a, level, score);
	    return result;
	  }
	  if(a == MAZE) {
	    SubMenu(d, level[MAZE]);
	    result = SDL_SubMenu_Events(d, gamearray, gamewin, a, level, score);
	    return result;
	  }
	  if(a == BBALL) {
	    SubMenu(d, level[BBALL]);
	    result = SDL_SubMenu_Events(d, gamearray, gamewin, a, level, score);
	    return result;
	  }
	  loop = 0;
	}
	if(a == GOBACK) {
	  Incubator(d, level[LVL]);
	  loop = 0;
	}
      }
    }
  }
  return 0;
}

int SDL_SubMenu_Events(Display *d, char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin, int game, int level[LVLARR], int score[SCORES])
{
  int a, loop = 1, result = 0;
  SDL_Event SubMenuEvent;
  FILE *file = NULL;
  while(loop > 0) {
    while(SDL_PollEvent(&SubMenuEvent)) {   
      switch (SubMenuEvent.type){
      case SDL_MOUSEBUTTONDOWN:
	if((a = XYInMenu(SubMenuEvent.button.x, SubMenuEvent.button.y)) < 3) {
	  if(game == JUMP) {
	    if(a == 0) {
	      if(a < level[JUMP]) {
		file = fopen("jump1.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Jump Game Level 1");
	      }
	      else {
		loop++;
	      }
	    }
	    if(a == 1) {
	      if(a < level[JUMP]) {
		file = fopen("jump2.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Jump Game Level 2");
	      }
	      else {
		loop++;
	      }
	    }
	    if(a == 2) {
	      if(a < level[JUMP]) {
		file = fopen("jump3.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Jump Game Level 3");
	      }
	      else {
		loop++;
	      }
	    }
	    if(loop == 1) {
	      Incubator(d, level[LVL]);
	      system("xdg-open jump.gfn");
	      read2array(file, gamearray);
	      result = playJump(gamearray, gamewin);
	      SDL_DestroyWindow(gamewin.win);
	      atexit(SDL_Quit);
	      if(result > 0) {
		level[JUMP]++;
		level[LVL]++;
		if(result > score[JUMP*LVL+a]) {
		  score[JUMP*LVL+a] = result;
		  UpdateHiScores(score);
		}
	      }
	      Incubator(d, level[LVL]);
	    }
	    loop--;
	  }
	  if(game == MAZE) {
	    if(a == 0) {
	      if(a < level[MAZE]) {
		file = fopen("level1.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Maze Game Level 1");
		system("xdg-open findcandy.gfn");
	      }
	      else {
		loop++;
	      }
	    }
	    if(a == 1) {
	      if(a < level[MAZE]) {
		file = fopen("level2.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Maze Game Level 2");
		system("xdg-open solve.gfn");
	      }
	      else {
		loop++;
	      }
	    }
	    if(a == 2) {
	      if(a < level[MAZE]) {
		file = fopen("level3.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Maze Game Level 3");
		system("xdg-open findcandy.gfn");
	      }
	      else {
		loop++;
	      }
	    }
	    if(loop == 1) {
	      Incubator(d, level[LVL]);
	      
	      //system("xdg-open data_structs.pdf");
	      read2array(file, gamearray);
	      result = playMaze(gamearray, gamewin);
	      atexit(SDL_Quit);
	      SDL_DestroyWindow(gamewin.win);
	      if(result > 0) {
		level[MAZE]++;
		level[LVL]++;
		if(result > score[MAZE*LVL+a]) {
		  score[MAZE*LVL+a] = result;
		  UpdateHiScores(score);
		}
	      }
	      Incubator(d, level[LVL]);
	    }
	    loop--;
	  }
	  if(game == BBALL) {
	    if(a == 0) {
	      if(a < level[BBALL]) {
		file = fopen("Basketball1.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Basketball Game Level 1");
	      }
	      else {
		loop++;
	      }
	    }
	    if(a == 1) {
	      if(a < level[BBALL]) {
		file = fopen("Basketball2.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Basketball Game Level 2");
	      }
	      else {
		loop++;
	      }
	    }
	    if(a == 2) {
	      if(a < level[BBALL]) {
		file = fopen("Basketball2.txt", "r");
		filechecker(file);
		Neill_SDL_Init(&gamewin, "Basketball Game Level 3");
	      }
	      else {
		loop++;
	      }
	    }
	    if(loop == 1) {
	      Incubator(d, level[LVL]);
	      read2array(file, gamearray);
	      playMaze(gamearray, gamewin);
	    }
	    loop--;
	  }
	} 
	if(a == GOBACK) {
	  Incubator(d, level[LVL]);
	  loop = 0;
	}
      }
    }
  }
  return 0;
}

void UpdateLevelFile(int level[LVLARR])
{
  FILE *lvlfile;
  lvlfile = fopen("level.txt", "w");
  fprintf(lvlfile, "%d %d %d", level[JUMP], level[MAZE], level[BBALL]);
  fclose(lvlfile);
}

void UpdateHiScores(int score[SCORES])
{
  FILE *scoresfile;
  scoresfile = fopen("hiscores.txt", "w");
  fprintf(scoresfile, "%d %d %d %d %d %d %d %d %d", score[J1], score[J2], score[J3], score[M1], score[M2], score[M3], score[B1], score[B2], score[B3]);
  fclose(scoresfile);
}

// Checks the file exists.
int filechecker(FILE * file)
{
    if (!file) {
	fprintf(stderr, "Can't read the file.\n");
	exit(1);
    }
    return 0;
}
