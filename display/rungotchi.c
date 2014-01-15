    // assumes only one of each 'object' type on the board at a time (e.g. one ball)

//start with ONLY COMMAND LINE (one 'line' at a time) - then add FILE READING etc.
//start With only 'MOVE' and maybe 'ADD' (object) - get working then add other 'function' stuff

#include "display.h"

#define WHITE 255,255,255
#define BLACK 0,0,0
#define BLUE 0,0,255
#define MIN(a,b) ((a) < (b) ? (a) : (b)) /* standard min fcn (net/lecture notes)*/
#define STR_LENGTH 100
#define DELAY 50

int move(char displaygrid[HEIGHT][WIDTH], char selectedobj, int rowshift, int colshift);
//void UpdateDisplay(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH]);
void RemoveSpaces(char *inputstr, char *newstr);


int main(int argc, char *argv[]) 
{
  
  char inputstr[STR_LENGTH], formattedstr[STR_LENGTH];
  char selectedobj;
  char displaygrid[HEIGHT][WIDTH];
  char *i;
  int n, j, row, col, rowshift, colshift;
  int distance, numchars;
  //SDL_Simplewin sw;
  FILE *fin;

  const char *objstrings[5] = {"gotchi","ball",NULL,NULL,NULL};
  const char objcodes[5] = {'G','B','\0','\0','\0'};

  //Init SDL
  //Neill_SDL_Init(&sw);

  //Read initial grid from file (no need to validate)
  fin = fopen("testgrid.txt","r");
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH+1; col++) { // +1 to include '/n' column
      fscanf(fin,"%c",&displaygrid[row][col]); 
    }
  }
  fclose(fin);
  //UpdateDisplay(sw,displaygrid);
  SDL (displaygrid);
  
  
  //---LOOP UNTIL CLOSED -- GET ENDLESS COMMANDS FROM USER---//
  while (1) {
  
    //re-initialise vars
    selectedobj = '\0';
    n = rowshift = colshift = 0;
    distance = 1; //assume moving by 1 unless otherwise specified
    
    //get input command - put all in while loop for multiple commands!
    printf("\nEnter Command: ");
    scanf("%s",inputstr); //UPDATE: NEED TO UPDATE TO HANDLE SPACES FROM TERMINAL INPUT!!!
    RemoveSpaces(inputstr,formattedstr); //remove spaces to simplify processing    
    i = formattedstr;
    
    //--------'MOVE' COMMANDS----------//
    if (strstr(i,"move") == i) {
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
	  }
	  n++;
	}
	//error if no matches
	if (selectedobj == '\0') {
	  //ERROR: INVALID OBJECT IDENTIFIER!
	}
	//move str pointer forwards by the length of the relevant object identifier
	i = i + strlen(objstrings[n-1]);
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
	//ERROR! INVALID DIRECTION SPECIFIER
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
	  //ERROR! INVALID SYNTAX AFTER THE INITIAL DIRECTION SPECIFIER!
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
	  //ERROR! INVALID SYNTAX AT END OF STRING!
	}
      }

      // apply the move - loop depending on the 'distance' for GOTCHI to travel
      for  (j = 1; j <= distance; j++) {
	if (move(displaygrid,selectedobj,rowshift,colshift) == 0) {
	  return 0; //some error - message handled by 'move'
	}
	//UpdateDisplay(sw,displaygrid);
	SDL (displaygrid);
	//SDL_Delay(DELAY);
      }
    }
    
    
    //--------'XXXX' COMMANDS----------//
    //similar structure to 'move' algorithm for other stuff

    //final check following all 'commands':
    //if *i still not '/0', something is wrong!
    //(some crap at the end of the string)

    //update display (regardless of command type)
  }
  
  //finish
  atexit(SDL_Quit);  
  return 0;
}



// move object on the display grid //
int move(char displaygrid[HEIGHT][WIDTH], char selectedobj, int rowshift, int colshift) {
  //output int for success/fail?
  int row, col, newrow, newcol;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      //find the CURRENT location of the 'selected object'
      if (displaygrid[row][col] == selectedobj) {
	  newrow = row + rowshift; newcol = col + colshift;
	  // no action if move would go 'off edge' or overlap with an existing object 
	  if (newrow < 0 || newrow >= HEIGHT || newcol < 0 || newcol >= WIDTH || displaygrid[newrow][newcol] != '.') {
	    return 1; //move not possible but still valid object code etc.
	  }
	  // move the selected obj and set previous location to blank
	  displaygrid[newrow][newcol] = selectedobj;
	  displaygrid[row][col] = '.';
	  return 1; 
      }
    }
  }
  //if get this far, selected object doesn't exist on the grid
  printf("Attempted to move object that hasn't been added yet!");
  return 0;
}




/*----OUTPUT BOARD TO SDL WINDOW-----*/




//remove spaces from a string
void RemoveSpaces(char *inputstr, char *newstr)
{
  //makes a new string and copies non-space characters across
  //returns pointer to the new string
  char *i = inputstr;
  char *j = newstr;
  while(*i != '\0') {
    if (*i != ' ') 
    {
      *j = *i;
      j++;
    }
    i++; 
  }
  //add end of string to new
  *j = '\0';
}
