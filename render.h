#ifndef __RENDER__
#define __RENDER__

#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "errorHandling.h"
#include "charBuffer.h"

class ScreenRender
{
private:
  int screenrows;
  int screencols;

public:
  int cx, cy;
  ScreenRender();
  int getCursorPosition();
  int getWindowSize();
  void initEditor();
  void editorRefreshScreen();
  void editorDrawRows(CharBuffer &buffer);
  int getWidth();
  int getHeight();
  ~ScreenRender();
};

ScreenRender::ScreenRender()
{
}

int ScreenRender::getCursorPosition()
{
  char buf[32];
  unsigned int i = 0;

  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
    return -1;

  while (i < sizeof(buf) - 1)
  {
    if (read(STDIN_FILENO, &buf[i], 1) != 1)
      break;
    if (buf[i] == 'R')
      break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[')
    return -1;
  if (sscanf(&buf[2], "%d;%d", &screenrows, &screencols) != 2)
    return -1;
  return 0;
}

int ScreenRender::getWindowSize()
{
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
  {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
      return -1;
    return getCursorPosition();
  }
  else
  {
    this->screencols = ws.ws_col;
    this->screenrows = ws.ws_row;
    return 0;
  }
}

void ScreenRender::initEditor()
{
  cx = 1;
  cy = 1;

  if (getWindowSize() == -1)
    errorHandling("window size");
}

void ScreenRender::editorDrawRows(CharBuffer &buffer)
{
  int y;
  for (y = 0; y < this->screenrows; y++)
  {
    if (y == this->screenrows / 3)
    {
      char welcome[80];
      int welcomelen = snprintf(welcome, sizeof(welcome),
                                "--version--");
      if (welcomelen > this->screenrows)
        welcomelen = this->screenrows;

      int padding = (this->screencols - welcomelen) / 2;
      if (padding)
      {
        buffer.appendC("~", 1);
        padding--;
      }
      while (padding--)
        buffer.appendC(" ", 1);
      buffer.appendC(welcome, welcomelen);
    }
    else
    {
      buffer.appendC("~", 1);
    }

    buffer.appendC("\x1b[K", 3);
    if (y < this->screenrows - 1)
      buffer.appendC("\r\n", 2);
  }
}

void ScreenRender::editorRefreshScreen()
{
  CharBuffer buffer;
  buffer.appendC("\x1b[?25l", 6);
  buffer.appendC("\x1b[H", 3);
  editorDrawRows(buffer);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", cy, cx);
  buffer.appendC(buf, strlen(buf));
  buffer.appendC("\x1b[?25h", 6);

  write(STDOUT_FILENO, buffer.getBuff(), buffer.getLength());
}

int ScreenRender::getWidth()
{
  return screencols;
}

int ScreenRender::getHeight()
{
  return screenrows;
}

ScreenRender::~ScreenRender()
{
}

#endif
