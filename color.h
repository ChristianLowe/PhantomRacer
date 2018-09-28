#pragma once

#include <cstdio>
#include <ostream>

#define ENABLE_COLORS 1

// Attributes
#define RESET           0
#define BRIGHT          1
#define DIM             2
#define UNDERLINE       3
#define BLINK           4
#define REVERSE         7
#define HIDDEN          8

// Colors
#define BLACK           0
#define RED             1
#define GREEN           2
#define YELLOW          3
#define BLUE            4
#define MAGENTA         5
#define CYAN            6
#define WHITE           7

void setColor(std::ostream &stream, int attr, int fg, int bg) {
#if ENABLE_COLORS
    char command[13];
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    stream << command;
#endif
}

void resetColor(std::ostream &stream) {
#if ENABLE_COLORS
    stream << "\033[0m";
#endif
}
