#include "game.h"

void updateUI(){
    const int padding = 10;
    const int buttonWidth = 100;
    const int buttonHeight = 30;

    Vector2 colorButtonPos = {padding + 10, padding + 10};
    DrawRectangleRec((Rectangle){colorButtonPos.x, colorButtonPos.y, buttonWidth, buttonHeight}, WHITE);
    DrawText("Snow Color", colorButtonPos.x + buttonWidth / 2 - MeasureText("Snow Color", 10) / 2, colorButtonPos.y + buttonHeight / 2 - 5, 10, BLACK);
}