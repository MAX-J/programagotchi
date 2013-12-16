#define WIDTH 110
#define HEIGHT 70
int jump(char gamearray[HEIGHT][WIDTH]);
char screen[HEIGHT][WIDTH];
//int currentrow = 0;
//int currentcol = 0;
//int score = 0;
char laststatus;

int gameturn(int height,int step);
int checkboard();
int readfile(FILE *fp);
