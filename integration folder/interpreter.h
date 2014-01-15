
#define PASS 1
#define FAIL 0

enum direction {left,right,above,below};
typedef enum direction Direction;

int parsefcn(SDL_Simplewin sw, char displaygrid[HEIGHT][WIDTH], char *filestr, char *commandstr);
int moveobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, int rowshift, int colshift);
int addobject(char displaygrid[HEIGHT][WIDTH], char selectedobj, Direction dir);
int eatcandy(char displaygrid[HEIGHT][WIDTH]);
int objectongrid(char displaygrid[HEIGHT][WIDTH], char selectedobj);
int obstacle_adjacent(char displaygrid[HEIGHT][WIDTH],Direction dir,char selectedobj);
int hazard_adjacent(char displaygrid[HEIGHT][WIDTH],Direction dir,char selectedobj);
int candy_adjacent(char displaygrid[HEIGHT][WIDTH],char selectedobj);

