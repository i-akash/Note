#include <iostream>

// custom include
#include "terminalMode.h"
#include "input.h"
#include "render.h"

TerminalMode terminalMode;

int main()
{

  // process
  ScreenRender screenRender;
  Input input(&screenRender);
  terminalMode.enableRawMode();
  screenRender.initEditor();
  while (1)
  {
    // std::cout << screenRender.cx << "\r\n";
    // // screenRender.cx++;
    screenRender.editorRefreshScreen();
    input.editorProcessKeypress();
  }
  return 0;
}