#pragma once

#include <raylib.h>

typedef struct {
    Rectangle box;
    bool isActive;
    char text[4]; // it is 4 bc of null terminator
    int numberCount;
} ColorInputBox;

ColorInputBox createColorInputBox();

void getColorInputBoxInput(ColorInputBox* inputBox);

void renderColorInputBox(ColorInputBox inputBox, Font font, int fontSize, Color buttonColor, Color textColor);
