// assumes only one of each 'object' type on the board at a time (e.g. one ball)

//UPDATE: More 'statuses' required for things like 'HIT_SHARK' or 'ATE_CANDY'
//(these statuses END THE MOVE and get passed back up to original caller)
//could also have local statuses that end a 'movement loop' but allow processing to continue
//(such as hitting a wall or something - doesn't need to end process but stops current 'movement loop');

//UPDATE: Need to include 'EAT' as a BASE COMMAND!
//(should be quite straightforward)
//NOTE: What if you eat something and then there are further commands in fcn or something?
//wouldn't really make sense to RETURN after eating the candy...
//so HOW DO YOU LET THE ORIGINAL GAME KNOW? (needs to update scores etc... tricky...)
//(this could apply for some other stuff that 'games' need to know about too...)


#include "interpreter.h"

#define WHITE 255,255,255
#define BLACK 0,0,0
#define BLUE 0,0,255
#define MIN(a,b) ((a) < (b) ? (a) : (b)) /* standard min fcn (net/lecture notes) */
//CHANGEHERE(STR_LENGTH IS MOVED TO H FILE AND INCREASED TO 400)
//ALSO CHANGED TO 'STRLEN'
#define DELAY 50
#define NULL_CHAR 'N'

enum direction {left,right,above,below};
typedef enum direction Direction;


void moveobject(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj, int rowshift, int colshift);
void addobject(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj, Direction dir);
int objectongrid(char displaygrid[GRID_HEIGHT][GRID_WIDTH], char selectedobj);
void UpdateDisplay(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH]);
void RemoveSpaces(char *inputstr, char *newstr);



int main(int argc, char *argv[]) 
{
  
  int row, col;
  char commandstr[STR_LENGTH];
  char displaygrid[GRID_HEIGHT][GRID_WIDTH];
  SDL_Simplewin sw;
  FILE *fin;

  //Init SDL
  Neill_SDL_Init(&sw);

  //Read initial grid from file (no need to validate)
  fin = fopen("testgrid.txt","r");
  for (row = 0; row < GRID_HEIGHT; row++) {
    for (col = 0; col < GRID_WIDTH+1; col++) { // +1 to include '/n' column
      fscanf(fin,"%c",&displaygrid[row][col]); 
    }
  }
  fclose(fin);
  UpdateDisplay(sw,displaygrid);
  
  //---LOOP UNTIL CLOSED -- GET ENDLESS COMMANDS FROM USER---//
  while (sw.finished == 0) {
    //get input command 
    printf("\nEnter Command: ");
    fgets(commandstr,STR_LENGTH,stdin);
    //remove spaces to simplify processing 
    //parse the command line
    runcommand(sw,displaygrid,commandstr); 
  }
  
  //finish
  atexit(SDL_Quit);  
  return 0;
}



//----parse individual 'command' lines (from fcn file OR terminal input)-----//
int runcommand(SDL_Simplewin sw, char displaygrid[GRID_HEIGHT][GRID_WIDTH], char *commandstr) {

  char selectedobj = NULL_CHAR;
  //CHANGEHERE(filestr)
  char formattedstr[STR_LENGTH], filestr[STR_LENGTH], *i;
  int n = 0, rowshift = 0, colshift = 0, distance = 1, numchars, j, ret;
  Direction dir = right; 
  //CHANGEHERE
  FILE *ftemp;
  
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
	return BAD_COMMAND;
      }
      //match found but object not added to display grid yet
      if (objectongrid(displaygrid,selectedobj) == 0) {
	printf("\nERROR: Need to add object before you can move it!\n");
	return BAD_COMMAND;
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
      return BAD_COMMAND;
    }
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
	return BAD_COMMAND;
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
	return BAD_COMMAND;
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
      return BAD_COMMAND;
    }
    //match found, but object has already been added to grid
    if (objectongrid(displaygrid,selectedobj) == 1) {
      printf("\nERROR: Object has already been added!\n");
      return BAD_COMMAND;
    }
    //end of string not reached - expect 'direction' specifier
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
	return BAD_COMMAND;
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
  
  //CHANGEHERE(COPY WHOLE SECTION!)
  //------NO 'BASE' COMMAND RECOGNISED---------//
  //revert to custom function files
  else {
    //check for '.gfn' (gotchi function) file in current folder matching the command
    //if none matching, then this must be a bad command
    strcpy(filestr,commandstr);
    strcat(strtok(filestr," \t\n\0"),".gfn");
    if ((ftemp = fopen(filestr,"r")) == NULL) {
      printf("\nERROR: Command not recognised\n");
      return BAD_COMMAND;    
    }
    //if match found, parse the file as a 'function'
    //note: use unformatted command str (without spaces removed)
    else {
      fclose(ftemp);
      ret = parsefcn(sw,displaygrid,filestr,commandstr);
      //--------------HANDLE THE 'STATUS CHAIN'-----------------//
      /* anything apart from SUCCESS means that the current 'move'
       * should come to an end, and the status message should be 
       * passed all the way back up to the original caller to 
       * 'runcommand' (probably one of the game modules) */
      if (ret != SUCCESS) {
	//for 'bad commands' also add error message to the 'stack'
	if (ret == BAD_COMMAND) {
	  printf("\nERROR in runcommand: Problem parsing file WHATEVER\n");
	}
	return ret;
      }
    }
  }
  return SUCCESS;
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
