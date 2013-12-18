#define WIDTH 110
#define HEIGHT 70
int jump(char gamearray[HEIGHT][WIDTH]);
//char screen[HEIGHT][WIDTH];
//int currentrow = 0;
//int currentcol = 0;
//int score = 0;
char laststatus;

int gameturn(int height,int step, char screen[HEIGHT][WIDTH]);
int checkboard(char screen[HEIGHT][WIDTH]);
int readfile(FILE *fp, char screen[HEIGHT][WIDTH]);
