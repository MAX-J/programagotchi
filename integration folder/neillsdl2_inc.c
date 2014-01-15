#include "neillsdl2_inc.h"

// Set up a simple (WIDTH, HEIGHT) window.
// Attempt to hide the renderer etc. from user.
void Neill_SDL_Init(SDL_Simplewin *sw, char *WindowTitle)
{

   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   } 

   sw->finished = 0;
   
   sw->win= SDL_CreateWindow("Challenge Arena", 1000, 0, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
   if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
   if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   // Set screen to black
   Neill_SDL_SetDrawColour(sw, 0, 0, 0);
   SDL_RenderClear(sw->renderer);
   SDL_RenderPresent(sw->renderer);
   SDL_SetWindowTitle(sw->win, WindowTitle);
}

// Attempt to hide the renderer etc. from user.
void Mini_SDL_Init(SDL_Simplewin *sw, char *WindowTitle)
{
   

   if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   } 

   sw->finished = 0;
   
   sw->win= SDL_CreateWindow("Challenge Arena", 400, 50, MINIWIDTH, MINIHEIGHT, SDL_WINDOW_BORDERLESS);
   if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
   if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   // Set screen to black
   Neill_SDL_SetDrawColour(sw, 0, 0, 0);
   SDL_RenderClear(sw->renderer);
   SDL_RenderPresent(sw->renderer);
   SDL_SetWindowTitle(sw->win, WindowTitle);
}

// Gobble all events & ignore most
void Neill_SDL_Events(SDL_Simplewin *sw)
{ 
  SDL_Event event;
   while(SDL_PollEvent(&event)) 
   {      
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
       case SDL_KEYDOWN:
	    SDL_DestroyWindow(sw->win);
	    sw->finished = 1;
       }
    }
}


// Trivial wrapper to avoid complexities of renderer & alpha channels
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b)
{
   SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

// Filled Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{

   for (double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
   }

}

// Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r)
{

   double dx, dy;
   dx = floor(sqrt((2.0 * r ) ));
   SDL_RenderDrawLine(rend, cx-dx, cy+r, cx+dx, cy+r);
   SDL_RenderDrawLine(rend, cx-dx, cy-r, cx+dx, cy-r);
   for (dy = 1; dy <= r; dy += 1.0) {
        dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawPoint(rend, cx+dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx+dx, cy-r+dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy-r+dy);
   }

}

/* Based on :
 sdl_picofont
   http://nurd.se/~noname/sdl_picofont
   File authors:      Fredrik Hultin
   License: GPLv2
*/
void Neill_SDL_DrawText(SDL_Simplewin *sw, const char* text, int ox, int oy)
{
   unsigned int i, x, y, col, row, len;
   unsigned char *fnt, chr;

   fnt = Neill_SDL_GetFont();
   col = row = 0;
   len = strlen(text);
   for(i = 0; i < len; i++){
      switch(text[i]){
         case '\n':
            row++;
            col = 0;
            chr = 0;
            break;

         case '\r':
            chr = 0;
            break;

         case '\t':
            chr = 0;
            col += 4 - col % 4;
            break;
   
         default:
            col++;
            chr = text[i];
            break;
      }

      if(chr != 0){
         for(y = 0; y < FNT_FONTHEIGHT; y++){
            for(x = 0; x < FNT_FONTWIDTH; x++){
               if(fnt[text[i] * FNT_FONTHEIGHT + y] >> (7 - x) & 1){
                        SDL_RenderDrawPoint(sw->renderer, ((col - 1) * FNT_FONTWIDTH) + x + ox, (y + row * FNT_FONTHEIGHT) + oy);
               }
            }
         }
      }
   }
   SDL_RenderPresent(sw->renderer);

}

// Hide the details of font_pearl_8x8 away here
unsigned char* Neill_SDL_GetFont()
{
   static const unsigned char spf_font[FONTCHARS*FONTBYTESCHAR] = {
   /* 0 0x00      */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   /* 1 0x01 '^A' */ 0x7e, 0x81, 0xa5, 0x81, 0xbd, 0x99, 0x81, 0x7e,
   /* 2 0x02 '^B' */ 0x7e, 0xff, 0xdb, 0xff, 0xc3, 0xe7, 0xff, 0x7e, 
   /* 3 0x03 '^C' */ 0x6c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38, 0x10, 0x00, 
   /* 4 0x04 '^D' */ 0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00, 
   /* 5 0x05 '^E' */ 0x38, 0x7c, 0x38, 0xfe, 0xfe, 0xd6, 0x10, 0x38, 
   /* 6 0x06 '^F' */ 0x10, 0x38, 0x7c, 0xfe, 0xfe, 0x7c, 0x10, 0x38, 
   /* 7 0x07 '^G' */ 0x00, 0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00, 0x00, 
   /* 8 0x08 '^H' */ 0xff, 0xff, 0xe7, 0xc3, 0xc3, 0xe7, 0xff, 0xff, 
   /* 9 0x09 '^I' */ 0x00, 0x3c, 0x66, 0x42, 0x42, 0x66, 0x3c, 0x00, 
   /* 10 0x0a '^J' */ 0xff, 0xc3, 0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff, 
   /* 11 0x0b '^K' */ 0x0f, 0x07, 0x0f, 0x7d, 0xcc, 0xcc, 0xcc, 0x78, 
   /* 12 0x0c '^L' */ 0x3c, 0x66, 0x66, 0x66, 0x3c, 0x18, 0x7e, 0x18, 
   /* 13 0x0d '^M' */ 0x3f, 0x33, 0x3f, 0x30, 0x30, 0x70, 0xf0, 0xe0, 
   /* 14 0x0e '^N' */ 0x7f, 0x63, 0x7f, 0x63, 0x63, 0x67, 0xe6, 0xc0, 
   /* 15 0x0f '^O' */ 0x18, 0xdb, 0x3c, 0xe7, 0xe7, 0x3c, 0xdb, 0x18, 
   /* 16 0x10 '^P' */ 0x80, 0xe0, 0xf8, 0xfe, 0xf8, 0xe0, 0x80, 0x00, 
   /* 17 0x11 '^Q' */ 0x02, 0x0e, 0x3e, 0xfe, 0x3e, 0x0e, 0x02, 0x00, 
   /* 18 0x12 '^R' */ 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x7e, 0x3c, 0x18, 
   /* 19 0x13 '^S' */ 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00, 
   /* 20 0x14 '^T' */ 0x7f, 0xdb, 0xdb, 0x7b, 0x1b, 0x1b, 0x1b, 0x00, 
   /* 21 0x15 '^U' */ 0x3e, 0x61, 0x3c, 0x66, 0x66, 0x3c, 0x86, 0x7c, 
   /* 22 0x16 '^V' */ 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x7e, 0x00, 
   /* 23 0x17 '^W' */ 0x18, 0x3c, 0x7e, 0x18, 0x7e, 0x3c, 0x18, 0xff, 
   /* 24 0x18 '^X' */ 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 25 0x19 '^Y' */ 0x18, 0x18, 0x18, 0x18, 0x7e, 0x3c, 0x18, 0x00, 
   /* 26 0x1a '^Z' */ 0x00, 0x18, 0x0c, 0xfe, 0x0c, 0x18, 0x00, 0x00, 
   /* 27 0x1b '^[' */ 0x00, 0x30, 0x60, 0xfe, 0x60, 0x30, 0x00, 0x00, 
   /* 28 0x1c '^\' */ 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xfe, 0x00, 0x00, 
   /* 29 0x1d '^]' */ 0x00, 0x24, 0x66, 0xff, 0x66, 0x24, 0x00, 0x00, 
   /* 30 0x1e '^^' */ 0x00, 0x18, 0x3c, 0x7e, 0xff, 0xff, 0x00, 0x00, 
   /* 31 0x1f '^_' */ 0x00, 0xff, 0xff, 0x7e, 0x3c, 0x18, 0x00, 0x00, 
   /* 32 0x20 ' ' */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   /* 33 0x21 '!' */ 0x18, 0x3c, 0x3c, 0x3c, 0x18, 0x00, 0x18, 0x00, 
   /* 34 0x22 '"' */ 0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   /* 35 0x23 '#' */ 0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00, 
   /* 36 0x24 '$' */ 0x18, 0x3e, 0x60, 0x3c, 0x06, 0x7c, 0x18, 0x00, 
   /* 37 0x25 '%' */ 0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00, 
   /* 38 0x26 '&' */ 0x38, 0x6c, 0x68, 0x76, 0xdc, 0xcc, 0x76, 0x00, 
   /* 39 0x27 ''' */ 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
   /* 40 0x28 '(' */ 0x0c, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x00, 
   /* 41 0x29 ')' */ 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x18, 0x30, 0x00, 
   /* 42 0x2a '*' */ 0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00, 
   /* 43 0x2b '+' */ 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00, 
   /* 44 0x2c ',' */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 
   /* 45 0x2d '-' */ 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 
   /* 46 0x2e '.' */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 
   /* 47 0x2f '/' */ 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x00, 
   /* 48 0x30 '0' */ 0x7c, 0xc6, 0xde, 0xfe, 0xf6, 0xc6, 0x7c, 0x00, 
   /* 49 0x31 '1' */ 0x18, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 50 0x32 '2' */ 0x7c, 0xc6, 0x0c, 0x18, 0x30, 0x60, 0xfe, 0x00, 
   /* 51 0x33 '3' */ 0x7c, 0xc6, 0x06, 0x1c, 0x06, 0xc6, 0x7c, 0x00, 
   /* 52 0x34 '4' */ 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x0c, 0x00, 
   /* 53 0x35 '5' */ 0xfe, 0xc0, 0xfc, 0x06, 0x06, 0xc6, 0x7c, 0x00, 
   /* 54 0x36 '6' */ 0x38, 0x60, 0xc0, 0xfc, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 55 0x37 '7' */ 0xfe, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x60, 0x00, 
   /* 56 0x38 '8' */ 0x7c, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 57 0x39 '9' */ 0x7c, 0xc6, 0xc6, 0x7e, 0x06, 0x0c, 0x38, 0x00, 
   /* 58 0x3a ':' */ 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 
   /* 59 0x3b ';' */ 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30, 
   /* 60 0x3c '<' */ 0x0c, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0c, 0x00, 
   /* 61 0x3d '=' */ 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 
   /* 62 0x3e '>' */ 0x30, 0x18, 0x0c, 0x06, 0x0c, 0x18, 0x30, 0x00, 
   /* 63 0x3f '?' */ 0x3c, 0x66, 0x06, 0x0c, 0x18, 0x00, 0x18, 0x00, 
   /* 64 0x40 '@' */ 0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x7c, 0x00, 
   /* 65 0x41 'A' */ 0x10, 0x38, 0x6c, 0xc6, 0xfe, 0xc6, 0xc6, 0x00, 
   /* 66 0x42 'B' */ 0xfc, 0xc6, 0xc6, 0xfc, 0xc6, 0xc6, 0xfc, 0x00, 
   /* 67 0x43 'C' */ 0x7c, 0xc6, 0xc0, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 
   /* 68 0x44 'D' */ 0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 
   /* 69 0x45 'E' */ 0xfe, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0xfe, 0x00, 
   /* 70 0x46 'F' */ 0xfe, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0xc0, 0x00, 
   /* 71 0x47 'G' */ 0x7c, 0xc6, 0xc0, 0xce, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 72 0x48 'H' */ 0xc6, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 
   /* 73 0x49 'I' */ 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e, 0x00, 
   /* 74 0x4a 'J' */ 0x06, 0x06, 0x06, 0x06, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 75 0x4b 'K' */ 0xc6, 0xcc, 0xd8, 0xf0, 0xd8, 0xcc, 0xc6, 0x00, 
   /* 76 0x4c 'L' */ 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0x00, 
   /* 77 0x4d 'M' */ 0x82, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0x00, 
   /* 78 0x4e 'N' */ 0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00, 
   /* 79 0x4f 'O' */ 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 80 0x50 'P' */ 0xfc, 0xc6, 0xc6, 0xfc, 0xc0, 0xc0, 0xc0, 0x00, 
   /* 81 0x51 'Q' */ 0x7c, 0xc6, 0xc6, 0xc6, 0xf6, 0xde, 0x7c, 0x06, 
   /* 82 0x52 'R' */ 0xfc, 0xc6, 0xc6, 0xfc, 0xd8, 0xcc, 0xc6, 0x00, 
   /* 83 0x53 'S' */ 0x7c, 0xc6, 0x60, 0x38, 0x0c, 0xc6, 0x7c, 0x00, 
   /* 84 0x54 'T' */ 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 85 0x55 'U' */ 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 86 0x56 'V' */ 0xc3, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x00, 
   /* 87 0x57 'W' */ 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00, 
   /* 88 0x58 'X' */ 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 
   /* 89 0x59 'Y' */ 0xc3, 0xc3, 0x66, 0x3c, 0x18, 0x18, 0x18, 0x00, 
   /* 90 0x5a 'Z' */ 0xfe, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xfe, 0x00, 
   /* 91 0x5b '[' */ 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00, 
   /* 92 0x5c '\' */ 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x00, 
   /* 93 0x5d ']' */ 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00, 
   /* 94 0x5e '^' */ 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00, 
   /* 95 0x5f '_' */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 
   /* 96 0x60 '`' */ 0x18, 0x18, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 
   /* 97 0x61 'a' */ 0x00, 0x00, 0x7c, 0x06, 0x7e, 0xc6, 0x7e, 0x00, 
   /* 98 0x62 'b' */ 0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 
   /* 99 0x63 'c' */ 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc6, 0x7c, 0x00, 
   /* 100 0x64 'd' */ 0x06, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0x7e, 0x00, 
   /* 101 0x65 'e' */ 0x00, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 
   /* 102 0x66 'f' */ 0x3c, 0x66, 0x60, 0xf0, 0x60, 0x60, 0x60, 0x00, 
   /* 103 0x67 'g' */ 0x00, 0x00, 0x7e, 0xc6, 0xc6, 0x7e, 0x06, 0x7c, 
   /* 104 0x68 'h' */ 0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 
   /* 105 0x69 'i' */ 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 106 0x6a 'j' */ 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0xc6, 0x7c, 
   /* 107 0x6b 'k' */ 0xc0, 0xc0, 0xcc, 0xd8, 0xf0, 0xd8, 0xcc, 0x00, 
   /* 108 0x6c 'l' */ 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 109 0x6d 'm' */ 0x00, 0x00, 0xec, 0xfe, 0xd6, 0xc6, 0xc6, 0x00, 
   /* 110 0x6e 'n' */ 0x00, 0x00, 0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 
   /* 111 0x6f 'o' */ 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 112 0x70 'p' */ 0x00, 0x00, 0xfc, 0xc6, 0xc6, 0xfc, 0xc0, 0xc0, 
   /* 113 0x71 'q' */ 0x00, 0x00, 0x7e, 0xc6, 0xc6, 0x7e, 0x06, 0x06, 
   /* 114 0x72 'r' */ 0x00, 0x00, 0xdc, 0xe6, 0xc0, 0xc0, 0xc0, 0x00, 
   /* 115 0x73 's' */ 0x00, 0x00, 0x7e, 0xc0, 0x7c, 0x06, 0xfc, 0x00, 
   /* 116 0x74 't' */ 0x30, 0x30, 0x7c, 0x30, 0x30, 0x36, 0x1c, 0x00, 
   /* 117 0x75 'u' */ 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 118 0x76 'v' */ 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 
   /* 119 0x77 'w' */ 0x00, 0x00, 0xc6, 0xc6, 0xd6, 0xfe, 0x6c, 0x00, 
   /* 120 0x78 'x' */ 0x00, 0x00, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0x00, 
   /* 121 0x79 'y' */ 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x30, 0x60, 
   /* 122 0x7a 'z' */ 0x00, 0x00, 0xfe, 0x0c, 0x38, 0x60, 0xfe, 0x00, 
   /* 123 0x7b '{' */ 0x0e, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0e, 0x00, 
   /* 124 0x7c '|' */ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 125 0x7d '}' */ 0x70, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x70, 0x00, 
   /* 126 0x7e '~' */ 0x72, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   /* 127 0x7f '' */ 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0x00, 
   /* 128 0x80 '�' */ 0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x0c, 0x78, 
   /* 129 0x81 '�' */ 0xcc, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 
   /* 130 0x82 '�' */ 0x0c, 0x18, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 
   /* 131 0x83 '�' */ 0x7c, 0x82, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00, 
   /* 132 0x84 '�' */ 0xc6, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00, 
   /* 133 0x85 '�' */ 0x30, 0x18, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00, 
   /* 134 0x86 '�' */ 0x30, 0x30, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00, 
   /* 135 0x87 '�' */ 0x00, 0x00, 0x7e, 0xc0, 0xc0, 0x7e, 0x0c, 0x38, 
   /* 136 0x88 '�' */ 0x7c, 0x82, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 
   /* 137 0x89 '�' */ 0xc6, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 
   /* 138 0x8a '�' */ 0x30, 0x18, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 
   /* 139 0x8b '�' */ 0x66, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3c, 0x00, 
   /* 140 0x8c '�' */ 0x7c, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3c, 0x00, 
   /* 141 0x8d '�' */ 0x30, 0x18, 0x00, 0x38, 0x18, 0x18, 0x3c, 0x00, 
   /* 142 0x8e '�' */ 0xc6, 0x38, 0x6c, 0xc6, 0xfe, 0xc6, 0xc6, 0x00, 
   /* 143 0x8f '�' */ 0x38, 0x6c, 0x7c, 0xc6, 0xfe, 0xc6, 0xc6, 0x00, 
   /* 144 0x90 '�' */ 0x18, 0x30, 0xfe, 0xc0, 0xf8, 0xc0, 0xfe, 0x00, 
   /* 145 0x91 '�' */ 0x00, 0x00, 0x7e, 0x18, 0x7e, 0xd8, 0x7e, 0x00, 
   /* 146 0x92 '�' */ 0x3e, 0x6c, 0xcc, 0xfe, 0xcc, 0xcc, 0xce, 0x00, 
   /* 147 0x93 '�' */ 0x7c, 0x82, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 148 0x94 '�' */ 0xc6, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 149 0x95 '�' */ 0x30, 0x18, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 150 0x96 '�' */ 0x78, 0x84, 0x00, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 
   /* 151 0x97 '�' */ 0x60, 0x30, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 
   /* 152 0x98 '�' */ 0xc6, 0x00, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0xfc, 
   /* 153 0x99 '�' */ 0xc6, 0x38, 0x6c, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 
   /* 154 0x9a '�' */ 0xc6, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 155 0x9b '�' */ 0x18, 0x18, 0x7e, 0xc0, 0xc0, 0x7e, 0x18, 0x18, 
   /* 156 0x9c '�' */ 0x38, 0x6c, 0x64, 0xf0, 0x60, 0x66, 0xfc, 0x00, 
   /* 157 0x9d '�' */ 0x66, 0x66, 0x3c, 0x7e, 0x18, 0x7e, 0x18, 0x18, 
   /* 158 0x9e '�' */ 0xf8, 0xcc, 0xcc, 0xfa, 0xc6, 0xcf, 0xc6, 0xc7, 
   /* 159 0x9f '�' */ 0x0e, 0x1b, 0x18, 0x3c, 0x18, 0xd8, 0x70, 0x00, 
   /* 160 0xa0 '�' */ 0x18, 0x30, 0x78, 0x0c, 0x7c, 0xcc, 0x76, 0x00, 
   /* 161 0xa1 '�' */ 0x0c, 0x18, 0x00, 0x38, 0x18, 0x18, 0x3c, 0x00, 
   /* 162 0xa2 '�' */ 0x0c, 0x18, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 
   /* 163 0xa3 '�' */ 0x18, 0x30, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00, 
   /* 164 0xa4 '�' */ 0x76, 0xdc, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x00, 
   /* 165 0xa5 '�' */ 0x76, 0xdc, 0x00, 0xe6, 0xf6, 0xde, 0xce, 0x00, 
   /* 166 0xa6 '�' */ 0x3c, 0x6c, 0x6c, 0x3e, 0x00, 0x7e, 0x00, 0x00, 
   /* 167 0xa7 '�' */ 0x38, 0x6c, 0x6c, 0x38, 0x00, 0x7c, 0x00, 0x00, 
   /* 168 0xa8 '�' */ 0x18, 0x00, 0x18, 0x18, 0x30, 0x63, 0x3e, 0x00, 
   /* 169 0xa9 '�' */ 0x00, 0x00, 0x00, 0xfe, 0xc0, 0xc0, 0x00, 0x00, 
   /* 170 0xaa '�' */ 0x00, 0x00, 0x00, 0xfe, 0x06, 0x06, 0x00, 0x00, 
   /* 171 0xab '�' */ 0x63, 0xe6, 0x6c, 0x7e, 0x33, 0x66, 0xcc, 0x0f, 
   /* 172 0xac '�' */ 0x63, 0xe6, 0x6c, 0x7a, 0x36, 0x6a, 0xdf, 0x06, 
   /* 173 0xad '�' */ 0x18, 0x00, 0x18, 0x18, 0x3c, 0x3c, 0x18, 0x00, 
   /* 174 0xae '�' */ 0x00, 0x33, 0x66, 0xcc, 0x66, 0x33, 0x00, 0x00, 
   /* 175 0xaf '�' */ 0x00, 0xcc, 0x66, 0x33, 0x66, 0xcc, 0x00, 0x00, 
   /* 176 0xb0 '�' */ 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 
   /* 177 0xb1 '�' */ 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 
   /* 178 0xb2 '�' */ 0x77, 0xdd, 0x77, 0xdd, 0x77, 0xdd, 0x77, 0xdd, 
   /* 179 0xb3 '�' */ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
   /* 180 0xb4 '�' */ 0x18, 0x18, 0x18, 0x18, 0xf8, 0x18, 0x18, 0x18, 
   /* 181 0xb5 '�' */ 0x18, 0x18, 0xf8, 0x18, 0xf8, 0x18, 0x18, 0x18, 
   /* 182 0xb6 '�' */ 0x36, 0x36, 0x36, 0x36, 0xf6, 0x36, 0x36, 0x36, 
   /* 183 0xb7 '�' */ 0x00, 0x00, 0x00, 0x00, 0xfe, 0x36, 0x36, 0x36, 
   /* 184 0xb8 '�' */ 0x00, 0x00, 0xf8, 0x18, 0xf8, 0x18, 0x18, 0x18, 
   /* 185 0xb9 '�' */ 0x36, 0x36, 0xf6, 0x06, 0xf6, 0x36, 0x36, 0x36, 
   /* 186 0xba '�' */ 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 
   /* 187 0xbb '�' */ 0x00, 0x00, 0xfe, 0x06, 0xf6, 0x36, 0x36, 0x36, 
   /* 188 0xbc '�' */ 0x36, 0x36, 0xf6, 0x06, 0xfe, 0x00, 0x00, 0x00, 
   /* 189 0xbd '�' */ 0x36, 0x36, 0x36, 0x36, 0xfe, 0x00, 0x00, 0x00, 
   /* 190 0xbe '�' */ 0x18, 0x18, 0xf8, 0x18, 0xf8, 0x00, 0x00, 0x00, 
   /* 191 0xbf '�' */ 0x00, 0x00, 0x00, 0x00, 0xf8, 0x18, 0x18, 0x18, 
   /* 192 0xc0 '�' */ 0x18, 0x18, 0x18, 0x18, 0x1f, 0x00, 0x00, 0x00, 
   /* 193 0xc1 '�' */ 0x18, 0x18, 0x18, 0x18, 0xff, 0x00, 0x00, 0x00, 
   /* 194 0xc2 '�' */ 0x00, 0x00, 0x00, 0x00, 0xff, 0x18, 0x18, 0x18, 
   /* 195 0xc3 '�' */ 0x18, 0x18, 0x18, 0x18, 0x1f, 0x18, 0x18, 0x18, 
   /* 196 0xc4 '�' */ 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
   /* 197 0xc5 '�' */ 0x18, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 
   /* 198 0xc6 '�' */ 0x18, 0x18, 0x1f, 0x18, 0x1f, 0x18, 0x18, 0x18, 
   /* 199 0xc7 '�' */ 0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36, 
   /* 200 0xc8 '�' */ 0x36, 0x36, 0x37, 0x30, 0x3f, 0x00, 0x00, 0x00, 
   /* 201 0xc9 '�' */ 0x00, 0x00, 0x3f, 0x30, 0x37, 0x36, 0x36, 0x36, 
   /* 202 0xca '�' */ 0x36, 0x36, 0xf7, 0x00, 0xff, 0x00, 0x00, 0x00, 
   /* 203 0xcb '�' */ 0x00, 0x00, 0xff, 0x00, 0xf7, 0x36, 0x36, 0x36, 
   /* 204 0xcc '�' */ 0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36, 
   /* 205 0xcd '�' */ 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 
   /* 206 0xce '�' */ 0x36, 0x36, 0xf7, 0x00, 0xf7, 0x36, 0x36, 0x36, 
   /* 207 0xcf '�' */ 0x18, 0x18, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 
   /* 208 0xd0 '�' */ 0x36, 0x36, 0x36, 0x36, 0xff, 0x00, 0x00, 0x00, 
   /* 209 0xd1 '�' */ 0x00, 0x00, 0xff, 0x00, 0xff, 0x18, 0x18, 0x18, 
   /* 210 0xd2 '�' */ 0x00, 0x00, 0x00, 0x00, 0xff, 0x36, 0x36, 0x36, 
   /* 211 0xd3 '�' */ 0x36, 0x36, 0x36, 0x36, 0x3f, 0x00, 0x00, 0x00, 
   /* 212 0xd4 '�' */ 0x18, 0x18, 0x1f, 0x18, 0x1f, 0x00, 0x00, 0x00, 
   /* 213 0xd5 '�' */ 0x00, 0x00, 0x1f, 0x18, 0x1f, 0x18, 0x18, 0x18, 
   /* 214 0xd6 '�' */ 0x00, 0x00, 0x00, 0x00, 0x3f, 0x36, 0x36, 0x36, 
   /* 215 0xd7 '�' */ 0x36, 0x36, 0x36, 0x36, 0xff, 0x36, 0x36, 0x36, 
   /* 216 0xd8 '�' */ 0x18, 0x18, 0xff, 0x18, 0xff, 0x18, 0x18, 0x18, 
   /* 217 0xd9 '�' */ 0x18, 0x18, 0x18, 0x18, 0xf8, 0x00, 0x00, 0x00, 
   /* 218 0xda '�' */ 0x00, 0x00, 0x00, 0x00, 0x1f, 0x18, 0x18, 0x18, 
   /* 219 0xdb '�' */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
   /* 220 0xdc '�' */ 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
   /* 221 0xdd '�' */ 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 
   /* 222 0xde '�' */ 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 
   /* 223 0xdf '�' */ 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
   /* 224 0xe0 '�' */ 0x00, 0x00, 0x76, 0xdc, 0xc8, 0xdc, 0x76, 0x00, 
   /* 225 0xe1 '�' */ 0x78, 0xcc, 0xcc, 0xd8, 0xcc, 0xc6, 0xcc, 0x00, 
   /* 226 0xe2 '�' */ 0xfe, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 
   /* 227 0xe3 '�' */ 0x00, 0x00, 0xfe, 0x6c, 0x6c, 0x6c, 0x6c, 0x00, 
   /* 228 0xe4 '�' */ 0xfe, 0xc6, 0x60, 0x30, 0x60, 0xc6, 0xfe, 0x00, 
   /* 229 0xe5 '�' */ 0x00, 0x00, 0x7e, 0xd8, 0xd8, 0xd8, 0x70, 0x00, 
   /* 230 0xe6 '�' */ 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7c, 0xc0, 
   /* 231 0xe7 '�' */ 0x00, 0x76, 0xdc, 0x18, 0x18, 0x18, 0x18, 0x00, 
   /* 232 0xe8 '�' */ 0x7e, 0x18, 0x3c, 0x66, 0x66, 0x3c, 0x18, 0x7e, 
   /* 233 0xe9 '�' */ 0x38, 0x6c, 0xc6, 0xfe, 0xc6, 0x6c, 0x38, 0x00, 
   /* 234 0xea '�' */ 0x38, 0x6c, 0xc6, 0xc6, 0x6c, 0x6c, 0xee, 0x00, 
   /* 235 0xeb '�' */ 0x0e, 0x18, 0x0c, 0x3e, 0x66, 0x66, 0x3c, 0x00, 
   /* 236 0xec '�' */ 0x00, 0x00, 0x7e, 0xdb, 0xdb, 0x7e, 0x00, 0x00, 
   /* 237 0xed '�' */ 0x06, 0x0c, 0x7e, 0xdb, 0xdb, 0x7e, 0x60, 0xc0, 
   /* 238 0xee '�' */ 0x1e, 0x30, 0x60, 0x7e, 0x60, 0x30, 0x1e, 0x00, 
   /* 239 0xef '�' */ 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 
   /* 240 0xf0 '�' */ 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0x00, 
   /* 241 0xf1 '�' */ 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x7e, 0x00, 
   /* 242 0xf2 '�' */ 0x30, 0x18, 0x0c, 0x18, 0x30, 0x00, 0x7e, 0x00, 
   /* 243 0xf3 '�' */ 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x00, 0x7e, 0x00, 
   /* 244 0xf4 '�' */ 0x0e, 0x1b, 0x1b, 0x18, 0x18, 0x18, 0x18, 0x18, 
   /* 245 0xf5 '�' */ 0x18, 0x18, 0x18, 0x18, 0x18, 0xd8, 0xd8, 0x70, 
   /* 246 0xf6 '�' */ 0x00, 0x18, 0x00, 0x7e, 0x00, 0x18, 0x00, 0x00, 
   /* 247 0xf7 '�' */ 0x00, 0x76, 0xdc, 0x00, 0x76, 0xdc, 0x00, 0x00, 
   /* 248 0xf8 '�' */ 0x38, 0x6c, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00, 
   /* 249 0xf9 '�' */ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 
   /* 250 0xfa '�' */ 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 
   /* 251 0xfb '�' */ 0x0f, 0x0c, 0x0c, 0x0c, 0xec, 0x6c, 0x3c, 0x1c, 
   /* 252 0xfc '�' */ 0x6c, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 
   /* 253 0xfd '�' */ 0x78, 0x0c, 0x18, 0x30, 0x7c, 0x00, 0x00, 0x00, 
   /* 254 0xfe '�' */ 0x00, 0x00, 0x3c, 0x3c, 0x3c, 0x3c, 0x00, 0x00, 
   /* 255 0xff '�' */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   };
   return (unsigned char*)(&spf_font);
}
