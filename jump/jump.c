#include <stdio.h>
#include <memory.h>
#define WIDTH 110
#define HEIGHT 70

char screen[HEIGHT][WIDTH];
int currentrow = 0;
int currentcol = 0;
int score = 0;
char laststatus;

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
			return 0;
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

int main(int argc,char** argv)
{
	FILE* fp;
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
	printf("LETS JUMP\n");
	fp = fopen(argv[1],"r");
	for( j = 0;j< HEIGHT;j++)
	{
		memset(screen[j],'\0',sizeof(screen[j]));
	}
	if( fp != NULL)
	{
		while( fp != NULL)
		{
			i++;
			if(i < HEIGHT)
			{
				fscanf(fp,"%s",screen[i]);		
			}
			else
			{
				break;
			}
		}
		
	}
	else
	{
		printf("Read file error.\n");
		return 0;
	}
	fclose(fp);
	for(j =1;j < i;j++)
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
	for( j = 1;j < i;j++)
	{
			printf("%2d:%s\n",j,screen[j]);
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
			if( gameturn(height,time) ==0)
			{
				for( j = 1;j < i;j++)
				{
					printf("%2d:%s\n",j,screen[j]);
				}
				printf("Your score is %d\n",score);
			}
			getchar();
		}
		/*else if(command == 's' || command == 'S')
		{
			printf("Dangerous, play again!!!\n");
			break;
		}*/
		else
		{
			continue;
		}
		
	}
	printf("Game over!\n");
	printf("Your score is %d!\n",score);
	return 1;
}



	

