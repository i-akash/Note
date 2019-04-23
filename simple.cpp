#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>

using namespace std;



/** define **/
#define CTRL_KEY(key) (key & 0x1f) 


/** data **/
struct editorConfig{
   int screenrows;
   int screencols;
   struct termios terminalState;
}E;


/** error **/
void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}


/** terminal **/
void enableRowMode(struct termios termState){
   termState.c_iflag &=~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
   termState.c_oflag &= ~(OPOST);
   termState.c_cflag |= (CS8);
   termState.c_lflag &=~(ECHO | ICANON | ISIG | IEXTEN);
   
   if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&termState)==-1)
   	    die("tcsetattr");
}

void disableRowMode(){
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&E.terminalState)==-1)
   	    die("tcsetattr");
}

void terminalMode(){
	if(tcgetattr(STDIN_FILENO,&E.terminalState)==-1)
		die("tcgetattr");

	atexit(disableRowMode);
	enableRowMode(E.terminalState);

}


/** Input **/
char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void initEditor() {
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
}

void editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      exit(0);
      break;
    default:
      cout<<c<<"\r\n";  
  }
} 






/** output **/
void editorDrawRows(){
  int y;
  for (y = 0; y < E.screenrows; y++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}

void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[K", 4);
  write(STDOUT_FILENO,"\x1b[H",3);

  editorDrawRows();
  write(STDOUT_FILENO,"\x1b[H",3);
}

 


/** init **/
int main(){
   
   terminalMode();
   initEditor();
   while(1){
       editorRefreshScreen();
       editorProcessKeypress();
   }
   return 0;
}

