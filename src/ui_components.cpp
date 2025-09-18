#include "ui_components.h"
#include "raylib.h"
#include "loading.h"

static Texture2D turnOffTexture;
static Texture2D turnOnTexture;
static Texture2D loadingTexture;
static Rectangle powerButtonRect;
static Font font;

void InitUIComponents() {
    Image turnOffButton = LoadImage(RESOURCES_PATH "images/turn_off_button.png");
    Image turnOnButton = LoadImage(RESOURCES_PATH "images/turn_on_button.png");
    Image loadingButton = LoadImage(RESOURCES_PATH "images/idle_button.png");

    turnOffTexture = LoadTextureFromImage(turnOffButton);
    turnOnTexture = LoadTextureFromImage(turnOnButton);
    loadingTexture = LoadTextureFromImage(loadingButton);

    UnloadImage(turnOffButton);
    UnloadImage(turnOnButton);
    UnloadImage(loadingButton);

    powerButtonRect = { 44, 860, (float)turnOffTexture.width, (float)turnOffTexture.height };
}

PcState UpdatePowerButton(PcState currentState) {
    Vector2 mousePosition = GetMousePosition();
    PcState newState = currentState;

    if (CheckCollisionPointRec(mousePosition, powerButtonRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (currentState == STATE_OFF) {
                newState = STATE_BOOTING;
                ResetLoadingAnimation();
            }
            else {
                newState = STATE_OFF;
            }
        }
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    return newState;
}

void DrawPowerButton(PcState currentState) {
    switch (currentState) {
    case STATE_OFF:
        DrawTexture(turnOffTexture, (int)powerButtonRect.x, (int)powerButtonRect.y, WHITE);
        break;
    case STATE_ON:
        DrawTexture(turnOnTexture, (int)powerButtonRect.x, (int)powerButtonRect.y, WHITE);
        break;
    case STATE_BOOTING:
    case STATE_BOOT_SELECT:
    case STATE_SETTINGS:
        DrawTexture(loadingTexture, (int)powerButtonRect.x, (int)powerButtonRect.y, WHITE);
        break;
    }
}

void UnloadUIComponents() {
    UnloadTexture(turnOffTexture);
    UnloadTexture(turnOnTexture);
    UnloadTexture(loadingTexture);
    UnloadFont(font);
}