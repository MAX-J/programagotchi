// assumes only one of each 'object' type on the board at a time (e.g. one ball)

//start with ONLY COMMAND LINE (one 'line' at a time) - then add FILE READING etc.
//start With only 'MOVE' and maybe 'ADD' (object) - get working then add other 'function' stuff

#include "programagotchi.h"

#define WHITE 255,255,255
#define BLACK 0,0,0
#define BLUE 0,0,255
#define MIN(a,b) ((a) < (b) ? (a) : (b)) /* standard min fcn (net/lecture notes) */
#define STR_LENGTH 100
#define DELAY 50
#define NULL_CHAR 'N'

enum direction {left,right,above,below};
typedef enum direction Direction;


void moveobject(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj, int rowshift, int colshift);
void addobject(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj, Direction dir);
int objectongrid(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj);
void UpdateDisplay(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH]);
void RemoveSpaces(char *inputstr, char *newstr);



//----parse individual 'command' lines (from fcn file OR terminal input)-----//
void runcommand(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *commandstr) {

  char selectedobj = NULL_CHAR;
  char formattedstr[STR_LENGTH], *i;
  int n = 0, rowshift = 0, colshift = 0, distance = 1, numchars, j;
  Direction dir = right; 
  
  //define all 'objects' that may be added grid
  const char *objstrings[5] = {"gotchi","ball",NULL,NULL,NULL};
  const char objcodes[5] = {'G','B','\0','\0','\0'};
  
  //remove spaces and newlines from input string
  RemoveSpaces(commandstr,formattedstr);
  i = formattedstr;
  
  //--------PARSE 'MOVE' COMMANDS----------//
  if (strstr(i,"move") == i || strstr(i,"swim") == i) { //UPDATE: NOT GOOD!! SWIM NEEDS TO BE RESTRICTED TO SHARK GAME ONLY!!
    i = i + 4;
    //if the command goes straight to a 'direction' (no object identifier), then assume GOTCHI is being moved
    if (strstr(i,"up") == i || strstr(i,"down") == i || strstr(i,"left") == i || strstr(i,"right") == i) {
      selectedobj = 'G';
    }
    //otherwise there should be a valid 'object identifier' next
    else {
      //scroll list of valid 'object strings' and match with command
      //set the associated code character as 'selected object' when found
      while (objstrings[n] != NULL) {
	if (strstr(i,objstrings[n]) == i) {
	  selectedobj = objcodes[n];  
	  i = i + strlen(objstrings[n]);
	}
	n++;
      }
      //no match for specified object!
      if (selectedobj == NULL_CHAR) {
	printf("\nERROR: Invalid object!\n");
	return;
      }
      //match found but object not added to display grid yet
      if (objectongrid(displaygrid,selectedobj) == 0) {
	printf("\nERROR: Need to add object before you can move it!\n");
	return;
      }
    }
    //handle the 'initial direction' part of the string//
    if (strstr(i,"up") == i) {
      rowshift = -1;
      i = i + 2;
    } 
    else if (strstr(i,"down") == i) {
      rowshift = +1;
      i = i + 4;
    }
    else if (strstr(i,"left") == i) {
      colshift = -1;
      i = i + 4;
    }
    else if (strstr(i,"right") == i) {
      colshift = +1;
      i = i + 5;
    }
    else {
      printf("ERROR: Expecting up, down, left or right");
      return;
    }
    
    //UPDATE: NOT CHECKING END OF STRING PROPERLY!! CATCHES ERRORS WHEN IT SHOULDN'T!
    
    //if end of string not reached, handle additional parts
    if (*i != '\0') {
      //digit next - means direction distance specified here
      if (isdigit(*i)) {
	sscanf(i,"%d%n",&distance,&numchars);
	i = i + numchars;
      }
      //if not a number, then expect further direction instructions
      else if (strstr(i,"andup") == i) {
	rowshift = -1;
	i = i + 5;
      } 
      else if (strstr(i,"anddown") == i) {
	rowshift = +1;
	i = i + 7;
      }
      else if (strstr(i,"andleft") == i) {
	colshift = -1;
	i = i + 7;
      }
      else if (strstr(i,"andright") == i) {
	colshift = +1;
	i = i + 8;
      }
      else {
	printf("ERROR: Expecting 'and up', 'and down', 'and left' or 'and right', or a number");
	return;
      }
    }
    //if end of str STILL not reached, expect number at end representing 'distance'
    if (*i != '\0') {
      //digit next - means direction distance specified here
      if (isdigit(*i)) {
	sscanf(i,"%d%n",&distance,&numchars);
	i = i + numchars;
      }
      else {
	printf("ERROR: Expecting a positive integer at end of command");
	return;
      }
    }
    // apply the move - loop depending on the 'distance' for GOTCHI to travel
    for (j = 1; j <= distance; j++) {
      moveobject(displaygrid,selectedobj,rowshift,colshift);
      UpdateDisplay(sw,displaygrid);
      SDL_Delay(DELAY);
    }
  }
  
  //--------PARSE 'ADD' COMMANDS----------//
  else if (strstr(i,"add") == i) {
    i = i + 3;
    //scroll list of valid 'object strings' and match with command
    //add the associated 'character code' to the board if matched, otherwise no action
    while (objstrings[n] != NULL) {
      if (strstr(i,objstrings[n]) == i) {
	selectedobj = objcodes[n];
	i = i + strlen(objstrings[n]);
      }
      n++;
    }
    //no match for the specified object
    if (selectedobj == NULL_CHAR) {
      printf("\nERROR: Invalid object!\n");
      return;
    }
    //match found, but object has already been added to grid
    if (objectongrid(displaygrid,selectedobj) == 1) {
      printf("\nERROR: Object has already been added!\n");
      return;
    }
    //end of string not reached - expect 'direction' specifier
    
    //UPDATE: NOT CHECKING END OF STRING PROPERLY!! CATCHES ERRORS WHEN IT SHOULDN'T!
    
    if (*i != '\0') {
      if (strstr(i,"left") == i) {
	dir = left;
	i = i + 4;
      } 
      else if (strstr(i,"right") == i) {
	dir = right;
	i = i + 5;
      }
      else if (strstr(i,"above") == i) {
	dir = above;
	i = i + 5;
      }
      else if (strstr(i,"below") == i) {
	dir = below;
	i = i + 5;
      }
      else {
	printf("\nERROR: Expecting left, right, above or below\n");
	return;
      }
    }
    //no direction specifier - assume right-hand side placement
    else {
      dir = right;
    }
    //add the object to grid and refresh display
    addobject(displaygrid,selectedobj,dir);       
    UpdateDisplay(sw,displaygrid);
  }
  
  
  //------NO 'BASE' COMMAND RECOGNISED---------//
  else {
    
    //---UPDATE: FIRST SEARCH FOR VALID FUNCTIONS (.txt files) THAT MATCH THE STRING!-----//
    // CALL THE 'FUNCTION READER' (which will end up referring back to this parser!)
    
    //otherwise...
    printf("\nERROR: Command not recognised\n");
    return;    
  
  }
  
}



// move object on the display grid //
void moveobject(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj, int rowshift, int colshift) {
  int row, col, newrow, newcol;
  for (row = 0; row < GRID_HEIGHT; row++) {
    for (col = 0; col < GRID_WIDTH; col++) {
      //find the CURRENT location of the 'selected object'
      if (displaygrid[row][col] == selectedobj) {
	  newrow = row + rowshift; newcol = col + colshift;
	  // no action if move would go 'off edge' or overlap with an existing object 
	  if (newrow < 0 || newrow >= GRID_HEIGHT || newcol < 0 || newcol >= GRID_WIDTH || displaygrid[newrow][newcol] != '.') {
	    return; 
	  }
	  // move the selected obj and set previous location to blank
	  displaygrid[newrow][newcol] = selectedobj;
	  displaygrid[row][col] = '.';
	  return;
      }
    }
  }
}



// add object to the display grid (next to the GOTCHI) //
void addobject(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj, Direction dir) {
  int row, col, newrow = 0, newcol = 0;
  //find the current location of the GOTCHI
  for (row = 0; row < GRID_HEIGHT; row++) {
    for (col = 0; col < GRID_WIDTH; col++) {
      if (displaygrid[row][col] == 'G') {
	//set location (adjacent to GOTCHI) depending on dir
	switch (dir) {
	  case left:
	    newrow = row; newcol = col - 1;
	    break;
	  case right: 
	    newrow = row; newcol = col + 1;
	    break;
	  case above:
	    newrow = row - 1; newcol = col;
	    break;
	  case below:
	    newrow = row + 1; newcol = col;
	    break; 
	}
	//error if new object location overlaps or goes off edge
	if (newrow < 0 || newrow >= GRID_HEIGHT || newcol < 0 || newcol >= GRID_WIDTH || displaygrid[newrow][newcol] != '.') {
	  printf("\nnot enough room to add object!\n");
	  return; 
	}
	//add the object
	displaygrid[newrow][newcol] = selectedobj;
      }
    }
  }
}



//check if a particular object character is already on the display grid //
int objectongrid(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj) {
  int row, col;
  for (row = 0; row < GRID_HEIGHT; row++) {
    for (col = 0; col < GRID_WIDTH; col++) {  
      if (displaygrid[row][col] == selectedobj) {
	return 1; //on grid!
      }
    }
  }
  return 0; //not on grid!
}




/*----OUTPUT BOARD TO SDL WINDOW-----*/
void UpdateDisplay(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH])
{
  int row, col;
  SDL_Rect rectangle;
  // fix dimensions of rectangles and circles 
  rectangle.w = rectangle.h = MIN(WWIDTH/GRID_WIDTH,WHEIGHT/GRID_HEIGHT);
  const int rad = rectangle.w / 2; 
  // run through all characters in the board 
  for (row = 0; row < GRID_HEIGHT; row++) {
    for (col = 0; col < GRID_WIDTH; col++) { 
      // calculate x-y position of the corner of each square 
      rectangle.x = col*rectangle.w;
      rectangle.y = row*rectangle.h;
      /* fill all squares white */
      Neill_SDL_SetDrawColour(&sw,WHITE); 
      SDL_RenderFillRect(sw.renderer, &rectangle);
      //update display for object locations
      switch (displaygrid[row][col]) {
	//gotchi: add pink block
	case 'G':
	  Neill_SDL_SetDrawColour(&sw,BLUE); 
	  SDL_RenderFillRect(sw.renderer, &rectangle); 
	  break;
	//obstacles: add black block
	case '1':
	  Neill_SDL_SetDrawColour(&sw,BLACK); 
	  SDL_RenderFillRect(sw.renderer, &rectangle); 
	  break;
	//'ball' object: add black circle
	case 'B':
	  Neill_SDL_SetDrawColour(&sw,BLACK); 
	  Neill_SDL_RenderFillCircle(sw.renderer,rectangle.x+rad,rectangle.y+rad,rad);
	  break;
      }
    }
  }
  /* update the window 'in one go' for each board */
  SDL_RenderPresent(sw.renderer);
  SDL_UpdateWindowSurface(sw.win); 
}



//remove spaces from a string
//UPDATE: Also removes newline characters
void RemoveSpaces(char *inputstr, char *newstr)
{
  //makes a new string and copies non-space characters across
  //returns pointer to the new string
  char *i = inputstr;
  char *j = newstr;
  while(*i != '\0') {
    if (*i != ' ' && *i != '\n') 
    {
      *j = *i;
      j++;
    }
    i++; 
  }
  //add end of string to new
  *j = '\0';
}
