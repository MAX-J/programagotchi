#include <stdio.h>
#include <memory.h>
#include <jump.h>

int main(int argc,char** argv)
{
       
        int i = 0;
        int j = 0;
        int k = 0;
        int startflag = 0;
        
        int height = 0;
        int time = 0;

        char command;
        if( argc  != 2)
        {
                printf("Invalid argument!\n");
                printf("Usage:game boardfile\n");
                return 0;
        }
	if( readfile(argv[1])) 
	{
		return 1;
	}       
	/*if(checkboard()  == -1)
	{
		return 1;
	}*/
        printf("LETS JUMP\n");
       
	for(j =0;j < HEIGHT;j++)
        {
                for(k = 0;k< WIDTH;k++)
                {
                        if( screen[j][k] == '.')
                        {
                                laststatus = '.';
                                screen[j][k] = 'G';
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
        }
        for( j = 0;j < HEIGHT;j++)
        {
                        printf("%2d:%s\n",j+1,screen[j]);
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
                                        printf("%2d:%s\n",j,screen[j]);
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


int readfile(char* filename)
{
	FILE* fp;
	fp = fopen(filename,"r");
	int j = 0;
	for( j = 0;j< HEIGHT;j++)
	{
		memset(screen[j],'\0',sizeof(screen[j]));
	}
	j = 0;
	if( fp != NULL)
	{
		while( fp != NULL)
		{
			if(j < HEIGHT)
			{
				fscanf(fp,"%s",screen[j]);	
				printf("%s\n",screen[j]);
				j++;	
			}
			else
			{
				break;
			}
		}	
	}
	else
	{
		printf("read file error \n");
		return 1;
	}
	fclose(fp);
	return 0;
}


int gameturn(int height,int step)
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
		else if(screen[newrow][newcol] == 's' ||
			screen[newrow][newcol] == 'S')
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
}


int checkboard()
{
	int i = 0;
	int j = 0;
	int s_countter = 0;
	for( i = 0;i < HEIGHT;i++)
	{
		if(screen[i][0] != '#' ||
			screen[i][WIDTH-2] != '#')
			{
				printf("not valid wall!\n");
				return -1;
			}
	}
	for(i = 0;i < WIDTH-1;i++)
	{
		if(screen[0][i] != '#' ||
			screen[HEIGHT-1][i] != '#')
			{
				printf("not valid wall!\n");
				return -1;
			}
	}
	for( i = 0;i<HEIGHT;i++)
	{
		for(j = 0;j<WIDTH;j++)
		{
			if(screen[i][j] == 'c'||
				screen[i][j] == 'C')
				{
					s_countter++;
				}
		}
	}
	if(s_countter < 10)
	{
		printf("not enough 'c'\n");
		return -1;
	}
	return 1;
}


