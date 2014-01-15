
//UPDATE: DECLARATION ABOUT USING C ASSIGNMENT PARSER HERE!!!
//WARN OTHERS READING THIS NOT TO USE ANY CODE ELSEWHERE!!!!!!!!!

//UPDATE: ADD WARNINGS THROUGHOUT TO AVOID USING CODE ELSEWHERE!!!


#include "programagotchi.h"
#include "interpreter.h"

//
struct varstruct {
  char name[STRLEN];
  char value[STRLEN];
  struct varstruct *next;
};
typedef struct varstruct varstruct;

//
struct globstruct {
  FILE *fin;
  SDL_Simplewin sw;
  char *commandstr;
  varstruct *varlist;
};
typedef struct globstruct globstruct;

int TOP(globstruct *globals, char commandstr[STRLEN]);
int BLOCK(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec);
int DO(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec);
int WHILE(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec);
int IF(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec);
int TEST(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line);
char *getnext(globstruct *globals);
void addvar(globstruct *globals, char *name, char *val);
char *replacevars(varstruct *varlist, char *str);
char *skipspace(char *str);


int parsefcn(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *filestr, char commandstr[400]) {

  FILE *fin;
  int line;

  //open the file (should have already been validated)
  if ((fin = fopen(filestr,"r")) == NULL) {
    return BAD_COMMAND;
  }  
  
  //initialise 'global information' (for this file)
  globstruct *globals = malloc(sizeof(globstruct));
  globals -> fin = fin;
  globals -> sw = sw;
  globals->commandstr = commandstr;
  globals -> varlist = NULL;
  
  //parse top line
  line = TOP(globals,commandstr);
  if (line < NO_ACTION) { //error parsing 'TOP'
    return line; 
  }
  
  //parse body
  //either returns a line number of a useful status code <= 0 
  line = BLOCK(globals,displaygrid,getnext(globals),line,0,0);
  if (line > NO_ACTION) {
    return NO_ACTION;
  }
  return line;
  
}



int TOP(globstruct *globals, char commandstr[]) {

  char str[STRLEN], cstr[STRLEN], fline[STRLEN];
  char *i, *a, *b;
  int line = 0;
  
  //find function top-line
  do { 
    //
    line++;
    if (fgets(str,STRLEN,globals->fin) == NULL) {
      printf("\nERROR: No 'function' line in the file!\n");
      return BAD_COMMAND;
    }
    i = skipspace(str);
    //can only accept comments or blankspace above the function line
    if (strstr(i,"function") != i && strstr(i,"//") != i && *i != '\n' && *i != '\0') {
      printf("\nERROR: Only comments can appear above the 'function' line!\n");
      return BAD_COMMAND;
    }
  } while (strstr(i,"function") != i);
  i += 8;

  //parse function top-line//
  strcpy(cstr,commandstr);
  strcat(cstr,"\n"); //important i think...
  strcpy(fline,i);
  a = cstr, b = fline;
  //first token should be the fcn name in both cases
  a = strtok(a," \t\n"); b = strtok(b," \t\n");
  if (strcmp(a,b) != 0) {
    printf("\nERROR: Can't find a function called %s in the .gfn file\n",cstr);
    return BAD_COMMAND;
  }
  //move along then find the next token
  a += strlen(a) + 1; b += strlen(b) + 1; 
  a = strtok(a," \t\n"); b = strtok(b," \t\n");
  //step through tokens 'in synch', adding input variables as you go
  while (a != NULL && b != NULL) {
    addvar(globals,b,a); //add the new variable to linked list here!
    a += strlen(a) + 1; b += strlen(b) + 1;
    a = strtok(a," \t\n"); b = strtok(b," \t\n");
  }
  //check for any 'mismatches'
  if (a != NULL) {
    printf("\nERROR: Too many inputs!\n");
    return BAD_COMMAND;
  }
  if (b != NULL) {
    printf("\nERROR: Not enough inputs!\n");
    return BAD_COMMAND;
  }  
  
  //return the line number of fcn top-line
  return line;
}



int BLOCK(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec) {
  //
  char *i = str;
  int n, ret = NO_ACTION; 
  //traverse indents at start
  for (n = 0; n < indents; n++) {
    //base case 1: decreased indentation
    if (*i != '\t') {
      return line; 
    }
    i++;
  }
  //base case 2: end of file
  if (str == NULL) {
    printf("\nEND OF FILE REACHED\n");
    //UPDATE: Set 'Warning' if robot not turned off? (will need a variable)
    return GENERAL_STOP; //not an error
  }
  printf("parsing line %d. BAREXEC = %d. str: %s",line,barexec,str);
  //do loops//
  if (strstr(i,"do") == i) {
    if ((line = DO(globals,displaygrid,str,line,indents,barexec)) < NO_ACTION) {
      return line;
    }
    //UPDATE: ONLY IGNORE RETURN (ERRORS) IF NO FURTHER STATEMENTS CAN BE REACHED
    line = BLOCK(globals,displaygrid,str,line,indents,barexec);
  }
  //conditional loops//
  else if (strstr(i,"while") == i) {
    if ((line = WHILE(globals,displaygrid,str,line,indents,barexec)) < NO_ACTION) {
      return line;
    }
    //UPDATE: ONLY IGNORE RETURN (ERRORS) IF NO FURTHER STATEMENTS CAN BE REACHED
    line = BLOCK(globals,displaygrid,str,line,indents,barexec);
  }    
  //branches//
  else if (strstr(i,"if") == i) {
    if ((line = IF(globals,displaygrid,str,line,indents,barexec)) < NO_ACTION) {
      return line;
    }
    //UPDATE: ONLY IGNORE RETURN (ERRORS) IF NO FURTHER STATEMENTS CAN BE REACHED
    line = BLOCK(globals,displaygrid,str,line,indents,barexec);
  } 
  
  //--- ACTIONS ---- PASS LINE TO 'RUN COMMAND' ---- //
  //only execute if there are no 'execution bars'
  //also make an allowance for comment lines and blank lines
  else {
    if (barexec == 0 && strstr(i,"//") != i && *i != '\n' && *i != '\0') {
      ret = runcommand(globals->sw,displaygrid,globals->commandstr);
      if (ret < NO_ACTION) { //bad command identified by 'runcommand'
	return ret; 
      }
    }
    //next line in block
    //UPDATE: ONLY IGNORE RETURN (ERRORS) IF NO FURTHER STATEMENTS CAN BE REACHED
    line = BLOCK(globals,displaygrid,getnext(globals),line+1,indents,barexec);
  }
  //-------------------------------------------------//
  
  return line; //incl. status codes if applicable
}


int DO(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec) {
  //
  char *i = str;
  int a, b, numloops, endline;
  i = skipspace(i);
  i += 2; //skip past 'do'
  numloops = strtol(i,&i,10);
  //verify number of loops (>0) and syntax
  if (numloops <= 0 || (*i != ':' && *i != ' ' && *i != '\t' && *i != '\0' && *i != '\n')) {
    printf("\nERROR: Couldn't find a valid positive number after 'do'\n");
    return BAD_COMMAND;
  }
  i = skipspace(i);
  if (*i != ':') {
    printf("\nERROR: Expecting ':' after number of loops in 'do'\n");
    return BAD_COMMAND; 
  }
  //run the 'inner block' for the first time, pass any errors/stops back up
  if ((endline = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec)) < NO_ACTION) {
    return endline;
  }
  //loop further inner blocks, 'rewind' file before each iteration
  for (a = 1; a < numloops; a++) {
    rewind(globals->fin);
    for (b = 0; b < line; b++) {
      fgets(str,STRLEN,globals->fin); 
    }
    if ((endline = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec)) < NO_ACTION) {
      return endline;
    }
  }
  //all loops complete
  return endline; 
}


int WHILE(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec) {
  //
  char *i = str;
  char teststr[STRLEN];
  int n, result, endline = NO_ACTION;
  i = skipspace(i);
  i += 5; //skip past 'while'
  strcpy(teststr,i); //need to retain for later in the fcn
  result = TEST(globals,displaygrid,teststr,line);
  //first time through 
  //if test fails, still need to run through inner block (for parsing), but bar execution
  switch (result) {
    case BAD_COMMAND:
      return BAD_COMMAND; //bad syntax in test
      break;
    case FAIL:
      endline = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec+1);
      break;
    case PASS:
      endline = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec);
      break;
  }
  if (endline < NO_ACTION) { //error or stop in block
    return endline;
  }
  //loop until test fails, 'rewind' file before each iteration
  //note: can assume that test won't return syntax errors (already validated above)
  while (TEST(globals,displaygrid,teststr,line) == 1 && barexec == 0) {
    rewind(globals->fin);
    for (n = 0; n < line; n++) {
      fgets(str,STRLEN,globals->fin);
    }
    if ((endline = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec)) < NO_ACTION) {
      return endline;
    }
  }
  //all loops complete
  return endline; 
}


int IF(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line, int indents, int barexec) {
  //similar to first pass of WHILE loop
  //some code repetition but simpler than having a seperate sub-fcn
  char *i = str;
  int n, result;
  i = skipspace(i);
  i += 2; //skip past 'if'
  result = TEST(globals,displaygrid,i,line);
  //if test fails, still need to run through inner block (for parsing), but bar execution
  switch (result) {
    case BAD_COMMAND:
      return BAD_COMMAND; //bad syntax in test
      break;
    case FAIL:
      line = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec+1);
      break;
    case PASS:
      line = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec);
      break;
  }
  if (line < NO_ACTION) { //error in block
    return line;
  }
  //------'ELSE' STATEMENTS-------//
  //'str' should contain the correct line when BLOCK (above) returns
  //traverse (expected) indents
  i = str;
  for (n = 0; n < indents; n++) {
    if (*i != '\t') {
      return line; 
    }
    i++;
  }
  //
  if (strstr(i,"else") == i) {
    printf("parsing line %d. BAREXEC = %d. str: %s",line,barexec,str);
    //validate ':' following else
    i += 4;
    i = skipspace(i);
    if (*i != ':') {
      printf("\nERROR: Expecting ':' following 'else'\n");
      return BAD_COMMAND;
    }
    //run inner block: toggle execution as necessary
    //same test result as 'if' above - but OPPOSITE 'barexec' response
    if (result == 0) {
      line = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec);
    } 
    else {
      line = BLOCK(globals,displaygrid,getnext(globals),line+1,indents+1,barexec+1);
    }
    if (line < NO_ACTION) { //error in block
      return line;
    } 
  }
  //all done
  return line;
}


int TEST(globstruct *globals, char displaygrid[HEIGHT][WIDTH], char *str, int line) {
  //
  char *i;
  int result;
  i = skipspace(str);
  //------------OBSTACLES------------//
  //UPDATE: COULD ADD 'CLEAR' VERSIONS LATER!!
  if (strstr(i,"obstacle_left") == i) {
    result = obstacle_adjacent(displaygrid,left,GOTCHI);
    i += strlen("obstacle_left");
  }
  else if (strstr(i,"obstacle_right") == i) {
    result = obstacle_adjacent(displaygrid,right,GOTCHI);
    i += strlen("obstacle_right");
  }
  else if (strstr(i,"obstacle_above") == i) {
    result = obstacle_adjacent(displaygrid,above,GOTCHI);
    i += strlen("obstacle_above");
  }
  else if (strstr(i,"obstacle_below") == i) {
    result = obstacle_adjacent(displaygrid,below,GOTCHI);
    i += strlen("obstacle_below");
  }
  //----------BALL OBSTACLES------------//
  else if (strstr(i,"obstacle_left_of_ball") == i) {
    result = obstacle_adjacent(displaygrid,left,BALL);
    i += strlen("obstacle_left_of_ball");
  }
  else if (strstr(i,"obstacle_right_of_ball") == i) {
    result = obstacle_adjacent(displaygrid,right,BALL);
    i += strlen("obstacle_right_of_ball");
  }
  else if (strstr(i,"obstacle_above_ball") == i) {
    result = obstacle_adjacent(displaygrid,above,BALL);
    i += strlen("obstacle_above_ball");
  }
  else if (strstr(i,"obstacle_below_ball") == i) {
    result = obstacle_adjacent(displaygrid,below,BALL);
    i += strlen("obstacle_below_ball");
  }
  //------------HAZARDS------------//
  //UPDATE: COULD ADD 'CLEAR' VERSIONS LATER!!
  if (strstr(i,"hazard_left") == i) {
    result = hazard_adjacent(displaygrid,left,GOTCHI);
    i += strlen("hazard_left");
  }
  else if (strstr(i,"hazard_right") == i) {
    result = hazard_adjacent(displaygrid,right,GOTCHI);
    i += strlen("hazard_right");
  }
  else if (strstr(i,"hazard_above") == i) {
    result = hazard_adjacent(displaygrid,above,GOTCHI);
    i += strlen("hazard_above");
  }
  else if (strstr(i,"hazard_below") == i) {
    result = hazard_adjacent(displaygrid,below,GOTCHI);
    i += strlen("hazard_below");
  }
  //--------------CANDY-------------//
  else if (strstr(i,"next_to_candy") == i) {
    //this one checks left AND right by default (no 'dir')
    result = candy_adjacent(displaygrid,GOTCHI); 
    i += strlen("next_to_candy");
  }
  else if (strstr(i,"not_next_to_candy") == i) {
    result = (candy_adjacent(displaygrid,GOTCHI) == 0);
    i += strlen("not_next_to_candy");
  }  
  //-----------EXIT---------//
  //doesn't really make sense to have an 'on exit' test
  else if (strstr(i,"not_on_exit") == i) {
    result = FAIL; //think about it!
    i += strlen("not_on_exit");
  }  
  //anything else is invalid
  else {
    printf("\nERROR: LINE %d - Invalid Test\n",line);
    return BAD_COMMAND;
  } 
  //validate ':' following test
  i = skipspace(i);
  if (*i != ':') {
    printf("\nERROR: LINE %d - Expecting ':' after test\n",line);
    return BAD_COMMAND; 
  }
  //all good
  return result; 
}




//--get next line in file with variable replacements--//
char *getnext(globstruct *globals) {
  char str[STRLEN];
  fgets(str,STRLEN,globals->fin);
  return replacevars(globals->varlist,str);
}


//-----add new variables (and associated info) to the variables list-----//
/* Note: If the 'name' input matches an existing var name, then
 * the value of that variable will simply be replaced with the 'val' input. 
 * i.e. later variable definitions over-write earlier variable definitions
 * (just like most other programming languages) */
void addvar(globstruct *globals, char *name, char *val) {
  //handle the first var
  varstruct *v;
  if (globals -> varlist == NULL) { 
    globals -> varlist = (varstruct *)malloc(sizeof(varstruct));
    v = globals -> varlist;
  }
  else {
    //check first node for duplicate name
    v = globals -> varlist;
    if (strcmp(v->name,name) == 0) { //duplicate
      strcpy(v -> value, val); //replace
      return;
    }
    //run through rest of list and check for duplicates
    while (v -> next != NULL) {
      v = v -> next;
      if (strcmp(v->name,name) == 0) { //duplicate
	strcpy(v -> value, val); //replace
	return;
      }
    }
    //no existing matches: add new variable to list
    v -> next = (varstruct *)malloc(sizeof(varstruct));
    v = v -> next;
  } 
  //populate the fields
  strcpy(v -> name, name);
  strcpy(v -> value, val);
  v -> next = NULL; 
} 



//---replace variable names in a str with corresponding values---//
char *replacevars(varstruct *varlist, char *str) {
  //
  varstruct *v = varlist;
  char *newstr = NULL, *a, *b, *i;
  int n, replacevar;
  //run through each variable in the list
  while (v != NULL) {
    //new memory needed for each variable 'sweep'
    newstr = (char *)malloc(STRLEN*sizeof(char));
    a = i = str;
    b = newstr;
    //run through all occurences of the CURRENT variable in str
    while (strstr(i,v->name) != NULL) {
      i = strstr(i,v->name);
      //copy characters across to newstr 
      //up to, but not including, this variable occurrence
      while (a < i) {
	*b = *a;
	a++; b++;
      }
      //move to next occurrence
      i += strlen(v->name);
      a = i;
      //determine whether var replacement should go ahead
      //(basically if the match is in the middle of a larger alphanum string, then don't replace)
      if (isalnum(*a) == 0) {
	if (b == newstr) { //start of line 
	  replacevar = 1;
	} 
	else if (isalnum(*(--b)) == 0) {
	  replacevar = 1; b++;
	}
	else {
	  replacevar = 0; b++;
	}
      }
      else replacevar = 0;
      //copy var value or name across to 'newstr'
      //(depending on decision above)
      if (replacevar == 1) {
	for (n = 0; n < strlen(v->value); n++) {
	  *b = v -> value[n];
	  b++;
	}
      } else {
	for (n = 0; n < strlen(v->name); n++) {
	  *b = v -> name[n];
	  b++;
	}
      }
    }
    //remainder of string following all occurrences
    //(including \0)
    strcpy(b,i);
    //next variable
    v = v -> next;
    str = newstr; //important
  }
  return newstr;
}
  
  
//traverse spaces and tabs in a string//
char *skipspace(char *str) {
  char *i = str;
  while (*i == ' ' || *i == '\t') {
    i++;
  }
  return i;
}
