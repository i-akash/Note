#ifndef __INPUT__
#define __INPUT__

#include <unistd.h>
#include <errno.h>

#include "errorHandling.h"
#include "defination.h"
#include "render.h"

enum editorKey
{
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    PAGE_UP,
    PAGE_DOWN
};

class Input
{
  private:
    ScreenRender *screenRender;

  public:
    Input(ScreenRender *screenRender);
    int editorReadKey();
    void editorMoveCursor(int key);
    void editorProcessKeypress();
    ~Input();
};

Input::Input(ScreenRender *screenRender)
{
    this->screenRender = screenRender;
}

int Input::editorReadKey()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
            errorHandling("read");
    }
    if (c == '\x1b')
    {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1)
            return '\x1b';

        if (read(STDIN_FILENO, &seq[1], 1) != 1)
            return '\x1b';

        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                if (read(STDIN_FILENO, &seq[2], 1) != 1)
                    return '\x1b';

                if (seq[2] == '~')
                {
                    switch (seq[1])
                    {
                    case '5':
                        return PAGE_UP;
                    case '6':
                        return PAGE_DOWN;
                    }
                }
            }
            else
            {
                switch (seq[1])
                {
                case 'A':
                    return ARROW_UP;
                case 'B':
                    return ARROW_DOWN;
                case 'C':
                    return ARROW_RIGHT;
                case 'D':
                    return ARROW_LEFT;
                }
            }
        }
        return '\x1b';
    }
    else
    {
        return c;
    }
}

void Input::editorMoveCursor(int key)
{
    switch (key)
    {
    case ARROW_LEFT:
        screenRender->cx != 0 ? screenRender->cx-- : 0;
        break;
    case ARROW_RIGHT:
        screenRender->cx != screenRender->getWidth() - 1 ? screenRender->cx++ : screenRender->getWidth() - 1;
        break;
    case ARROW_DOWN:
        screenRender->cy != screenRender->getHeight() - 1 ? screenRender->cy++ : screenRender->getHeight() - 1;
        break;
    case ARROW_UP:
        screenRender->cy != 0 ? screenRender->cy-- : 0;
        break;
    }
}

void Input::editorProcessKeypress()
{
    int c = editorReadKey();

    switch (c)
    {
    case CTRL_KEY('q'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
    case PAGE_UP:
        screenRender->cy = 0;
        break;
    case PAGE_DOWN:
        screenRender->cy = 0;
        break;
    default:
        editorMoveCursor(c);
    }
}

Input::~Input()
{
}

#endif