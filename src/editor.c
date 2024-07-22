#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>

#include "editor.h"
#include "pixel.h"

Rectangle framesBackgroundPanel;
Rectangle canvasBackgroundPanel;
Rectangle colorPickerBackgroundPanel;

float canvasBackgroundRoundness;
float colorPickerBackgroundRoundness; // this one is also used for the frames background

// maybe have this calculated based on screen size but i dont
// give that much of a fuck about it so fuck it thats how it is
float backgroundPanelPadding = 10; // in pixels

float canvasSize;

// *temp* change to double pointer later :)
Pixel canvasPixels[16][16];

// calculated in the resizeEditor function and is based on size of canvas
float pixelSize;

Rectangle colorPickerPreview;

// current color
Color currentColor;

void resizeEditor() {
    framesBackgroundPanel.width = ((float) (GetScreenWidth() - GetScreenHeight()) / 2) - backgroundPanelPadding;
    framesBackgroundPanel.height = GetScreenHeight() - backgroundPanelPadding * 2;
    framesBackgroundPanel.x = backgroundPanelPadding;
    framesBackgroundPanel.y = (float) GetScreenHeight() / 2 - framesBackgroundPanel.height / 2;

    canvasBackgroundPanel.width = GetScreenHeight() - backgroundPanelPadding * 2;
    canvasBackgroundPanel.height = GetScreenHeight() - backgroundPanelPadding * 2;
    canvasBackgroundPanel.x = (float) GetScreenWidth() / 2 - canvasBackgroundPanel.width / 2;
    canvasBackgroundPanel.y = (float) GetScreenHeight() / 2 - canvasBackgroundPanel.height / 2;

    colorPickerBackgroundPanel.width = ((float) (GetScreenWidth() - GetScreenHeight()) / 2) - backgroundPanelPadding;
    colorPickerBackgroundPanel.height = GetScreenHeight() - backgroundPanelPadding * 2;
    colorPickerBackgroundPanel.x = (float) GetScreenWidth() - colorPickerBackgroundPanel.width - backgroundPanelPadding;
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
    pixelSize = (GetScreenHeight() - backgroundPanelPadding * 6) / canvasSize;

    // god came down and told me how to do this
    float canvasPixelsXStartingPos = (float) GetScreenWidth() / 2 - (GetScreenHeight() - backgroundPanelPadding * 6) / 2;
    float canvasPixelsYStartingPos = (float) GetScreenHeight() / 2 - (GetScreenHeight() - backgroundPanelPadding * 6) / 2;

    // all the code under this prolly has to be refactored later :(
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            canvasPixels[x][y].position.x = canvasPixelsXStartingPos + pixelSize * x;
            canvasPixels[x][y].position.y = canvasPixelsYStartingPos + pixelSize * y;
        }
    }

    // color picker
    colorPickerPreview.width = colorPickerBackgroundPanel.width - backgroundPanelPadding * 4;
    colorPickerPreview.height = colorPickerPreview.width;
    colorPickerPreview.x = colorPickerBackgroundPanel.x + backgroundPanelPadding * 2;
    colorPickerPreview.y = colorPickerBackgroundPanel.y + backgroundPanelPadding * 2;
}

void createEditor(int size) {
    canvasSize = size;

    resizeEditor();

    // create checkerboard pattern

    // false = white | true = gray
    bool color = false;
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            if (color) {
                canvasPixels[x][y].color = LIGHTGRAY;
            } else {
                canvasPixels[x][y].color = WHITE;
            }

            color = !color;
        }
        color = !color;
    }

    // temp
    currentColor = BLUE;
}


void updateEditor() {
    Rectangle pixel;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        for (int x = 0; x < canvasSize; x++) {
            for (int y = 0; y < canvasSize; y++) {
                pixel.x = canvasPixels[x][y].position.x;
                pixel.y = canvasPixels[x][y].position.y;
                pixel.width = pixelSize;
                pixel.height = pixelSize;

                if (CheckCollisionPointRec(GetMousePosition(), pixel)) {
                    canvasPixels[x][y].color = currentColor;
                }
            }
        }
    }
}

void renderEditor() {
    DrawRectangleRounded(framesBackgroundPanel, colorPickerBackgroundRoundness, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(canvasBackgroundPanel, canvasBackgroundRoundness, 10, (Color) {100, 100, 100, 50});
    DrawRectangleRounded(colorPickerBackgroundPanel, colorPickerBackgroundRoundness, 10, (Color) {100, 100, 100, 50});

    DrawRectangleRounded(colorPickerPreview, 0.1f, 10, currentColor);

    // canvas
    for (int x = 0; x < canvasSize; x++) {
        for (int y = 0; y < canvasSize; y++) {
            DrawRectangleV(canvasPixels[x][y].position, (Vector2) {pixelSize, pixelSize}, canvasPixels[x][y].color);
        }
    }
}

void destoryEditor() {

}
