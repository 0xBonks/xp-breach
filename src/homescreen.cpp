#include "homescreen.h"
#include "raylib.h"
#include "icons.h"

static Texture2D layout_texture;
static Texture2D wallpaper_texture;

void InitHomeScreen(int screen_width, int screen_height) {
    LoadIconResources(screen_width, screen_height);

    Image layout = LoadImage(RESOURCES_PATH "images/layout_upscale.png");
    Image wallpaper = LoadImage(RESOURCES_PATH "images/base-background.png");

    ImageResize(&layout, screen_width, screen_height);
    ImageResize(&wallpaper, screen_width, screen_height);

    layout_texture = LoadTextureFromImage(layout);
    wallpaper_texture = LoadTextureFromImage(wallpaper);

    UnloadImage(layout);
    UnloadImage(wallpaper);
}

void DrawHomeScreen() {
    DrawTexture(wallpaper_texture, 0, 0, WHITE);
    DrawTexture(layout_texture, 0, 0, WHITE);
    DrawIconsToDesktop();
}

void UnloadHomeScreen() {
    UnloadTexture(layout_texture);
    UnloadTexture(wallpaper_texture);
    UnloadIconTextures();
}
