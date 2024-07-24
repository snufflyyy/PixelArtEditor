#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <raylib.h>

#include "editor.h"
#include "inputbox.h"
#include "pixel.h"

Rectangle framesBackgroundPanel;
Rectangle canvasBackgroundPanel;
Rectangle colorPickerBackgroundPanel;

Rectangle addFrameButton;
Texture2D addFramePlusIcon;

float canvasBackgroundRoundness;
float colorPickerBackgroundRoundness; // this one is also used for the frames background

// now calculated based on screen size :)
float padding;

int numberOfFrames;
int canvasSize;

int currentFrame;
Pixel ***canvasPixels; // never thought i see the day where triple pointers
                       // are used for something that isn't retarded! :O

// calculated in the resizeEditor function and is based on size of canvas
float pixelSize;

Rectangle colorPickerPreview;

Color currentColor = WHITE;

Font robotoFont;

ColorInputBox *selectedColorInputBox;

ColorInputBox redInputBox;
ColorInputBox greenInputBox;
ColorInputBox blueInputBox;
float colorInputBoxFontSize;

Rectangle favoriteColorButton;
Texture2D favoriteIcon;

Rectangle applyColorButton;
Texture2D applyColorIcon;

Rectangle colorPickerSeparatorLine;

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

    // color input boxes
    redInputBox = createColorInputBox();
    greenInputBox = createColorInputBox();
    blueInputBox = createColorInputBox();

    // textures
    addFramePlusIcon = LoadTexture("assets/icons/plus.png");
    favoriteIcon = LoadTexture("assets/icons/star.png");
    applyColorIcon = LoadTexture("assets/icons/check.png");

    createFrame();
    resizeEditor();
}

void resizeEditor() {
    // padding
    padding = (float) (GetScreenWidth() + GetScreenHeight()) / 200;

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

    // borders
    // thank u chatgpt for this crazy math shit i dont know how to do!
    float smallestDimension = fmin(fmin(canvasBackgroundPanel.width, canvasBackgroundPanel.height),
                                   fmin(colorPickerBackgroundPanel.width, colorPickerBackgroundPanel.height));

    float radius = smallestDimension * 0.1f / 2;

    canvasBackgroundRoundness = (canvasBackgroundPanel.width > canvasBackgroundPanel.height) ?
                                (2 * radius / canvasBackgroundPanel.height) :
                                (2 * radius / canvasBackgroundPanel.width);

    colorPickerBackgroundRoundness = (colorPickerBackgroundPanel.width > colorPickerBackgroundPanel.height) ?
                                     (2 * radius / colorPickerBackgroundPanel.height) :
                                     (2 * radius / colorPickerBackgroundPanel.width);

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

    // color input boxes font size
    colorInputBoxFontSize = (float) (colorPickerBackgroundPanel.width + colorPickerBackgroundPanel.height) / 40;

    // color input boxes
    redInputBox.box.width = (colorPickerPreview.width - padding * 2) / 3;
    redInputBox.box.height = colorPickerPreview.height / 8; // magic bullshit
    redInputBox.box.x = colorPickerPreview.x;
    redInputBox.box.y = colorPickerPreview.y + colorPickerPreview.height + padding * 2;

    greenInputBox.box.width = redInputBox.box.width;
    greenInputBox.box.height = redInputBox.box.height;
    greenInputBox.box.x = colorPickerPreview.x + greenInputBox.box.width + padding;
    greenInputBox.box.y = colorPickerPreview.y + colorPickerPreview.height + padding * 2;

    blueInputBox.box.width = greenInputBox.box.width;
    blueInputBox.box.height = greenInputBox.box.height;
    blueInputBox.box.x = colorPickerPreview.x + greenInputBox.box.width * 2 + padding * 2;
    blueInputBox.box.y = colorPickerPreview.y + colorPickerPreview.height + padding * 2;

    favoriteColorButton.width = (colorPickerPreview.width / 2) - padding / 2;
    favoriteColorButton.height = colorPickerBackgroundPanel.height / 35;
    favoriteColorButton.x = redInputBox.box.x;
    favoriteColorButton.y = redInputBox.box.y + redInputBox.box.height + padding * 2;

    applyColorButton.width = (colorPickerPreview.width / 2) - padding / 2;
    applyColorButton.height = colorPickerBackgroundPanel.height / 35;
    applyColorButton.x = favoriteColorButton.x + favoriteColorButton.width + padding;
    applyColorButton.y = favoriteColorButton.y;

    colorPickerSeparatorLine.width = colorPickerPreview.width;
    colorPickerSeparatorLine.height = colorPickerBackgroundPanel.height / 200;
    colorPickerSeparatorLine.x = favoriteColorButton.x;
    colorPickerSeparatorLine.y = favoriteColorButton.y + favoriteColorButton.height + padding * 2;

    // add frame button
    addFrameButton.width = framesBackgroundPanel.width - padding * 4;
    addFrameButton.height = framesBackgroundPanel.height / 35; // magic bullshit
    addFrameButton.x = framesBackgroundPanel.x + padding * 2;
    addFrameButton.y = framesBackgroundPanel.y + framesBackgroundPanel.height - padding * 2 - addFrameButton.height;

    // add frame plus icon
    addFramePlusIcon.width = addFrameButton.height - addFrameButton.height / 5; // magic bullshit
    addFramePlusIcon.height = addFramePlusIcon.width;
}

void updateEditor() {
    if (selectedColorInputBox != NULL) {
        getColorInputBoxInput(selectedColorInputBox);
    }

    // frames panel & color picker panel
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        redInputBox.isActive = false;
        greenInputBox.isActive = false;
        blueInputBox.isActive = false;

        if (CheckCollisionPointRec(GetMousePosition(), framesBackgroundPanel)) {
            if (CheckCollisionPointRec(GetMousePosition(), addFrameButton)) {
                createFrame();
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), colorPickerBackgroundPanel)) {
            if (CheckCollisionPointRec(GetMousePosition(), redInputBox.box)) {
                redInputBox.isActive = true;
            } else if (CheckCollisionPointRec(GetMousePosition(), greenInputBox.box)) {
                greenInputBox.isActive = true;
            } else if (CheckCollisionPointRec(GetMousePosition(), blueInputBox.box)) {
                blueInputBox.isActive = true;
            } else {
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

    if (redInputBox.isActive) {
        selectedColorInputBox = &redInputBox;
    } else if (greenInputBox.isActive) {
        selectedColorInputBox = &greenInputBox;
    } else if (blueInputBox.isActive) {
        selectedColorInputBox = &blueInputBox;
    } else {
        selectedColorInputBox = NULL;
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
    DrawRectangleRounded(framesBackgroundPanel, colorPickerBackgroundRoundness, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(canvasBackgroundPanel, canvasBackgroundRoundness, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(colorPickerBackgroundPanel, colorPickerBackgroundRoundness, 10, (Color) {100, 100, 100, 50});

    // magic number (fix later!)
    DrawRectangleRounded(colorPickerPreview, 0.1f, 10, currentColor);

    renderColorInputBox(redInputBox, robotoFont, colorInputBoxFontSize, (Color) {100, 100, 100, 50}, (Color) {100, 100, 100, 150});
    renderColorInputBox(greenInputBox, robotoFont, colorInputBoxFontSize, (Color) {100, 100, 100, 50}, (Color) {100, 100, 100, 150});
    renderColorInputBox(blueInputBox, robotoFont, colorInputBoxFontSize, (Color) {100, 100, 100, 50}, (Color) {100, 100, 100, 150});

    if (selectedColorInputBox != NULL) {
        DrawRectangleRoundedLinesEx(selectedColorInputBox->box, 1, 10, 2, (Color) {100, 100, 100, 150});
    }

    DrawRectangleRounded(favoriteColorButton, 1, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(applyColorButton, 1, 10, (Color) {100, 100, 100, 50});

    DrawRectangleRounded(colorPickerSeparatorLine, 1, 10, (Color) {100, 100, 100, 50});

    // add frame button
    DrawRectangleRounded(addFrameButton, 1, 10, (Color) {100, 100, 100, 50});
    DrawTextureV(addFramePlusIcon, (Vector2) {addFrameButton.x + addFrameButton.width / 2 - (float) addFramePlusIcon.width / 2, addFrameButton.y + addFrameButton.height / 2 - (float) addFramePlusIcon.height / 2}, (Color) {100, 100, 100, 150});

    // canvas
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            DrawRectangleV(canvasPixels[currentFrame][x][y].position, (Vector2) {pixelSize, pixelSize}, canvasPixels[currentFrame][x][y].color);
        }
    }
}

void destoryEditor() {
    canvasPixels = NULL;
    selectedColorInputBox = NULL;

    UnloadTexture(addFramePlusIcon);
}
