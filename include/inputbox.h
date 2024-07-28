#pragma once

#include <raylib.h>

typedef struct {
    Rectangle box;
    char text[4]; // it is 4 bc of null terminator
    int numberCount;

    bool hovered;
    bool selected;
} ColorInputBox;

ColorInputBox createColorInputBox();

void setColorInputBoxSize(ColorInputBox *inputBox, float width, float height);
void setColorInputBoxPosition(ColorInputBox *inputBox, float x, float y);

bool isColorInputBoxClicked(ColorInputBox *inputBox);

void getInputColorInputBoxInput(ColorInputBox* inputBox);

void updateColorInputBox(ColorInputBox *inputBox);
void renderColorInputBox(ColorInputBox inputBox, Font font, int fontSize, Color inputBoxColor, Color inputBoxHoverColor, Color inputBoxSelectedColor, Color textColor);
