
#include "programagotchi.h"
#include "interpreter.h"


// move object on the display grid //
int moveobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, int rowshift, int colshift) {
  int row, col, newrow, newcol;
  char nextchar;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      //find current location of the 'selected object'
      if (displaygrid[row][col] == selectedobj) {
	
	  //-------------------------------------------------------------------------------------//
	  //-------------------------APPLY COLLISION RULES---------------------------------------//
	  //-------------------------------------------------------------------------------------//
	  //---get new position and check in grid range---//
	  newrow = row + rowshift; newcol = col + colshift;
	  if (newrow < 0 || newrow >= HEIGHT || newcol < 0 || newcol >= WIDTH) { 
	    if (selectedobj == GOTCHI) {
	      return HIT_OBSTACLE; 
	    }
	    else { //ball or some other object
	      return GENERAL_STOP;
	    }
	  }
	  //get character currently occupying new position
	  nextchar = displaygrid[newrow][newcol];
	  //--run into a hazard--//
	  //GOTCHI: Delete GOTCHI from grid and end current process. 
	  //OTHER (e.g. ball): remove hazard and keep going
	  if (nextchar == HAZARD) {
	    displaygrid[row][col] = '.';
	    if (selectedobj == GOTCHI) {
	      return ON_HAZARD;
	    }
	    else {
	      displaygrid[newrow][newcol] = selectedobj;
	      return NO_ACTION;
	    }   
	  }
	  //--run into an exit--//
	  //GOTCHI: Delete GOTCHI from grid and end current process. 
	  //OTHER (e.g. ball): just stop and end current process (don't move onto exit)
	  else if (nextchar == EXIT_POINT) {
	    if (selectedobj == GOTCHI) {
	      displaygrid[row][col] = FREE_SPACE;
	      return ON_EXIT;
	    }
	    else {
	      return NO_ACTION;
	    }
	  }
	  //--ball hits 'net' (specific to basketball game)--//
	  //remove ball from grid and return a 'win'
	  else if (selectedobj == BALL && nextchar == NET) {
	    displaygrid[row][col] = FREE_SPACE;
	    return HIT_BASKET;
	  }
	  
	  //--run into any other obstacle--//
	  //stop object in current position. Return status depends on object in question
	  else if (nextchar != FREE_SPACE) {
	    if (selectedobj == GOTCHI) {
	      return HIT_OBSTACLE; 
	    }
	    else { //ball or some other object
	      return GENERAL_STOP;
	    }
	  }
	  //--free space--//
	  //move along
	  else {
	    displaygrid[newrow][newcol] = selectedobj;
	    displaygrid[row][col] = '.';
	    return NO_ACTION;
	  }
	  //----------------------------------------------------------------------------//
	  
      }
    }
  }
  //shouldn't reach here
  return NO_ACTION;
}



// add object to the display grid (next to the GOTCHI) //
int addobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, Direction dir) {
  int row, col, newrow = 0, newcol = 0;
  //find the current location of the GOTCHI
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      if (displaygrid[row][col] == 'G') { //find GOTCHI
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
	//error if new object location goes off edge or overlaps with another object
	if (newrow < 0 || newrow >= HEIGHT || newcol < 0 || newcol >= WIDTH) {
	  printf("\nnot enough room to add object!\n");
	  return GENERAL_STOP; 
	}
	if (displaygrid[newrow][newcol] != FREE_SPACE) {
	  printf("\nnot enough room to add object!\n");
	  return GENERAL_STOP; 
	}
	//add the object
	displaygrid[newrow][newcol] = selectedobj;
	return NO_ACTION;
      }
    }
  }
  //shouldn't reach here
  return NO_ACTION;  
}



//--'eat' candy adjacent to the gotchi--//
//candy must be directly adjacent on left or right of current GOTCHI position
int eatcandy(char displaygrid[HEIGHT][WIDTH]) {
  int row, col;
  //find the current location of the GOTCHI
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {
      if (displaygrid[row][col] == 'G') { 
	//check left of GOTCHI for candy
	if (col > 0) {
	  if (displaygrid[row][col-1] == CANDY) {
	    displaygrid[row][col-1] = FREE_SPACE;
	    return ATE_CANDY;
	  }
	}
	//check right of GOTCHI for candy
	if (col < WIDTH-1) {
	  if (displaygrid[row][col+1] == CANDY) {
	    displaygrid[row][col+1] = FREE_SPACE;
	    return ATE_CANDY;
	  }
	}
	//no candy if fcn reaches here
	printf("\nNo candy to eat!\n");
	return GENERAL_STOP;
      }
    }
  }
  //shouldn't reach here
  return NO_ACTION;  
}



//-------'TEST' FUNCTIONS------//
//UPDATE: NEED FILLING IN!!!!


//check if a particular object character is on the display grid //
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


int obstacle_adjacent(char displaygrid[HEIGHT][WIDTH],Direction dir,char selectedobj) {
  
  //UPDATE: FILL CONTENT
  return FAIL;

}
  
  
int hazard_adjacent(char displaygrid[HEIGHT][WIDTH],Direction dir,char selectedobj) {
  
  //UPDATE: FILL CONTENT
  return FAIL;

}
  

int candy_adjacent(char displaygrid[HEIGHT][WIDTH],char selectedobj) {
  
  //UPDATE: FILL CONTENT
  return FAIL;  
  
}
  

  

