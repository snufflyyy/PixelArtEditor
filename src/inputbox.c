#include "inputbox.h"

#include <raylib.h>

ColorInputBox createColorInputBox() {
    ColorInputBox inputBox;

    inputBox.box = (Rectangle) {0, 0, 100, 100};
    inputBox.selected = false;

    inputBox.text[0] = '2';
    inputBox.text[1] = '5';
    inputBox.text[2] = '5';
    inputBox.text[3] = '\0';
    inputBox.numberCount = 3;

    return inputBox;
}

void setColorInputBoxSize(ColorInputBox *inputBox, float width, float height) {
    inputBox->box.width = width;
    inputBox->box.height = height;
}

void setColorInputBoxPosition(ColorInputBox *inputBox, float x, float y) {
    inputBox->box.x = x;
    inputBox->box.y = y;
}

bool isColorInputBoxClicked(ColorInputBox *inputBox) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), inputBox->box)) {
        inputBox->selected = true;

        return true;
    }

    return false;
}

void getInputColorInputBoxInput(ColorInputBox* inputBox) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);

    int key = GetCharPressed();
    while (key > 0) {
        // only allow numbers
        if ((key >= 48) && (key <= 57)) {
            if (inputBox->numberCount < 3) {
                inputBox->text[inputBox->numberCount] = (char) key;
                inputBox->text[inputBox->numberCount + 1] = '\0';
                inputBox->numberCount++;
            }
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        inputBox->numberCount--;
        if (inputBox->numberCount < 0) {
            inputBox->numberCount = 0;
        }
        inputBox->text[inputBox->numberCount] = '\0';
    }
}

void updateColorInputBox(ColorInputBox *inputBox) {
    if (CheckCollisionPointRec(GetMousePosition(), inputBox->box)) {
        inputBox->hovered = true;
    } else {
        inputBox->hovered = false;
    }
}

void renderColorInputBox(ColorInputBox inputBox, Font font, int fontSize, Color inputBoxColor, Color inputBoxHoverColor, Color inputBoxSelectedColor, Color textColor) {
    Color color;

    if (inputBox.selected) {
        color = inputBoxSelectedColor;
    } else if (inputBox.hovered) {
        color = inputBoxHoverColor;
    } else {
        color = inputBoxColor;
    }

    DrawRectangleRounded(inputBox.box, 1, 10, color);

    Vector2 inputBoxTextSize = MeasureTextEx(font, inputBox.text, fontSize, 0);
    DrawTextEx(font, inputBox.text, (Vector2) {(inputBox.box.x + inputBox.box.width / 2) - inputBoxTextSize.x / 2, (inputBox.box.y + inputBox.box.height / 2) - inputBoxTextSize.y / 2}, fontSize, 0, textColor);
}
