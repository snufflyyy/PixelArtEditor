#pragma once

#include <raylib.h>

typedef struct {
    Rectangle box;
    Texture2D icon;
    bool hovered;
} Button;

Button createButton(Texture2D icon);

void setButtonSize(Button *button, float width, float height);
void setButtonPosition(Button *button, float x, float y);

void resizeButtonIcon(Button *button, float padding);

void updateButton(Button *button);

bool isButtonClicked(Button button);
bool isButtonPressed(Button button);

void renderButton(Button button, float roundness, Color color, Color hoverColor, Color clickColor, Color iconColor);
