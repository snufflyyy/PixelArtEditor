#pragma once

#include <raylib.h>

typedef struct {
    Rectangle box;
    bool isActive;
    char text[4]; // it is 4 bc of null terminator
    int numberCount;
} ColorInputBox;
