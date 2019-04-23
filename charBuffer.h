#ifndef __STRINGS__
#define __STRINGS__

#include <stdlib.h>
#include <string.h>
#include <iostream>

class CharBuffer
{
  private:
    char *buff;
    int blen;

  public:
    CharBuffer();
    void appendC(const char *apendedchar, int len);
    char *getBuff();
    int getLength();
    ~CharBuffer();
};

CharBuffer::CharBuffer()
{
    blen = 0;
    buff = nullptr;
}

void CharBuffer::appendC(const char *apendedchar, int len)
{
    char *s = (char *)realloc(buff, blen + len);

    if (s == nullptr)
        return;
    memcpy(&s[blen], apendedchar, len);
    buff = s;
    blen = blen + len;
}

char *CharBuffer::getBuff()
{
    return buff;
}

int CharBuffer::getLength()
{
    return blen;
}

CharBuffer::~CharBuffer()
{
    free(buff);
}

#endif