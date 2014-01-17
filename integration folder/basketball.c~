//Basketball

//Alex W

//Method

//1. Create gotchi board (110*70) edged by hash characters and add ball, gotchi and basket

//3. Start by translating it's position up and down ("reflecting" momtm off the top and bottom, perfect elasticity) using system clock to calculate
//position evolution over time at constant speed (no accel). Use floats for position calculation and round up/down as appropriate, to fit the pixel
//size of the board. Translate up and down first (y) and ignore horizontal (x). That should result in the ball bouncing up and down off the top and bottom at
//constant speed. Start with the ball moving downwards at an arbitrary speed. Keep the ability to add in this speed - this will end up being the y component
//input when calling the function.

//4. Now add an acceleration due to gravity downwards, emulating the equations of motion. Start with a y input of 0 - this should result in the ball
//bouncing up and down.

//5. Then add ability to input an x velocity component, and reflection of momentum from the sides of the board. Test with an x input, the ball should bounce
//up and down while moving from left to right.

//6. Now add modifiers to the velocity components - will need ability to input elasticity (e.g. set to 0 for jumping gotchi, don't want them to bounce.. Also
//make it possible to set to greater than 1 - gotchi's special bounce power?!) air resistance, ability to modify gravitational strength, and backspin for
//basketball. Also friction? Could make the gotchi skid when he jumps horizontally.

//7. Bouncing off corners? Need trig to modify diection

//Make main call a gravity function with the parameters above. Call the gravity function with a pointer to a game array declared in main. Do this to
//allow integration with the rest of the game's functionality.

//Include
    #include "./programagotchi.h"

//Defines
    #define INITX 6     		//Initial object position
    #define GRAV (-100)			//Value used for "g" can modify until game looks ok
    #define MILLISECONDDELAY 17		//Delay between frames in milliseconds approx (gives about 59fps)
    #define MSEC 1000			//Number of milliseconds per second
    #define ITERATIONS 1500	    	//Break loop, about 15 sec per throw
    #define FRAMES 118    		//Number of frames in 2 sec approx
    #define STRSIZE 50			//Max length of user input string containing throw angle and speed
    #define FLOOR1 68			//Level 1 row below basket
    #define FLOOR2 18			//Level 2 row below basket
    #define XFLOOREDGE2 66		//Position of the edge of the floor in level 2
    #define YFLOOREDGE2 19		//Position of the edge of the floor in level 2
    #define XWINPOS1 84			//Level 1 winning positions
    #define YWINPOS1 63			
    #define XWINPOS2 78			//Level 2 winning positions
    #define YWINPOS2 13
    #define MAXTURNS 10			//Number of throws allowed
    #define WINPERIOD 500		//Run for about 2.5sec after a winning shot
    #define ENDGAMEPAUSE 2000		//in ms
    
//Structures
    //Moving object
    struct obj
    {
      double vx, vy;		      	//x and y velocities
      double sx, sy;			//x and y positions
      long x, y;			//Location of the object in the gameboard array
    };
    typedef struct obj Obj;

//Functions

  //Main game function
  int playBasketball(char gamearray[HEIGHT][WIDTH], SDL_Simplewin sw);

  //Game function
  int Basketball(char gamearray[HEIGHT][WIDTH], double speed, double angle, SDL_Simplewin sw);

  //Calculate new position of object
  int TranslatePosition(Obj *Ball, char gamearray[HEIGHT][WIDTH], int Level, int *Iterations);
  
  //Set inital x and y speeds using input angle and throw speed
  void SetSpeeds(double speed, double angle, Obj *Ball);

  //Ball functions
  Obj *InitialiseBall(Obj *Ball);

int playBasketball(char gamearray[HEIGHT][WIDTH], SDL_Simplewin sw)	
{
    char userinput[STRSIZE];		      	//User input speed and angle (for Jon's parser)
    char temp[STRSIZE];			      	//Temp string to deal with whitespace in user's input
    double params;			      	//Temp variable for user's angle and speed input
    int CorrectInput = 0;		      	//Set to 1 if the user's input is ok
    char **end = NULL;			      	//For use with strtol
    int Score = 0;			      	//For renderer, game score
    char GameMessage[MAX];  			//First string for renderer
    int NumberOfTries = 0;		      	//Number of attempted throws
    int Level = 0;			      	//Game level
    int i = 0, j = 0;			      	//Counters
    int Delay = 0;				//SDL render delay
    int Winner = 0;				//For return from parse function
    int GameScores[MAXTURNS] = {		//Game scores
      100, 50, 25, 20, 15, 10, 5, 4, 3, 1
    };
    
    //First screen output
    strcpy(GameMessage, "Play Basketball!");
    SDL(gamearray, GameMessage, Score, Delay, sw);
    
    //Which level is it?
    if(gamearray[YWINPOS1][XWINPOS1] == NET)
    {
      Level = 1;
      printf("\n\nLevel 1\n\n");
    }
    else if(gamearray[YWINPOS2][XWINPOS2] == NET)
    {
      Level = 2;
      printf("\n\nLevel 2\n\n");
    }
    else
    {
      printf("Where's the basket?!\n");
      //exit? Return an error at least..
    }
    
    do
    {
      //Print these to terminal
      printf("Use the Throw function to throw the basketball\n\n"); 
      printf("Type \"throw\" , then the speed and throw angle separated by spaces, then press enter.\n");
      printf("Make sure the speed is more than 0 and less than 250, and the angle \n");
      printf("is between 10 and 80 degrees. You can use decimals if you want to:\n\n");
      printf("Enter Command:");
      
      //The loop scans in the inputs one by one and throws errors if it's not as expected
      //If everything's ok, the output string is sent to Jon's parser.
      do
      {  
	for(i = 0; i < 3; i++)
	{
	  scanf("%s", temp);
	  if(i == 0)
	  {
	    if(strcmp(temp, "quit") == 0)
	    {
	      printf("reached quit\n");
	      printf("LOSE %d\n", LOSE);
	      return LOSE;
	    }
	    else if(strcmp(temp, "throw") != 0)
	    {
	      printf("\n\nWrong function!\n\nTry again\n\nEnter Command:");
	      i = 3;
	      //Can't get this to work?
	      fflush(stdin);
	    }
	    else
	    {
	      strcpy(userinput, temp);
	    }
	  }
	  else if(i == 1)
	  {
	    params = strtod(temp, end);
	    if((params <= 0) || (params >= 250))
	    {
	      printf("\n\nEnter a speed more than 0 and less than 250!\n\nTry again\n\nEnter Command:");
	      i = 3;
	      //Can't get this to work?
	      fflush(stdin);
	    } 
	    else
	    {
	      strcat(userinput, " ");
	      strcat(userinput, temp);
	    }
	  }
	  else
	  {
	    params = strtod(temp, end);
	    if((params < 10) || (params > 80))
	    {
	      printf("\n\nEnter an angle between 10 and 80 degrees!\n\nTry again\n\nEnter Command:");
	      i = 3;
	      //Can't get this to work?
	      fflush(stdin);
	    } 
	    else
	    {
	      strcat(userinput, " ");
	      strcat(userinput, temp);
	      CorrectInput = 1;
	    }
	  }
	}
      }while(CorrectInput == 0);
      
      //Jon's parser
      Winner = runcommand(sw, gamearray, userinput);
      
      if(Winner == HIT_BASKET)
      {
	Score = GameScores[NumberOfTries];
	snprintf(GameMessage, MAX, "Gotchi power!! :-) Score %d", Score);
	Delay = ENDGAMEPAUSE;
	SDL(gamearray, GameMessage, Score, Delay, sw);
	return Score;
      }
      
      //Reset gamearray
      //Use .txt file if time
      for(i = 0; i < HEIGHT; i++)
      {
	  for(j = 0; j < WIDTH - 2; j++)
	  {
	    gamearray[i][j] = FREE_SPACE;
	  }
      }
      
      //Add edges
      for(i = 0; i < HEIGHT; i++)
      {
	  if(i == 0 || i == HEIGHT - 1)
	  {
	      for(j = 0; j < WIDTH - 2; j++)
	      {
		  gamearray[i][j] = BORDER;
	      }
	  }
	  else
	  {
	      gamearray[i][0] = BORDER;
	      gamearray[i][WIDTH - 3] = BORDER;
	  }
      }
      
      //Add Gotchi
      gamearray[HEIGHT - 2][INITX - 1] = GOTCHI;
      
      //Replace ball
      gamearray[HEIGHT - 3][INITX] = BALL;
      
      //Add Basket and surrounding obstacle chars
      if(Level == 1)
      {
	gamearray[63][83] = BARRIER;
	gamearray[YWINPOS1][XWINPOS1] = NET;
	gamearray[63][85] = BARRIER;
	gamearray[64][83] = BARRIER;
	gamearray[64][85] = BARRIER;
	for(i = 61; i < 69; i++)
	{
	  gamearray[i][86] = BARRIER;
	}
      }
      else if(Level == 2)
      {
	gamearray[13][77] = BARRIER;
	gamearray[YWINPOS2][XWINPOS2] = NET;
	gamearray[13][79] = BARRIER;
	gamearray[14][77] = BARRIER;
	gamearray[14][79] = BARRIER;
	for(i = 11; i < 19; i++)
	{
	  gamearray[i][80] = BARRIER;
	}
	for(i = 65; i < 108; i++)
	{	
	  gamearray[19][i] = BORDER;
	}
      }
      
      //reprint the board
      NumberOfTries++;
      snprintf(GameMessage, MAX, "Try again - throw %d", NumberOfTries + 1);
      SDL(gamearray, GameMessage, Score, Delay, sw);
      
    }while(NumberOfTries < MAXTURNS);
    
    //Game over
    strcpy(GameMessage, "Game over! Gotchi is sad :-(");
    Delay = ENDGAMEPAUSE;
    SDL(gamearray, GameMessage, Score, Delay, sw);
    
    return LOSE;
    
}

int Basketball(char gamearray[HEIGHT][WIDTH], double speed, double angle, SDL_Simplewin sw)
{
    int MaxIter = ITERATIONS;			//No of frames to calculate
    Obj *Ball = NULL;			      	//Ball object
    int Level = 0;				//Use the W char to determine the level
    int Winner = 0;				//Set to 1 if the game is won
    char Empty[MAX] = {"\0"};			//Empty string to pass to SDL renderer
    char GameMessage[MAX];			//String for renderer
    int NoScore = 0;				//Zero score to pass to render while ball is in motion
    int Delay = 0;				//Delay for renderer
    
    //strcpy(Empty, "");
    strcpy(GameMessage, "Play Basketball!");
    
    //Which level is it?
    if(gamearray[YWINPOS1][XWINPOS1] == NET)
    {
      Level = 1;
    }
    else if(gamearray[YWINPOS2][XWINPOS2] == NET)
    {
      Level = 2;
    }
    else
    {
      printf("Where's the basket?!\n");
      //exit? Return an error at least..
    }    
    
     //Initialise ball
    Ball = InitialiseBall(Ball);
    
    //Set ball x & y speeds using angle and throw speed
    SetSpeeds(speed, angle, Ball);
    
    //Set object initial positions
    Ball->sx = (double)Ball->x;
    Ball->sy = (double)Ball->y;
    
    //Run game
    do
    {
      //Pause
      SDL_Delay(MILLISECONDDELAY);
      
      //Update gamearray
      Winner = TranslatePosition(Ball, gamearray, Level, &MaxIter);
      
      //Render gamearray
      SDL(gamearray, Empty, NoScore, Delay, sw);		
 
      MaxIter--;
    }while(MaxIter != 0 && Winner == 0);
    
    return Winner;
}

int TranslatePosition(Obj *Ball, char gamearray[HEIGHT][WIDTH], int Level, int *Iterations)
{
  double vxOld = Ball->vx, vyOld = Ball->vy;		//Store previous vx and vy	
  double xdist = 0, ydist = 0;				//x and y distances travelled in interval MILLISECONDDELAY, initialise to 0
  int WinnerTemp = 0;					//Game won if set to 1, temp as need to keep the game running while ball comes to rest
  int Winner = LOSE;					//Set to WIN when ball comes to rest after a winning throw
  static long rolling = 0;				//Use this to store the y pos over a number of iterations, if it's the same after e.g. 120 iterations, the ball is rolling
  static int isrolling = 0;				//Set this to 1 when the ball is rolling
  const double drag = -0.1;				//Drag force (modified from calculated value below to make the game look (slightly!) better
  double bounce = 0.85;					//Coeff. of restitution is between 0.82 and 0.88 for a FIBA basketball
  const double RollResist = 6;				//Rolling resistance (force is the coefficient of rolling resistance times the normal force, 
							//6.5N @ basketball weight of .65kg & g = 10ms-2. With estimated C = 0.1 (same as car tyres on concrete)
							// --> 6.5N*0.1 = 0.65N, try this first
  const double RollingSpeed = 9.5;			//If y speed is less than this and y pos is the same as the floor position over 120 iterations, ball is rolling (cludge, source of bugs :-( )
  int i = 0;						//Counter
  static long ypos = 0;					//If this is the same after 120 iterations, the ball is in the winning position
  double ydistIntraFrame = 0;				//How far has ball move in y after a given amount of movement in x inbetween frame prints? 
							//Used to calculate bounce point for floor in level2
  double xdistIntraFrame = 0;				//Same again for x
  int yEdge = 0;					//Test whether ball hits edge of level 2 floor
  int Floor = 0;					//Test whether ball hits level 2 floor
  
  //If ball hits the NET char with downward velocity, then the game has been won
  //Ball bounces off all other obstacle and edge chars
  //Assumes the ball is a whole gameboard cell in size - change the bounce point to give 
  //the impression of a smaller ball..or make the basket three cells wide?
  
  //Reset statics if it's a new game
  if(*Iterations == ITERATIONS)
  {
    rolling = 0;
    isrolling = 0;
    ypos = 0;
  }
  
  //Is the ball in the basket, and y velocity -ve? Winner if so
  if(Level == 1 && gamearray[YWINPOS1][XWINPOS1] == BALL && Ball->vy < 0)
  {
    WinnerTemp = 1;
    //*Iterations = WINPERIOD;
    bounce = 0.45;		//Make ball stop sooner
  }
  else if(Level == 2 && gamearray[YWINPOS2][XWINPOS2] == BALL && Ball->vy < 0)
  {
    WinnerTemp = 1;
    //*Iterations = WINPERIOD;
    bounce = 0.45;
  }
  
  //Replace previous object char with a blank char
  gamearray[Ball->y][Ball->x] = FREE_SPACE;
  
  //Calculate new vx and vy - apply aero drag and gravity with v = v0 + at
  //Assumes drag scales linearly with speed, i.e. no turbulence
  //FIBA max ball weight is .65kg. Assume laminar flow, then drag coeff. ~ 0.1. Gives
  //a drag force of F = -C*v = -0.1v. Then a = F/m = -0.1v/.65 = -0.154v.
  if(WinnerTemp == 0)
  {
    //Drag applies to both vx and vy 
    Ball->vx = Ball->vx + Ball->vx*drag*((double)MILLISECONDDELAY)/(double)MSEC;
    Ball->vy = Ball->vy + Ball->vy*drag*((double)MILLISECONDDELAY)/(double)MSEC;
    //Gravity is a constant force downwards
    Ball->vy = Ball->vy + (double)GRAV*((double)MILLISECONDDELAY)/(double)MSEC;
  
    //Calculate distance travelled with x = ((v + v0)/2)*t
    //Ball won't appear next to the wall with this method?
    xdist = ((double)MILLISECONDDELAY/(double)MSEC)*(abs(vxOld) + abs(Ball->vx))/2;
    ydist = ((double)MILLISECONDDELAY/(double)MSEC)*(abs(vyOld) + abs(Ball->vy))/2;
  }
  
  //Need to bounce off if ball hits a wall or the edge of the extra floor in level 2
  if(WinnerTemp == 0)
  {
    
    /*
    //If the ball traverses the level 2 floor edge position, what's the intraframe distance to the edge?
    if(Ball->vx > 0 && Ball->sx < (double)(XFLOOREDGE2) && Ball->sx + xdist > (double)(XFLOOREDGE2) && Level == 2)
    {
      ydistIntraFrame = ydist*(((Ball->sx + xdist) - (double)(XFLOOREDGE2))/xdist);
    }
    
    //If yposition + intraframe distance is then equal to the edge position in y plane, it's hit the edge  
    if(ydistIntraFrame != 0 && Ball->vy > 0)
    {
      if(round(Ball->sy - ydistIntraFrame) == YFLOOREDGE2)
      {
	yEdge = 1;
      }
    }
    //Motion down
    else if(ydistIntraFrame != 0 && Ball->vy < 0)
    {
      if(round(Ball->sy + ydistIntraFrame) == YFLOOREDGE2)
      {
	yEdge = 1;
      }
    }
    */
    
    //Rhs wall
    if((Ball->sx + xdist > (double)(WIDTH - 4) && Ball->vx > 0))
    {
      Ball->sx = (double)(WIDTH - 4) - (xdist - ((double)(WIDTH - 4) -Ball->sx)); 
      Ball->vx = Ball->vx*(-bounce);
    }
    //Lhs wall
    else if((Ball->sx - xdist < (double)1 && Ball->vx < 0))
    {
      Ball->sx = xdist - Ball->sx + 2;
      Ball->vx = Ball->vx*(-bounce);
    }
    //Motion right
    else if(Ball->vx > 0)	//Unlikely but could be 0, >0 and <0 will miss this case
    {
      Ball->sx = Ball->sx + xdist;
    }
    //Motion left
    else
    {
      Ball->sx = Ball->sx - xdist;
    }
  }
  
  //Need to bounce off if ball hits the floor or ceiling - reversed due to array structure
  if(WinnerTemp == 0)
  {
    //If the ball traverses the floor in level 2, what's the intraframe distance to the ceiling?
    if(Ball->vy > 0 && Ball->sy > (double)(FLOOR2 + 1) && Ball->sy - ydist < (double)(FLOOR2 + 1) && Level == 2)
    {
      xdistIntraFrame = xdist*(((double)(FLOOR2 + 1) - (Ball->sy - ydist))/ydist);
    }
    //Or to the floor?
    else if(Ball->vy < 0 && Ball->sy < (double)(FLOOR2) && Ball->sy + ydist > (double)(FLOOR2) && Level == 2)
    {
      xdistIntraFrame = xdist*(((Ball->sy + ydist) - (double)(FLOOR2))/ydist);
    }
    
    //If xposition + intraframe distance is beyond the start of the floor edge, it's hit the level 2 ceiling or floor
    if(xdistIntraFrame != 0 && Ball->vx > 0)
    {
      if(round(Ball->sx + xdistIntraFrame) >= XFLOOREDGE2)
      {
	Floor = 1;
      }
    }
    else if(xdistIntraFrame != 0 && Ball->vx < 0)
    {  
      if(round(Ball->sx - xdistIntraFrame) >= XFLOOREDGE2)
      {
	Floor = 1;
      }
    }
    
    //Hits extra floor in level 2 if true - buggy
    if(Ball->vy < 0 && Ball->sy < (double)(FLOOR2) && Ball->sy + ydist > (double)(FLOOR2) && Floor == 1 && Level == 2)
    {
      Ball->sy = (double)(FLOOR2) - (ydist - ((double)(FLOOR2) - Ball->sy));
      Ball->vy = Ball->vy*(-bounce);
    }
    //Floor
    else if((Ball->sy + ydist > (double)(HEIGHT - 2) && Ball->vy < 0))
    {
      Ball->sy = (double)(HEIGHT - 2) - (ydist - ((double)(HEIGHT - 2) -Ball->sy)); 
      Ball->vy = Ball->vy*(-bounce);
    }
    //Hits extra ceiling in level 2 if true
    else if(Ball->vy > 0 && Ball->sy > (double)(FLOOR2 + 1) && Ball->sy - ydist < (double)(FLOOR2 + 1) && Floor == 1 && Level == 2)
    {
      Ball->sy = (double)(FLOOR2 + 1) + (ydist - (Ball->sy - (double)(FLOOR2 + 1)));
      Ball->vy = Ball->vy*(-bounce);
    }
    //Ceiling
    else if((Ball->sy - ydist < (double)1 && Ball->vy > 0))
    {
      Ball->sy = ydist - Ball->sy + 2;
      Ball->vy = Ball->vy*(-bounce);
    }
    //Motion up
    else if(Ball->vy > 0)	//Same problem with 0 not dealt with
    {
      //inverted because of array structure
      Ball->sy = Ball->sy - ydist;	
    }
    //Motion down
    else
    {
      Ball->sy = Ball->sy + ydist;
    }  
  }
  
  //Round off sx and sy to find position of object on the screen
  //round rounds to 1 from 0.5, to 0 from 0.4(9). Means ball in sx 
  //and sy positions are 0.5 at boundaries between integer values of x and y
  if(WinnerTemp == 1)
  {
    //Move to one below basket and zero speeds if last frame was a winner
    Ball->sy = Ball->sy + 1;
    Ball->y = Ball->y + 1;
    Ball->vx = 0;
    Ball->vy = 0;
  }
  else
  {
    Ball->x = round(Ball->sx);
    Ball->y = round(Ball->sy);
  }
  
  //If the ball has the same y pos over a couple of seconds 
  //(~120 frames) and y speed is low, treat it as rolling and 
  //apply roll resistance 
  if(*Iterations % FRAMES == 0)
  {
    rolling = Ball->y;
  }
  //Cludges to force this to work :-S Ball is approximately rolling if it's on a "floor" surface
  //for a couple of seconds, and if the y speed is low 
  if((*Iterations % FRAMES == FRAMES - 1) && (rolling == Ball->y) && ((rolling == FLOOR1) || (rolling == FLOOR2)) && (Ball->vy < RollingSpeed))
  {
    isrolling = 1;
  }
  if(isrolling == 1)
  {
    Ball->vy = 0.0001;	//minimise remaining y speed - stop ball from appearing to bounce after starting to roll
   
    if(Ball->vx > 0)
    {
      Ball->vx = Ball->vx - RollResist*((double)MILLISECONDDELAY)/(double)MSEC;
    }
    else
    {
      Ball->vx = Ball->vx + RollResist*((double)MILLISECONDDELAY)/(double)MSEC;
    }
  }
  
  //Want ball to roll of the edge and fall in game 2
  /*
  if(isrolling == 1 && Ball->vx < 0 && Ball->x < XFLOOREDGE2)
  {
    rolling = 0;
    isrolling = 0;
    ypos = 0;
  }
  */
  
  //Replace Gotchi, obstacles, and basket
  //Hardcoded, use .txt file if time
  gamearray[HEIGHT - 2][INITX - 1] = GOTCHI;
  if(Level == 1)
  {
    gamearray[63][83] = BARRIER;
    gamearray[YWINPOS1][XWINPOS1] = NET;
    gamearray[63][85] = BARRIER;
    gamearray[64][83] = BARRIER;
    gamearray[64][85] = BARRIER;
    for(i = 61; i < 69; i++)
    {
      gamearray[i][86] = BARRIER;
    }
  }
  else if(Level == 2)
  {
    gamearray[13][77] = BARRIER;
    gamearray[YWINPOS2][XWINPOS2] = NET;
    gamearray[13][79] = BARRIER;
    gamearray[14][77] = BARRIER;
    gamearray[14][79] = BARRIER;
    for(i = 11; i < 19; i++)
    {
      gamearray[i][80] = BARRIER;
    }
    for(i = 65; i < 108; i++)
    {	
      gamearray[19][i] = BORDER;
    }
  }
  
  //Print object in new position
  gamearray[Ball->y][Ball->x] = BALL;
  
  //Set winner = 1 if ball comes to a halt underneath basket, leave a couple of seconds to allow it to bounce
  //Massive bug here! What if ball stops here after rolling?
  if(*Iterations % FRAMES == 0)
  {
    ypos = Ball->y;
  }
  if((*Iterations % FRAMES == FRAMES - 1) && (ypos == Ball->y) && (Ball->vx < 0.1) && (Ball->vy < 5) && ((gamearray[HEIGHT - 2][XWINPOS1] == BALL) || (gamearray[HEIGHT - 2][XWINPOS2] == BALL)))
  {
    Winner = WIN;
  }
  
  return Winner; 
}

//Set object initial x and y speeds using user input angle and throw speed
void SetSpeeds(double speed, double angle, Obj *Ball)
{
  double angleradians;
  
  //Need to convert from degrees to radians
  angleradians = angle * (double)M_PI / (double)180.0;
  Ball->vx = speed*sin(angleradians);
  Ball->vy = speed*cos(angleradians);
}

//Initialise ball
Obj *InitialiseBall(Obj *Ball)
{
  Obj *NewBall = NULL;
  NewBall = (Obj*)calloc(1, sizeof(Obj));
  if(NewBall == NULL)
  {
      printf("Cannot allocate NewBall\n");
      exit(1);
  }
  NewBall->vx = 0;				//Ball velocities
  NewBall->vy = 0;		      	
  NewBall->sx = 0;				//Ball exact positions (doubles)
  NewBall->sy = 0;				
  NewBall->x = INITX;				//Ball discrete gameboard positions
  NewBall->y = HEIGHT - 3;			//Initially above and to the right of the Gotchi
  return NewBall;
  
}
