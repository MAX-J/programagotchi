#define WIDTH 110
#define HEIGHT 70

char screen[HEIGHT][WIDTH];
int currentrow = 0;
int currentcol = 0;
int score = 0;
char laststatus;

int gameturn(int height,int step);
int checkboard();
int readfile(char* filename);
