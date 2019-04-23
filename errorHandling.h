#ifndef __ERROR__
#define __ERROR__
#include <stdio.h>
void errorHandling(const char *s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(0);
}
#endif