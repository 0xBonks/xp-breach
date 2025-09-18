#include "bootup.h"
#include "raylib.h"
#include "homescreen.h"
#include <vector>
#include <iostream>
#include "config.h"
#include <string>

struct VolumeBox {
    Texture2D texture;
    Vector2 position;
};

int currentVolumeIndex = 4;

static std::vector<VolumeBox> volumeBoxes;
static Texture2D backgroundTexture;
static Rectangle returnButtonRect;
static Texture2D returnTextureUnpressed;
static Texture2D returnTexturePressed;
static Rectangle safeSettingsRect;
static Texture2D safeSettingsTextureUnpressed;
static Texture2D safeSettingsTexturePressed;
static Rectangle plusButton;
static Rectangle minusButton;

const char ready[256] = "READY";
const char start[256] = "F1 start";
const char settings[256] = "F2 settings";
const char volume[256] = "volume";

const float boxWidth = 32.0f;
const float boxHeight = 44.0f;

bool plusButtonWasPressed = false;
bool minusButtonWasPressed = false;
bool settingsSaved = false;
float buttonCooldown = 0.0f;

void InitBootUpScreen() {
    Config config = LoadSettings("settings.ini");

    try {
        if (config.find("Settings") != config.end() &&
            config["Settings"].find("volume") != config["Settings"].end() &&
            !config["Settings"]["volume"].empty()) {

            currentVolumeIndex = std::stoi(config["Settings"]["volume"]);
            currentVolumeIndex = std::max(0, std::min(8, currentVolumeIndex)); // Bounds check
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading volume setting: " << e.what() << std::endl;
        currentVolumeIndex = 4; // Fallback
    }

    volumeBoxes.clear();
    SetTextLineSpacing(16);

    Image backgroundImage = LoadImage(RESOURCES_PATH "volume_resources/off_volume.png");
    backgroundTexture = LoadTextureFromImage(backgroundImage);
    UnloadImage(backgroundImage);

    Image returnButtonPressed = LoadImage(RESOURCES_PATH "images/return_pressed.png");
    Image returnButtonUnpressed = LoadImage(RESOURCES_PATH "images/return_unpressed.png");
    Image safeSettingsPressed = LoadImage(RESOURCES_PATH "images/checked_box.png");
    Image safeSettingsUnpressed = LoadImage(RESOURCES_PATH "images/unchecked_box.png");

    returnTextureUnpressed = LoadTextureFromImage(returnButtonUnpressed);
    returnTexturePressed = LoadTextureFromImage(returnButtonPressed);
    safeSettingsTextureUnpressed = LoadTextureFromImage(safeSettingsUnpressed);
    safeSettingsTexturePressed = LoadTextureFromImage(safeSettingsPressed);

    UnloadImage(returnButtonPressed);
    UnloadImage(returnButtonUnpressed);
    UnloadImage(safeSettingsUnpressed);
    UnloadImage(safeSettingsPressed);

    Image volumeBoxImage = LoadImage(RESOURCES_PATH "volume_resources/volume_box.png");
    Texture2D volumeBoxTexture = LoadTextureFromImage(volumeBoxImage);
    UnloadImage(volumeBoxImage);

    float backgroundX = GetScreenWidth() / 2.0f - backgroundTexture.width / 2.0f;
    float backgroundY = GetScreenHeight() / 2.0f - backgroundTexture.height / 2.0f;

    for (int i = 0; i < 8; i++) {
        VolumeBox box;
        box.texture = volumeBoxTexture;
        box.position = { (backgroundX + 64) + i * boxWidth, backgroundY + 11};
        volumeBoxes.push_back(box);
    }

    plusButton = {
        backgroundX + 324,
        backgroundY + 12,
        48.0f,
        40.0f 
    };

    minusButton = {
        backgroundX + 12,
        backgroundY + 12,
        48.0f,
        40.0f
    };

    SetMasterVolume(currentVolumeIndex / 8.0f);
}

void DrawBootUpScreen(float screenWidth, float screenHeight, Font font) {
    DrawTextEx(font, ready, { screenWidth / 2 - 84, screenHeight / 2 - 64 }, 32.0f, 16.0f, WHITE);
    DrawTextEx(font, start, { 119.0f, 685.0f }, 32.0f, 16.0f, WHITE);
    DrawTextEx(font, settings, { 119.0f, 717.0f }, 32.0f, 16.0f, WHITE);
}

void DrawSettings(float screenWidth, float screenHeight, Font font, Music backgroundMusic) {
    DrawTextEx(font, volume, { screenWidth / 2 - 84, screenHeight / 2 - 64 }, 32.0f, 16.0f, WHITE);
    DrawTexture(backgroundTexture, screenWidth / 2 - backgroundTexture.width / 2, screenHeight / 2 - backgroundTexture.height / 2, WHITE);

    for (int i = 0; i < currentVolumeIndex && i < volumeBoxes.size(); i++) {
        DrawTexture(volumeBoxes[i].texture, volumeBoxes[i].position.x, volumeBoxes[i].position.y, WHITE);
    }

    if (settingsSaved && buttonCooldown > 0) {
        DrawTexture(safeSettingsTexturePressed, 895, 673, WHITE);
    }
    else {
        DrawTexture(safeSettingsTextureUnpressed, 895, 673, WHITE);
    }
    safeSettingsRect = { 895, 673, (float)safeSettingsTextureUnpressed.width, (float)safeSettingsTextureUnpressed.height };
}

void UpdateSettings() {
    static bool lastMouseState = false;

    if (buttonCooldown > 0) {
        buttonCooldown -= GetFrameTime();
        if (buttonCooldown <= 0) {
            settingsSaved = false;
        }
    }

    Vector2 mousePoint = GetMousePosition();
    bool isMouseOverPlus = CheckCollisionPointRec(mousePoint, plusButton);
    bool isMouseOverMinus = CheckCollisionPointRec(mousePoint, minusButton);
    bool isMouseOverSafeSettings = CheckCollisionPointRec(mousePoint, safeSettingsRect);
    bool currentMouseState = IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (isMouseOverPlus || isMouseOverMinus) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (currentMouseState && !lastMouseState) {
        if (isMouseOverPlus && currentVolumeIndex < 8) {
            currentVolumeIndex++;
            SetMasterVolume(currentVolumeIndex / 8.0f);
        }
        else if (isMouseOverMinus && currentVolumeIndex > 0) {
            currentVolumeIndex--;
            SetMasterVolume(currentVolumeIndex / 8.0f);
        }
        else if (isMouseOverSafeSettings) {
            safeSettings();
        }
    }
    lastMouseState = currentMouseState;
}

void safeSettings() {
    Config config = LoadSettings("settings.ini");
    SetSettings(config, "settings.ini");
    settingsSaved = true;
    buttonCooldown = 2.0f;
}

void UnloadBootUpTextures() {
    CloseAudioDevice();
    UnloadTexture(backgroundTexture);
    UnloadTexture(returnTextureUnpressed);
    UnloadTexture(returnTexturePressed);
    UnloadTexture(safeSettingsTextureUnpressed);
    UnloadTexture(safeSettingsTexturePressed);

    if (!volumeBoxes.empty()) {
        UnloadTexture(volumeBoxes[0].texture);
    }
    volumeBoxes.clear();
}