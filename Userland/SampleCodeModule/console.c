#include <screenManager.h>
#include <syscall.h>
#include <console.h>
#include <stdio.h>
#include <commandDispatcher.h>
#include <font.h>
#include <stdlib.h>

#define HORIZONTAL_MARGIN 2
#define VERTICAL_MARGIN 0
#define MAX_LINE_POSITION (windowWidth-(HORIZONTAL_MARGIN*(CHAR_WIDTH)))
#define NUM_OF_COLORS 6
#define BLACK 0
#define WHITE 1
#define BLUE 2
#define RED 3
#define GREEN 4
#define YELLOW 5



char buffer[20000];
int bufferIndex = 0;

static const char* consoleName = "Consola\\::";

static const struct RGB consoleColors[6] = {{0,0,0},{255,255,255},{50,50,255},{255, 50, 50},{50,255,50},{189,183,107}};
static int consoleBackgroundIndex;
static int inOutIndex;
static int errIndex;
static int consoleIndex;

int windowWidth;
int windowHeight;
int letterSize = 1;
int line=VERTICAL_MARGIN, linePosition=HORIZONTAL_MARGIN;

void console(){
  init();
  consoleLoop();
}


void init(){
  windowWidth = _syscall(_getScreenWidth);
  windowHeight = _syscall(_getScreenHeight);
  consoleBackgroundIndex = BLACK;
  inOutIndex = WHITE;
  errIndex = RED;
  consoleIndex = BLUE;
  _syscall(_fillScreen,consoleColors[consoleBackgroundIndex]);
  printf("Welcome to MikeOS. To get started, type man to check the different programs.");
}

void consoleLoop(){
  stdout();
  stderr();
  while(1){
    stdin();
    stdout();
    stderr();
  }
}


void checkSpace(){
  if((linePosition + 1) * (CHAR_WIDTH + 1) * letterSize > MAX_LINE_POSITION)
    newLine();
  else if(line*letterSize*CHAR_HEIGHT >= windowHeight-VERTICAL_MARGIN*CHAR_HEIGHT*letterSize){
    _syscall(_movePixelsUp, letterSize*CHAR_HEIGHT, consoleColors[consoleBackgroundIndex]);
    line--;
  }
}

void stdin(){
  bufferIndex=0;
  const char * text = consoleName;
  while(*text){
    writeChar(*text, linePosition*letterSize*(CHAR_WIDTH + 1), line*letterSize*CHAR_HEIGHT, consoleColors[consoleIndex], consoleColors[consoleBackgroundIndex], letterSize);
    linePosition++;
    text++;
  }
  char c;
  while((c = _syscall(_read, 0)) != '\n'){
    if(c==8){
      if(bufferIndex!=0){
        if(linePosition <= HORIZONTAL_MARGIN){
          line--;
          linePosition=(MAX_LINE_POSITION / ((CHAR_WIDTH + 1) * letterSize)) - 1;
        }else
          linePosition--;

        bufferIndex--;
        buffer[bufferIndex] = 0;
        _syscall(_writeBlock,(linePosition)*letterSize*(CHAR_WIDTH + 1), line*letterSize*CHAR_HEIGHT, consoleColors[consoleBackgroundIndex], (CHAR_WIDTH + 1)*letterSize, CHAR_HEIGHT*letterSize);
      }
    }else if(c){
      checkSpace();
      buffer[bufferIndex++] = c;
      writeChar(c, linePosition*letterSize*(CHAR_WIDTH + 1), line*letterSize*CHAR_HEIGHT, consoleColors[inOutIndex] , consoleColors[consoleBackgroundIndex], letterSize);
      linePosition++;
    }
  }
  newLine();
  commandDispatcher(buffer);
  int i;
  for(i = 0; i < bufferIndex; i++)
    buffer[i] = 0;
  bufferIndex = 0;
}

void stdout(){
  int flag=0;
  char c;
  while((c=_syscall(_read, 1))!=0){
    flag=1;
    if(c == '\n'){
      newLine();
    }else{
      checkSpace();
      writeChar(c, linePosition*letterSize*(CHAR_WIDTH + 1), line*letterSize*CHAR_HEIGHT, consoleColors[inOutIndex], consoleColors[consoleBackgroundIndex], letterSize);
      linePosition++;
    }
  }
  if(flag)
    newLine();
}

void stderr(){
  int flag;
  char c;
  while((c=_syscall(_read, 2))!=0){
    flag=1;
    if(c == '\n'){
      newLine();
    }else{
      checkSpace();
      writeChar(c, linePosition*letterSize*(CHAR_WIDTH + 1), line*letterSize*CHAR_HEIGHT, consoleColors[errIndex], consoleColors[consoleBackgroundIndex], letterSize);
      linePosition++;
    }
  }
  if(flag)
    newLine();
}

void resetConsole(){
  _syscall(_fillScreen, consoleColors[consoleBackgroundIndex]);
  linePosition = HORIZONTAL_MARGIN;
  line = VERTICAL_MARGIN;
}

void newLine(){
  line++;
  linePosition = HORIZONTAL_MARGIN;
  checkSpace();
}

void changeTheme(char * param) {
  int c1 = inOutIndex;
  int c2 = consoleBackgroundIndex;
  int c3 = errIndex;
  int c4 = consoleIndex;

  int aux = c1;

  if(strcmp(param, "background")) {
      c1 = c2;
      c2 = aux;
  }
  else if(strcmp(param, "error")){
    c1 = c3;
    c3 = aux;
  }
  else if(strcmp(param, "consoletext")){
    c1 = c4;
    c4 = aux;
  }
  else if(strcmp(param, "text")== 0) {
    checkSpace();
    writeString("Invalid parameter.", linePosition*letterSize*(CHAR_WIDTH + 1), line*letterSize*CHAR_HEIGHT, consoleColors[errIndex],consoleColors[consoleBackgroundIndex], letterSize);
    linePosition = HORIZONTAL_MARGIN;
    line++;
    return;
  }

  int oldIndex = c1;
  c1++;
  while(c1 == c2 || c1 == c3 || c1 == c4 || c1 >= NUM_OF_COLORS) {
    c1++;
    if(c1 >= NUM_OF_COLORS)
      c1 = 0;
  }

  if(strcmp(param, "background"))
    consoleBackgroundIndex = c1;
  else if(strcmp(param, "error"))
    errIndex = c1;
  else if(strcmp(param, "consoletext"))
    consoleIndex = c1;
  else if(strcmp(param, "text"))
    inOutIndex = c1;

  _syscall(_replaceColor, consoleColors[oldIndex], consoleColors[c1]);
}
