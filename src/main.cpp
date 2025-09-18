#include "raylib.h"
#include "statemanager.h"
#include "homescreen.h"
#include "bootup.h"
#include "loading.h"
#include "icons.h"
#include "ui_components.h"

void LoadGameResources(int screenWidth, int screenHeight);
void UnloadGameResources();

static Texture2D layout_texture;
Font font;
Music backgroundMusic;
const int screen_width = 1099;
const int screen_height = 980;
const int FPS = 60;

int main() {
    InitWindow(screen_width, screen_height, "XP Breach");
    SetTargetFPS(FPS);

    InitAudioDevice();
    font = LoadFontEx(RESOURCES_PATH "BoldPixels.ttf", 32, 0, 250);
    backgroundMusic = LoadMusicStream(RESOURCES_PATH "volume_resources/background_audio.ogg");

    SetMusicVolume(backgroundMusic, 1.0f);
    PlayMusicStream(backgroundMusic);
    LoadGameResources(screen_width, screen_height);

    PcState current_pc_state = STATE_OFF;

    while (!WindowShouldClose()) {
        PcState previous_state = current_pc_state;
        current_pc_state = UpdatePowerButton(current_pc_state);
        UpdateMusicStream(backgroundMusic);
        UpdateSettings();

        if (previous_state != STATE_OFF && current_pc_state == STATE_OFF) {
        } else {
            switch (current_pc_state) {
                case STATE_BOOTING: {
                    UpdateLoadingAnimation();
                    if (isLoadingAnimationFinished()) {
                        current_pc_state = STATE_BOOT_SELECT;
                    }
                    break;
                }
                case STATE_BOOT_SELECT: {
                    if (IsKeyPressed(KEY_F1)) {
                        current_pc_state = STATE_ON;
                    } else if (IsKeyPressed(KEY_F2)) {
                        current_pc_state = STATE_SETTINGS;
                    }
                    break;
                }
                case STATE_ON: {
                    UpdateIcons();
                    break;
                }
                case STATE_SETTINGS: {
                    UpdateSettings();
                    break;
                }
                default: break;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (current_pc_state == STATE_ON) {
            DrawHomeScreen();
        } else {
            DrawTexture(layout_texture, 0, 0, WHITE);
        }

        switch (current_pc_state) {
            case STATE_BOOTING: {
                DrawLoadingAnimation();
                break;
            }
            case STATE_BOOT_SELECT: {
                DrawBootUpScreen((float)screen_width, (float)screen_height, font);
                break;
            }
            case STATE_SETTINGS: {
                DrawSettings((float)screen_width, (float)screen_height, font, backgroundMusic);
                break;
            }
            default: break;
        }

        DrawPowerButton(current_pc_state);

        EndDrawing();
    }

    UnloadGameResources();
    CloseWindow();

    return 0;
}

void LoadGameResources(int screenWidth, int screenHeight) {
    Image layout = LoadImage(RESOURCES_PATH "images/layout_upscale.png");
    ImageResize(&layout, screenWidth, screenHeight);
    layout_texture = LoadTextureFromImage(layout);
    UnloadImage(layout);

    InitHomeScreen(screenWidth, screenHeight);
    InitBootUpScreen();
    InitLoadingAnimation();
    InitUIComponents();
}

void UnloadGameResources() {
    UnloadTexture(layout_texture);
    UnloadHomeScreen();
    UnloadBootUpTextures();
    ReInitLoadingAnimation();
    UnloadUIComponents();
}
