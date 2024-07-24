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
float padding; // change this name!!

int numberOfFrames;
int canvasSize;

int currentFrame;
Pixel ***canvasPixels; // never thought i see the day where triple pointers
                       // are used for something that isn't retarded! :O

// calculated in the resizeEditor function and is based on size of canvas
float pixelSize;

Rectangle colorPickerPreview;

// current color
Color currentColor = WHITE;

ColorInputBox redInputBox;
ColorInputBox greenInputBox;
ColorInputBox blueInputBox;

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

    // god came down and told me how to do this
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

    // color input boxes (NOT a fan of how i layed this out but its fine)
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

    // add frame button
    addFrameButton.width = framesBackgroundPanel.width - padding * 4;
    addFrameButton.height = framesBackgroundPanel.height / 35; // magic bullshit
    addFrameButton.x = framesBackgroundPanel.x + padding * 2;
    addFrameButton.y = framesBackgroundPanel.y + framesBackgroundPanel.height - padding * 2 - addFrameButton.height;

    // add frame plus icon
    addFramePlusIcon.width = addFrameButton.height - addFrameButton.height / 5; // magic bullshit
    addFramePlusIcon.height = addFramePlusIcon.width;
}

void createFrame() {
    numberOfFrames++;
    canvasPixels = realloc(canvasPixels, numberOfFrames * sizeof(Pixel**));

    canvasPixels[numberOfFrames - 1] = malloc(canvasSize * sizeof(Pixel*));
    for (int x = 0; x < canvasSize; x++) {
        canvasPixels[numberOfFrames - 1][x] = malloc(canvasSize * sizeof(Pixel));
    }

    // create checkerboard pattern

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

    // color input boxes
    redInputBox.text[0] = '2';
    redInputBox.text[1] = '5';
    redInputBox.text[2] = '5';
    redInputBox.text[3] = '\0';
    redInputBox.numberCount = 3;

    greenInputBox.text[0] = '2';
    greenInputBox.text[1] = '5';
    greenInputBox.text[2] = '5';
    greenInputBox.text[3] = '\0';
    greenInputBox.numberCount = 3;

    blueInputBox.text[0] = '2';
    blueInputBox.text[1] = '5';
    blueInputBox.text[2] = '5';
    blueInputBox.text[3] = '\0';
    blueInputBox.numberCount = 3;

    // textures
    addFramePlusIcon = LoadTexture("assets/plus.png");

    createFrame();
    resizeEditor();
}

void updateEditor() {
    if (redInputBox.isActive || greenInputBox.isActive || blueInputBox.isActive) {
        ColorInputBox *inputBox;

        if (redInputBox.isActive) {
            inputBox = &redInputBox;
        }
        if (greenInputBox.isActive) {
            inputBox = &greenInputBox;
        }
        if (blueInputBox.isActive) {
            inputBox = &blueInputBox;
        }

        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();
        while (key > 0) {
            // only allow numbers
            if ((key >= 48) && (key <= 57)) {
                if (inputBox->isActive && inputBox->numberCount <= 3) {
                    inputBox->text[inputBox->numberCount] = (char) key;
                    inputBox->text[inputBox->numberCount + 1] = '\0';
                    inputBox->numberCount++;
                }
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (inputBox->isActive) {
                inputBox->numberCount--;
                if (inputBox->numberCount < 0) {
                    inputBox->numberCount = 0;
                }
                inputBox->text[inputBox->numberCount] = '\0';
            }
        }
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

    // frames panel & color picker panel
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        redInputBox.isActive = false;
        greenInputBox.isActive = false;
        blueInputBox.isActive = false;

        if (CheckCollisionPointRec(GetMousePosition(), framesBackgroundPanel)) {
            if (CheckCollisionPointRec(GetMousePosition(), addFrameButton)) {
                createFrame();
                printf("number of frames: %d\n", numberOfFrames);
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), colorPickerBackgroundPanel)) {
            if (CheckCollisionPointRec(GetMousePosition(), redInputBox.box)) {
                redInputBox.isActive = true;
            }
            if (CheckCollisionPointRec(GetMousePosition(), greenInputBox.box)) {
                greenInputBox.isActive = true;
            }
            if (CheckCollisionPointRec(GetMousePosition(), blueInputBox.box)) {
                blueInputBox.isActive = true;
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
    DrawRectangleRounded(framesBackgroundPanel, colorPickerBackgroundRoundness, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(canvasBackgroundPanel, canvasBackgroundRoundness, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(colorPickerBackgroundPanel, colorPickerBackgroundRoundness, 10, (Color) {100, 100, 100, 50});

    // magic number (fix later!)
    DrawRectangleRounded(colorPickerPreview, 0.1f, 10, currentColor);

    DrawRectangleRounded(redInputBox.box, 1, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(greenInputBox.box, 1, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(blueInputBox.box, 1, 10, (Color) {100, 100, 100, 50});

    if (redInputBox.isActive) {
        DrawRectangleRoundedLinesEx(redInputBox.box, 1, 10, 2, (Color) {100, 100, 100, 150});
    }
    if (greenInputBox.isActive) {
        DrawRectangleRoundedLinesEx(greenInputBox.box, 1, 10, 2, (Color) {100, 100, 100, 150});
    }
    if (blueInputBox.isActive) {
        DrawRectangleRoundedLinesEx(blueInputBox.box, 1, 10, 2, (Color) {100, 100, 100, 150});
    }

    // temp until i find a nice font
    DrawText(redInputBox.text, (redInputBox.box.x + redInputBox.box.width / 2) - ((float) MeasureText(redInputBox.text, 25) / 2), redInputBox.box.y + 3, 25, (Color) {100, 100, 100, 150});

    DrawText(greenInputBox.text, (greenInputBox.box.x + greenInputBox.box.width / 2) - ((float) MeasureText(greenInputBox.text, 25) / 2), greenInputBox.box.y + 3, 25, (Color) {100, 100, 100, 150});

    DrawText(blueInputBox.text, (blueInputBox.box.x + blueInputBox.box.width / 2) - ((float) MeasureText(blueInputBox.text, 25) / 2), blueInputBox.box.y + 3, 25, (Color) {100, 100, 100, 150});

    // add frame button
    DrawRectangleRounded(addFrameButton, 1, 10, (Color) {100, 100, 100, 50});
    DrawTextureV(addFramePlusIcon, (Vector2) {addFrameButton.x + addFrameButton.width / 2 - (float) addFramePlusIcon.width / 2, addFrameButton.y + addFrameButton.height / 2 - (float) addFramePlusIcon.height / 2}, (Color) {100, 100, 100, 150});

    // canvas
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            // why the fuck is the normal raylib draw rectangle not using floats for coords
            DrawRectangleV(canvasPixels[currentFrame][x][y].position, (Vector2) {pixelSize, pixelSize}, canvasPixels[currentFrame][x][y].color);
        }
    }
}

void destoryEditor() {
    UnloadTexture(addFramePlusIcon);
}
