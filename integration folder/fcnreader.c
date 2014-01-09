//UPDATE: BETTER ERROR FCN (to include FILE NAME and line number etc.)
//ALSO: sort error wording and \n spacing so the whole 'stack' makes good sense to the user

//UPDATE: POP-UP TEXT EDITOR ON FCN CALL!
//what if fcn calls other fcns etc... what gets thrown to text editor??


#include "programagotchi.h"

struct varstruct {
  char name[STRLEN];
  char value[STRLEN];
  struct varstruct *next;
};
typedef struct varstruct varstruct;

struct loopstruct {
  int inloop;
  int startline;
  int currentloop;
  int numloops;
};
typedef struct loopstruct loopstruct;

varstruct *addvar(varstruct *varlist, char *name, char *val);
char *replacevars(varstruct *varlist, char *str);
char *skipspace(char *str);


//
int parsefcn(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *filestr, char *commandstr) {

  FILE *fin;
  varstruct *varlist = NULL;
  loopstruct loopinfo;
  loopinfo.inloop = loopinfo.startline = 0;
  loopinfo.currentloop = loopinfo.numloops = 0;
  char str[STRLEN], cstr[STRLEN], fline[STRLEN];
  char *formattedstr, *i, *a, *b, *stopchar;
  int line = 0, endfcn = 0, ret;
  
  //open the file (should have already been validated)
  if ((fin = fopen(filestr,"r")) == NULL) {
    return BAD_COMMAND;
  }

  //find function top-line
  do { 
    //
    line++;
    if (fgets(str,STRLEN,fin) == NULL) {
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
    varlist = addvar(varlist,b,a);
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
    
  //parse the function body//
  do { 
    //
    line++;
    if (fgets(str,STRLEN,fin) == NULL) {
      printf("\nERROR: 'function' without 'end function'!\n");
      return BAD_COMMAND;
    }
    //make str replacements for variables; skip leading whitespace
    formattedstr = replacevars(varlist,str);
    i = skipspace(formattedstr);
    //
    if (strstr(i,"function") == i) {
      printf("\nERROR: Nested functions are not allowed (and only one fcn per file)\n");
      return BAD_COMMAND;  
    }
     
    //loop start
    else if (strstr(i,"loop") == i) {
      //disallow nested loops (simplification)
      if (loopinfo.inloop == 1) {
	printf("\nERROR: Nested loops are not allowed\n");
	return BAD_COMMAND;
      }
      //
      loopinfo.inloop = 1;
      loopinfo.startline = line;
      loopinfo.currentloop = 1;
      i = i + 4; //expect number of loops after 'loop' command
      loopinfo.numloops = strtol(i,&stopchar,10);
      //strtol found a character it couldn't convert
      if (*stopchar != ' ' && *stopchar != '\t' && *stopchar != '\n' && *stopchar != '\0') {
	printf("\nERROR: Invalid syntax following 'loop'. Expecting a number\n");
	return BAD_COMMAND;
      }
      // need at least 1 loop (0 might indicate bad number conversion)
      if (loopinfo.numloops <= 0) {
	printf("\nERROR: Expecting a number greater than 0 following 'loop'\n");
	return BAD_COMMAND;
      }
    }
    
    //'end' commands//
    else if (strstr(i,"end") == i) { 
      //expect something after 'end'
      i += 3; 
      i = skipspace(i);
      //'end loop' commands//
      if (strstr(i,"loop") == i) {
	//
	if (loopinfo.inloop == 0) {
	  printf("\nERROR: 'end loop' without 'loop'!\n");
	  return BAD_COMMAND;
	}
	//final loop not reached: 'rewind' str to start
	if (loopinfo.currentloop < loopinfo.numloops) {
	  rewind(fin);
	  for (int n = 1; n <= loopinfo.startline; n++) {
	    fgets(str,STRLEN,fin);
	  }
	  line = loopinfo.startline;
	  loopinfo.currentloop++;
	} else { //loop complete
	  loopinfo.inloop = 0;
	}
      }
      //'end function' commands
      else if (strstr(i,"function") == i) {
	endfcn = 1; //terminates the fcn parsing loop
      }
      //anything else after 'end' is invalid (for now)
      else {
	printf("\nERROR: Expecting 'loop' or 'function' after 'end'\n");
	return BAD_COMMAND;
      }
    }

    //EVERYTHING ELSE EXCEPT COMMENTS: PASS TO 'RUNCOMMAND'//
    //must either be a 'base function' or something invalid
    else if (strstr(i,"//") != i && *i != '\n' && *i != '\0') {
      
      printf("\nattempting to run command: %s\n",formattedstr);
      
      ret = runcommand(sw,displaygrid,formattedstr);
      //--------------HANDLE THE 'STATUS CHAIN'-----------------//
      /* anything apart from SUCCESS means that the current 'move'
       * should come to an end, and the status message should be 
       * passed all the way back up to the original caller to 
       * 'runcommand' (probably one of the game modules) */
	//for 'bad commands' also add error message to the 'stack'
      if (ret == BAD_COMMAND) {
	printf("\nERROR: Problem running command at line WHATEVER\n");
	return BAD_COMMAND;
      }
    }
  } while (endfcn == 0);
  
  //check below end of function (only comments allowed)
  while (fgets(str,STRLEN,fin) != NULL) {
    line++;
    i = skipspace(str);
    if (strstr(i,"//") != i && *i != '\n' && *i != '\0') {
      printf("\nERROR: Only comments may appear below end of function!\n");
      return BAD_COMMAND;
    }
  }

  //check for 'open' loops
  if (loopinfo.inloop == 1) {
    printf("\nERROR: 'loop' without 'end loop'!\n");
    return BAD_COMMAND;
  } 
  
  //all good
  return NO_ACTION;
  
} 

  
//-----add new variables (and associated info) to the variables list-----//
/* Note: If the 'name' input matches an existing var name, then
 * the value of that variable will simply be replaced with the 'val' input. 
 * i.e. later variable definitions over-write earlier variable definitions
 * (just like most other programming languages) */
varstruct *addvar(varstruct *varlist, char *name, char *val) {
  //handle the first var
  varstruct *v;
  if (varlist == NULL) { 
    varlist = (varstruct *)malloc(sizeof(varstruct));
    v = varlist;
  }
  else {
    //check first node for duplicate name
    v = varlist;
    if (strcmp(v->name,name) == 0) { //duplicate
      strcpy(v -> value, val); //replace
      return varlist;
    }
    //run through rest of list and check for duplicates
    while (v -> next != NULL) {
      v = v -> next;
      if (strcmp(v->name,name) == 0) { //duplicate
	strcpy(v -> value, val); //replace
	return varlist;
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
  return varlist;
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
