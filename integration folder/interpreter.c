// assumes only one of each 'object' type on the board at a time (e.g. one ball)

//start with ONLY COMMAND LINE (one 'line' at a time) - then add FILE READING etc.
//start With only 'MOVE' and maybe 'ADD' (object) - get working then add other 'function' stuff

#include "programagotchi.h"

#define WHITE 255,255,255
#define BLACK 0,0,0
#define BLUE 0,0,255
#define MIN(a,b) ((a) < (b) ? (a) : (b)) /* standard min fcn (net/lecture notes) */
#define DELAY 50
#define NULL_CHAR 'N'

enum direction {left,right,above,below};
typedef enum direction Direction;


void moveobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, int rowshift, int colshift);
void addobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, Direction dir);
int objectongrid(char displaygrid[HEIGHT][WIDTH], char selectedobj);
void RemoveSpaces(char *inputstr, char *newstr);


//----parse individual 'command' lines (from fcn file OR terminal input)-----//
int runcommand(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *commandstr) {

  char selectedobj = NULL_CHAR;
  char formattedstr[STRLEN], filestr[STRLEN], *i;
  int n = 0, rowshift = 0, colshift = 0, distance = 1, numchars, j, ret;
  Direction dir = right; 
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
      SDL(displaygrid,"",sw);
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
	return BAD_COMMAND;
      }
    }
    //no direction specifier - assume right-hand side placement
    else {
      dir = right;
    }
    //add the object to grid and refresh display
    addobject(displaygrid,selectedobj,dir);       
    SDL(displaygrid,"",sw);
  }
   
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
      if (ret == BAD_COMMAND) {
	printf("\nERROR in runcommand: Problem parsing file WHATEVER\n");
	return BAD_COMMAND;
      }
    }
  }
  return NO_ACTION;
}



// move object on the display grid //
void moveobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, int rowshift, int colshift) {
  int row, col, newrow, newcol;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      //find the CURRENT location of the 'selected object'
      if (displaygrid[row][col] == selectedobj) {
	  newrow = row + rowshift; newcol = col + colshift;
	  // no action if move would go 'off edge' or overlap with an existing object 
	  if (newrow < 0 || newrow >= HEIGHT || newcol < 0 || newcol >= WIDTH || displaygrid[newrow][newcol] != '.') {
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
void addobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, Direction dir) {
  int row, col, newrow = 0, newcol = 0;
  //find the current location of the GOTCHI
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
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
	if (newrow < 0 || newrow >= HEIGHT || newcol < 0 || newcol >= WIDTH || displaygrid[newrow][newcol] != '.') {
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
int objectongrid(char displaygrid[HEIGHT][WIDTH], char selectedobj) {
  int row, col;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {  
      if (displaygrid[row][col] == selectedobj) {
	return 1; //on grid!
      }
    }
  }
  return 0; //not on grid!
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
