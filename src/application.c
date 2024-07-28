#include <raylib.h>

#include "application.h"
#include "editor.h"
#include "mainmenu.h"

ApplicationState applicationState;

void resizeApplication() {
    switch (applicationState) {
        case MAINMENU:
            resizeMainMenu();
            break;
        case EDITOR:
            resizeEditor();
            break;
    }
}

// would like to have a load project type thing
void createApplication() {
    applicationState = EDITOR;

    switch (applicationState) {
        case MAINMENU:
            createMainMenu();
            break;
        case EDITOR:
            createEditor(32); // debug
            break;
    }
}

void updateApplication() {
    if (IsWindowResized()) {
        resizeApplication();
    }

    switch (applicationState) {
        case MAINMENU:
            updateMainMenu();
            break;
        case EDITOR:
            updateEditor();
            break;
    }
}

void renderApplication() {
    ClearBackground((Color) {25, 25, 25, 255});

    switch (applicationState) {
        case MAINMENU:
            renderMainMenu();
            break;
        case EDITOR:
            renderEditor();
            break;
    }
}

void destroyApplication() {
    switch (applicationState) {
        case MAINMENU:
            destroyMainMenu();
            break;
        case EDITOR:
            destoryEditor();
            break;
    }
}
