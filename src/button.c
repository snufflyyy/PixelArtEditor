#include "button.h"

#include <stdio.h>
#include <raylib.h>

Button createButton(Texture2D icon) {
    Button button;

    button.box = (Rectangle) {0, 0, 100, 100};
    button.icon = icon;

    return button;
}

void setButtonSize(Button *button, float width, float height) {
    button->box.width = width;
    button->box.height = height;
}

void setButtonPosition(Button *button, float x, float y) {
    button->box.x = x;
    button->box.y = y;
}

// call this function after setButtonSize and setButtonPosition
// for best results`!
void resizeButtonIcon(Button *button, float padding) {
    button->icon.width = button->box.height - padding / 2;
    button->icon.height = button->icon.width;
}

void updateButton(Button *button) {
    if (CheckCollisionPointRec(GetMousePosition(), button->box)) {
        button->hovered = true;
    } else {
        button->hovered = false;
    }
}

bool isButtonClicked(Button button) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), button.box)) {
        return true;
    }

    return false;
}

bool isButtonPressed(Button button) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), button.box)) {
        return true;
    }

    return false;
}

void renderButton(Button button, float roundness, Color color, Color hoverColor, Color clickColor, Color iconColor) {
    Color buttonColor;

    if (isButtonPressed(button)) {
        buttonColor = clickColor;
    } else if (button.hovered) {
        buttonColor = hoverColor;
    } else {
        buttonColor = color;
    }

    DrawRectangleRounded(button.box, 1, 10, buttonColor);
    DrawTextureV(button.icon, (Vector2) {button.box.x + button.box.width / 2 - (float) button.icon.width / 2, button.box.y + button.box.height / 2 - (float) button.icon.height / 2}, iconColor);
}
