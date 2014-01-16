// assumes only one of each 'object' type on the board at a time (e.g. one ball)

//start with ONLY COMMAND LINE (one 'line' at a time) - then add FILE READING etc.
//start With only 'MOVE' and maybe 'ADD' (object) - get working then add other 'function' stuff

#include "programagotchi.h"
#include "interpreter.h"

#define DELAY 50
#define NULL_CHAR 'N'

void RemoveSpaces(char *inputstr, char *newstr);


//----parse individual 'command' lines (from fcn file OR terminal input)-----//
int runcommand(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *commandstr) {

  char selectedobj = NULL_CHAR;
  char formattedstr[STRLEN], filestr[STRLEN], *i;
  int n = 0, rowshift = 0, colshift = 0, distance = 1, numchars, j, ret;
  Direction dir = right; 
  FILE *ftemp;
  
  //debug
  printf("\nRUNCOMMAND: Input Fcn as follows: %s",commandstr);
  
  //define all 'objects' that may be added grid
  const char *objstrings[5] = {"gotchi","ball",NULL,NULL,NULL};
  const char objcodes[5] = {GOTCHI,BALL,'\0','\0','\0'};
  
  //remove spaces and newlines from input string
  RemoveSpaces(commandstr,formattedstr);
  i = formattedstr;
  
  //--------PARSE 'MOVE' COMMANDS----------//
  if (strstr(i,"move") == i || strstr(i,"swim") == i) { 
    i = i + 4;
    //if the command goes straight to a 'direction' (no object identifier), then assume GOTCHI is being moved
    if (strstr(i,"up") == i || strstr(i,"down") == i || strstr(i,"left") == i || strstr(i,"right") == i) {
      selectedobj = GOTCHI;
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
    
    printf("\ncurrent str at i: %s",i); 
    
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
    
    //------APPLY THE MOVE-------//
    //no distance specified - 
    //if (
    
    // distance specified- loop depending on the 'distance' for GOTCHI to travel
    //else {
      for (j = 1; j <= distance; j++) {
	ret = moveobject(displaygrid,selectedobj,rowshift,colshift);
	if (ret < NO_ACTION) { //status back from attempted move
	  return ret;
	}
	SDL(displaygrid,"",NO_SCORE,sw);
	SDL_Delay(DELAY);
      }
    //}
    
    
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
    ret = addobject(displaygrid,selectedobj,dir);       
    if (ret < NO_ACTION) {
      return ret; //status back from attempted add
    }
    SDL(displaygrid,"",NO_SCORE,sw);
  }
   
  //--------PARSE OTHER COMMANDS----------// 
  //---eat candy (adjacent to GOTCHI---//
  else if (strstr(i,"eat") == i) {
    ret = eatcandy(displaygrid);
    if (ret < NO_ACTION) {
      return ret; //status back from attempted eat
    }    
  }
  //----quit the current game----//
  else if (strstr(i,"quit") == i) {
    return QUIT_COMMAND;
  }
   
  //------NO 'BASE COMMAND' RECOGNISED---------//
  //revert to custom function files
  else {
    
    printf("\nTEST");
    printf("\ncommandstr: %s",commandstr);
    
    //check for '.gfn' (gotchi function) file in current folder matching the command
    //if none matching, then this must be a bad command
    strcpy(filestr,commandstr);
    strcat(strtok(filestr," \t\n\0"),".gfn");

    printf("\nAttempting to open file: %s",filestr);
    
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
      /* any status codes below 'NO_ACTION' means that the current 'move'
       * should come to an end, and the status message should be 
       * passed all the way back up to the original caller to 
       * 'runcommand' (probably one of the game modules) */
      if (ret < NO_ACTION) {
	return ret;
      }
    }
  }
  return NO_ACTION;
}



//remove spaces (and tabs) from a string
//UPDATE: Also removes newline characters
void RemoveSpaces(char *inputstr, char *newstr)
{
  //makes a new string and copies non-space characters across
  //returns pointer to the new string
  char *i = inputstr;
  char *j = newstr;
  while(*i != '\0') {
    if (*i != ' ' && *i != '\t' && *i != '\n') 
    {
      *j = *i;
      j++;
    }
    i++; 
  }
  //add end of string to new
  *j = '\0';
}
