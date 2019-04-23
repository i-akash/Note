

#ifndef _TERMINAL_MODE
#define _TERMINAL_MODE
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "errorHandling.h"

class TerminalMode
{
private:
  termios terminal_state;

public:
  TerminalMode();
  void disableRawMode();
  void enableRawMode();
  ~TerminalMode();
};

TerminalMode::TerminalMode()
{
  if (tcgetattr(STDIN_FILENO, &terminal_state) == -1)
    errorHandling("tcgetattr");
}

void TerminalMode::disableRawMode()
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal_state) == -1)
    errorHandling("tcsetattr");
}

void TerminalMode::enableRawMode()
{
  struct termios term_state = this->terminal_state;

  term_state.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  term_state.c_oflag &= ~(OPOST);
  term_state.c_cflag |= (CS8);
  term_state.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_state) == -1)
    errorHandling("tcsetattr");
}

TerminalMode::~TerminalMode()
{
  disableRawMode();
}

#endif
