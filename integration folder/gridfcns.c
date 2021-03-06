   
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
	  //--GOTCHI hits candy--//
	  else if (selectedobj == GOTCHI && nextchar == CANDY) {
	    displaygrid[newrow][newcol] = FREE_SPACE;
	    return ATE_CANDY;
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


//---check if a specified object is adjacent to an obstacle (such as a wall or barrier)---//
int obstacle_adjacent(char displaygrid[HEIGHT][WIDTH],Direction dir,char selectedobj) {
  //find the 'selected obj'
  int row, col;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {  
      if (displaygrid[row][col] == selectedobj) {  
	//
	switch (dir) {
	  //
	  case left:
	    if (col == 0) {
	      return PASS;
	    }
	    else if (displaygrid[row][col-1] == BORDER || displaygrid[row][col-1] == BARRIER) {
	      return PASS;
	    }
	    else return FAIL;
	    break;
	    
	  case right: 
	    if (col == WIDTH - 1) {
	      return PASS;
	    }
	    else if (displaygrid[row][col+1] == BORDER || displaygrid[row][col+1] == BARRIER) {
	      return PASS;
	    }
	    else return FAIL;
	    break;
	  
	  case above:
	    if (row == 0) {
	      return PASS;
	    }
	    else if (displaygrid[row-1][col] == BORDER || displaygrid[row-1][col] == BARRIER) {
	      return PASS;
	    }
	    else return FAIL;
	    break;  
	  
	  case below: 
	    if (row == HEIGHT - 1) {
	      return PASS;
	    }
	    else if (displaygrid[row+1][col] == BORDER || displaygrid[row+1][col] == BARRIER) {
	      return PASS;
	    }
	    else return FAIL;
	    break; 
	}
      }
    }
  }
  //shouldn't reach this far
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



//----APPLY GRAVITY TO CURRENT GAME BOARD----//
int apply_gravity(char displaygrid[HEIGHT][WIDTH], SDL_Simplewin sw) {
/*basically moves objects such as GOTCHI and BALL down until they reach a platform
 *(or trigger a 'collision rule' in moveobject, in which case the fcn will return status
 *code back to the the game, and the game will need to re-run apply_gravity until all objects
 *have reached the 'bottom')*/
int row, col, ret;
char obj;
  for (row = 0; row < HEIGHT; row++) {
    for (col = 0; col < WIDTH; col++) {  
      obj = displaygrid[row][col];
      if (obj == GOTCHI || obj == BALL) {
	//apply downward moves and catch return codes from 'collision rules'
	while (obstacle_adjacent(displaygrid,below,obj) == 0) {
	  ret = moveobject(displaygrid,obj,+1,0);
	  if (ret < NO_ACTION) {
	    return ret; //
	  }
	  //refresh display
	  SDL(displaygrid,"",NO_SCORE,0,sw);
	  SDL_Delay(DELAY);
	}
      }
    }
  }
  //all objects made it to the 'bottom'
  return NO_ACTION;
}

