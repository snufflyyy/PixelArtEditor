#pragma once

typedef enum {
    MAINMENU,
    EDITOR
} ApplicationState;

typedef enum {
    DARK,
    LIGHT
} ApplicationTheme;

void createApplication();

void updateApplication();
void renderApplication();

void destroyApplication();
