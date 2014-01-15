#include "./programagotchi.h"

int gameturn(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin); 

int playJump(char gamearray[HEIGHT][WIDTH], SDL_Simplewin sw){
	
	int score = 0;
	int state = 0;
	char str[STRLEN];
	
	SDL(gamearray, "Welcome to Jump", gamewin);
	  
	do{
		do{
			printf("Enter Command:");
			fgets(str,STRLEN,stdin);
	
			if(str[0] == '\n') {
				state = BAD_COMMAND;
			}
			else{
				state = runcommand(gamewin,gamearray,str);
				state = 0;
			}
		}while(state != BAD_COMMAND);
		
		if(state == QUIT_COMMAND){
			SDL(gamearray, "Exiting Game", gamewin);
			return LOSE;
		}
		
		else if(state == ON_EXIT){
			SDL(gamearray, "Level Completed. Congralutations", gamewin);
			return WIN;
		}
		
		else if(state == ON_HAZARD){
			SDL(gamearray, "Oh my god!you jump to a hole", gamewin);
			return LOSE;
		}

		//else if(state == ATE_CANDY){
			//SDL(gamearray, "You get a candy",gamewin);
 			//return 

		SDL(gamearray, "Type in next move", gamewin);
		//getchar();
		//score = gameturn(gamearray, score);
		
		SDL(gamearray, "", gamewin);
		
		Neill_SDL_Events(&gamewin);
		
	}while(state == NO_ACTION || !gamewin.finished);
	
	return LOSE;
}	

int gameturn(char gamearray[HEIGHT][WIDTH], SDL_Simplewin gamewin) {
 	int j = 0;
        int k = 0;
        int startflag = 0;

        char command;
       
	/*for(j =0;j < HEIGHT;j++)
        {
                for(k = 0;k< WIDTH;k++)
                {
                        if( gamearray[j][k] == '.')
                        {
                                laststatus = '.';
                               gamearray[j][k] = 'G';
                                startflag = 1;
                                currentrow = j;
                                currentcol = k;
                                break;
                        }
                        if( startflag)
                        {
                                break;
                        }
                }
        }*/
        for( j = 0;j < HEIGHT;j++)
        {
                        printf("%2d:%s\n",j+1,gamearray[j]);
        }
        while(score != 100 )
        {
                
                printf("Please input your option,'g' for take a turn,'e' for exit game\n");
                scanf("%c",&command);
                if(command == 'e' || command == 'E')
                {
                        break;
                }
                else if(command == 'g' || command == 'G')
                {
                        printf("Please input jump height and step\n");
                        printf("Positive number for downword/forwoad,Negtive number for upword/backward\n");
                        printf("Height = ");
                        scanf("%d",&height);
                        printf("Step = ");
                        scanf("%d",&time);
			k = gameturn(height,time);
			if(k == 0)
                        {
                                for( j = 0;j < HEIGHT;j++)
                                {
                                        printf("%2d:%s\n",j,gamearray[j]);
                                }
                                printf("Your score is %d\n",score);
                        }
			else if( k == -1) {
                        	getchar();
				break;
			}
			getchar();
                }
                else
                {
                        continue;
                }
                
        }
        printf("Game over!\n");
        printf("Your score is %d!\n",score);
        return 1;
}

/*int gameturn(int height,int step)
{

        
        int newrow = currentrow + height;
        int newcol = currentcol +  step;
        
        if( newrow < HEIGHT &&  newcol < WIDTH 
        && newrow >= 1 &&  newcol >= 1)
        {
                if(screen[newrow][newcol] == 'c' ||
                        screen[newrow][newcol] == 'C')
                {
                        score += 10;
                        screen[newrow][newcol] = 'G';
                        screen[currentrow][currentcol] = laststatus;
                        printf("You get a candy!\n");
                }
                else if(screen[newrow][newcol] == '1' ||
                        screen[newrow][newcol] == '#')
                {
                        printf("Ouch!you meet a barrier!\n");
                        printf("Please input a valid value!\n");
                        return 1;
                }
		else if(screen[newrow][newcol] == 'x' ||
			screen[newrow][newcol] == 'X')
		{
			printf("oh my god!you jump to a hole!\n");
			return -1;
		}
                else if(screen[newrow][newcol] == '.')
                {
                        screen[currentrow][currentcol] = laststatus;
                        laststatus = '.';
                        screen[newrow][newcol] = 'G';
                }
                else
                {
                        screen[currentrow][currentcol] = laststatus;
                        laststatus = screen[newrow][newcol];
                        screen[newrow][newcol] = 'G';
                }
                currentrow = newrow;
                currentcol = newcol;
        }
        else
        {
                printf("Oops!you are trying to jump out of the wall!\n");
                printf("Please input a valid value!\n");
        }
        return 0;
}*/
