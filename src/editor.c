#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <raylib.h>

#include "button.h"
#include "editor.h"
#include "inputbox.h"
#include "pixel.h"

// colors
const Color panelColor = {100, 100, 100, 50};

const Color buttonColor = {100, 100, 100, 50};
const Color buttonHoverColor = {100, 100, 100, 75};
const Color buttonClickedColor = {100, 100, 100, 100};
const Color buttonIconColor = {100, 100, 100, 150};

const Color colorInputBoxColor = {100, 100, 100, 50};
const Color colorInputBoxHoverColor = {100, 100, 100, 75};
const Color colorInputBoxSelectedColor = {100, 100, 100, 100};
const Color colorInputBoxTextColor = {100, 100, 100, 150};

float padding;

Rectangle framesBackgroundPanel;
Rectangle canvasBackgroundPanel;
Rectangle colorPickerBackgroundPanel;

float canvasBackgroundRoundness;
float sizePanelBackgroundRoundness;

Button addFrameButton;

int numberOfFrames;
int currentFrame;
int canvasSize;
Pixel ***canvasPixels;
float pixelSize;

Color currentColor = WHITE;

Rectangle colorPickerPreview;

Font robotoFont;

ColorInputBox *selectedColorInputBox;
ColorInputBox redInputBox;
ColorInputBox greenInputBox;
ColorInputBox blueInputBox;
float colorInputBoxFontSize;

Button favoriteColorButton;
Button applyColorButton;

void createFrame() {
    numberOfFrames++;
    canvasPixels = realloc(canvasPixels, numberOfFrames * sizeof(Pixel**));

    canvasPixels[numberOfFrames - 1] = malloc(canvasSize * sizeof(Pixel*));
    for (int x = 0; x < canvasSize; x++) {
        canvasPixels[numberOfFrames - 1][x] = malloc(canvasSize * sizeof(Pixel));
    }

    // false = white | true = gray
    bool color = false;
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            if (color) {
                canvasPixels[numberOfFrames - 1][x][y].color = LIGHTGRAY;
            } else {
                canvasPixels[numberOfFrames - 1][x][y].color = WHITE;
            }

            color = !color;
        }
        color = !color;
    }
}

void createEditor(int size) {
    canvasSize = size;

    robotoFont = LoadFont("assets/fonts/roboto/Roboto-Bold.ttf");

    addFrameButton = createButton(LoadTexture("assets/icons/plus.png"));
    favoriteColorButton = createButton(LoadTexture("assets/icons/star.png"));
    applyColorButton = createButton(LoadTexture("assets/icons/check.png"));

    createFrame();

    // color input boxes
    redInputBox = createColorInputBox();
    greenInputBox = createColorInputBox();
    blueInputBox = createColorInputBox();

    resizeEditor();
}

void resizeEditor() {
    // padding
    padding = (float) (GetScreenWidth() + GetScreenHeight()) / 200;

    // background panels
    framesBackgroundPanel.width = ((float) (GetScreenWidth() - GetScreenHeight()) / 2) - padding;
    framesBackgroundPanel.height = GetScreenHeight() - padding * 2;
    framesBackgroundPanel.x = padding;
    framesBackgroundPanel.y = (float) GetScreenHeight() / 2 - framesBackgroundPanel.height / 2;

    canvasBackgroundPanel.width = GetScreenHeight() - padding * 2;
    canvasBackgroundPanel.height = GetScreenHeight() - padding * 2;
    canvasBackgroundPanel.x = (float) GetScreenWidth() / 2 - canvasBackgroundPanel.width / 2;
    canvasBackgroundPanel.y = (float) GetScreenHeight() / 2 - canvasBackgroundPanel.height / 2;

    colorPickerBackgroundPanel.width = ((float) (GetScreenWidth() - GetScreenHeight()) / 2) - padding;
    colorPickerBackgroundPanel.height = GetScreenHeight() - padding * 2;
    colorPickerBackgroundPanel.x = (float) GetScreenWidth() - colorPickerBackgroundPanel.width - padding;
    colorPickerBackgroundPanel.y = (float) GetScreenHeight() / 2 - colorPickerBackgroundPanel.height / 2;

    // borders (i wanna rewrite like all of the border stuff)
    // thank u chatgpt for this crazy math shit i dont know how to do!
    float smallestDimension = fmin(fmin(canvasBackgroundPanel.width, canvasBackgroundPanel.height),
                                   fmin(colorPickerBackgroundPanel.width, colorPickerBackgroundPanel.height));

    float radius = smallestDimension * 0.1f / 2;

    canvasBackgroundRoundness = (canvasBackgroundPanel.width > canvasBackgroundPanel.height) ?
                                (2 * radius / canvasBackgroundPanel.height) :
                                (2 * radius / canvasBackgroundPanel.width);

    sizePanelBackgroundRoundness = (colorPickerBackgroundPanel.width > colorPickerBackgroundPanel.height) ?
                                   (2 * radius / colorPickerBackgroundPanel.height) :
                                   (2 * radius / colorPickerBackgroundPanel.width);

    // add frame button
    setButtonSize(&addFrameButton, framesBackgroundPanel.width - padding * 4, framesBackgroundPanel.height / 35);
    setButtonPosition(&addFrameButton, framesBackgroundPanel.x + padding * 2, framesBackgroundPanel.y + framesBackgroundPanel.height - padding * 2 - addFrameButton.box.height);
    resizeButtonIcon(&addFrameButton, padding);

    // canvas
    pixelSize = (GetScreenHeight() - padding * 6) / canvasSize;

    float canvasPixelsXStartingPos = (float) GetScreenWidth() / 2 - (GetScreenHeight() - padding * 6) / 2;
    float canvasPixelsYStartingPos = (float) GetScreenHeight() / 2 - (GetScreenHeight() - padding * 6) / 2;

    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            canvasPixels[currentFrame][x][y].position.x = canvasPixelsXStartingPos + pixelSize * x;
            canvasPixels[currentFrame][x][y].position.y = canvasPixelsYStartingPos + pixelSize * y;
        }
    }

    // color picker box thing
    colorPickerPreview.width = colorPickerBackgroundPanel.width - padding * 4;
    colorPickerPreview.height = colorPickerPreview.width;
    colorPickerPreview.x = colorPickerBackgroundPanel.x + padding * 2;
    colorPickerPreview.y = colorPickerBackgroundPanel.y + padding * 2;

    Vector2 colorInputBoxesSize = {(colorPickerBackgroundPanel.width - padding * 6) / 3, colorPickerBackgroundPanel.height / 35};
    float colorInputBoxesYPosition = colorPickerPreview.y + colorPickerPreview.height + padding;

    colorInputBoxFontSize = (colorInputBoxesSize.x + colorInputBoxesSize.y) / 4;

    // color input boxes
    setColorInputBoxSize(&redInputBox, colorInputBoxesSize.x, colorInputBoxesSize.y);
    setColorInputBoxPosition(&redInputBox, colorPickerPreview.x, colorInputBoxesYPosition);

    setColorInputBoxSize(&greenInputBox, colorInputBoxesSize.x, colorInputBoxesSize.y);
    setColorInputBoxPosition(&greenInputBox, colorPickerPreview.x + redInputBox.box.width + padding, colorInputBoxesYPosition);

    setColorInputBoxSize(&blueInputBox, colorInputBoxesSize.x, colorInputBoxesSize.y);
    setColorInputBoxPosition(&blueInputBox, colorPickerPreview.x + (greenInputBox.box.width + padding) * 2, colorInputBoxesYPosition);

    // favorite button
    setButtonSize(&favoriteColorButton, (colorPickerBackgroundPanel.width - padding * 5) / 2, colorPickerBackgroundPanel.height / 35);
    setButtonPosition(&favoriteColorButton, colorPickerPreview.x, redInputBox.box.y + redInputBox.box.height + padding);
    resizeButtonIcon(&favoriteColorButton, padding);

    // apply button
    setButtonSize(&applyColorButton, (colorPickerBackgroundPanel.width - padding * 5) / 2, colorPickerBackgroundPanel.height / 35);
    setButtonPosition(&applyColorButton, colorPickerPreview.x + favoriteColorButton.box.width + padding, redInputBox.box.y + redInputBox.box.height + padding);
    resizeButtonIcon(&applyColorButton, padding);
}

void updateEditor() {
    if (selectedColorInputBox != NULL) {
        getInputColorInputBoxInput(selectedColorInputBox);
    }

    // buttons
    if (CheckCollisionPointRec(GetMousePosition(), framesBackgroundPanel)) {
        updateButton(&addFrameButton);

        if (isButtonClicked(addFrameButton)) {
            createFrame();
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), colorPickerBackgroundPanel)) {
        updateColorInputBox(&redInputBox);
        updateColorInputBox(&greenInputBox);
        updateColorInputBox(&blueInputBox);

        updateButton(&favoriteColorButton);
        updateButton(&applyColorButton);

        // for the color input boxes
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (selectedColorInputBox != NULL) {
                selectedColorInputBox->selected = false;
            }

            if (isColorInputBoxClicked(&redInputBox)) {
                selectedColorInputBox = &redInputBox;
            } else if (isColorInputBoxClicked(&greenInputBox)) {
                selectedColorInputBox = &greenInputBox;
            } else if (isColorInputBoxClicked(&blueInputBox)) {
                selectedColorInputBox = &blueInputBox;
            } else {
                selectedColorInputBox = NULL;

                SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                int redColor = atoi(redInputBox.text);
                int greenColor = atoi(greenInputBox.text);
                int blueColor = atoi(blueInputBox.text);

                if (redColor <= 255) {
                    currentColor.r = redColor;
                }
                if (greenColor <= 255) {
                    currentColor.g = greenColor;
                }
                if (blueColor <= 255) {
                    currentColor.b = blueColor;
                }
            }
        }
    }

    // canvas panel
    Rectangle pixel;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), canvasBackgroundPanel)) {
        for (int x = 0; x < canvasSize; x++) {
            for (int y = 0; y < canvasSize; y++) {
                pixel.x = canvasPixels[currentFrame][x][y].position.x;
                pixel.y = canvasPixels[currentFrame][x][y].position.y;
                pixel.width = pixelSize;
                pixel.height = pixelSize;

                if (CheckCollisionPointRec(GetMousePosition(), pixel)) {
                    canvasPixels[currentFrame][x][y].color = currentColor;
                }
            }
        }
    }
}

void renderEditor() {
    // background panels
    DrawRectangleRounded(framesBackgroundPanel, sizePanelBackgroundRoundness, 10, panelColor);
    DrawRectangleRounded(canvasBackgroundPanel, canvasBackgroundRoundness, 10, panelColor);
    DrawRectangleRounded(colorPickerBackgroundPanel, sizePanelBackgroundRoundness, 10, panelColor);

    // add frame button
    renderButton(addFrameButton, 1, buttonColor,buttonHoverColor, buttonClickedColor, buttonIconColor);

    // canvas
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            DrawRectangleV(canvasPixels[currentFrame][x][y].position, (Vector2) {pixelSize, pixelSize}, canvasPixels[currentFrame][x][y].color);
        }
    }

    // color picker box thing | magic number (fix later!)
    DrawRectangleRounded(colorPickerPreview, 0.1f, 10, currentColor);

    renderColorInputBox(redInputBox, robotoFont, colorInputBoxFontSize, colorInputBoxColor, colorInputBoxHoverColor, colorInputBoxSelectedColor, colorInputBoxTextColor);
    renderColorInputBox(greenInputBox, robotoFont, colorInputBoxFontSize, colorInputBoxColor, colorInputBoxHoverColor, colorInputBoxSelectedColor, colorInputBoxTextColor);
    renderColorInputBox(blueInputBox, robotoFont, colorInputBoxFontSize, colorInputBoxColor, colorInputBoxHoverColor, colorInputBoxSelectedColor, colorInputBoxTextColor);

    renderButton(favoriteColorButton, 1, buttonColor, buttonHoverColor, buttonClickedColor, buttonIconColor);
    renderButton(applyColorButton, 1, buttonColor, buttonHoverColor, buttonClickedColor, buttonIconColor);
}

void destoryEditor() {
    free(canvasPixels);
    canvasPixels = NULL;

    free(selectedColorInputBox);
    selectedColorInputBox = NULL;

    UnloadTexture(addFrameButton.icon);
}
