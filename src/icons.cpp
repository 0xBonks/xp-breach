#include "raylib.h"
#include <vector>
#include <iostream>

typedef enum {
    NONE,
    FILE_EXPLORER,
    BIN
} IconState;

struct Icon {
    Rectangle rect;
    IconState state;
    Texture2D texture;
};

static std::vector<Icon> icons;
static Rectangle fileExplorerBoundary;
static Rectangle openedFileExplorerRectangle;
static Texture2D openedFileExplorer;
static Vector2 mouseOffset = { 0, 0 };
static IconState current_icon_state = NONE;
static bool isDragging = false;

void LoadIconResources(float screenWidth, float screenHeight) {
    Image fileExplorerImage = LoadImage(RESOURCES_PATH "images/file_explorer_upscaled.png");
    Image fileExplorerOpened = LoadImage(RESOURCES_PATH "images/file_explorer_opened.png");
    Image binImage = LoadImage(RESOURCES_PATH "images/empty_bin.png");

    Texture2D fileExplorerTexture = LoadTextureFromImage(fileExplorerImage);
    openedFileExplorer = LoadTextureFromImage(fileExplorerOpened);
    Texture2D binTexture = LoadTextureFromImage(binImage);

    UnloadImage(fileExplorerImage);
    UnloadImage(fileExplorerOpened);
    UnloadImage(binImage);

    Icon fileExplorerIcon = {
        { 119, 119, (float)fileExplorerTexture.width, (float)fileExplorerTexture.height },
        FILE_EXPLORER,
        fileExplorerTexture
    };
    Icon binIcon = {
        { 200, 119, (float)binTexture.width, (float)binTexture.height },
        BIN,
        binTexture
    };

    icons.push_back(fileExplorerIcon);
    icons.push_back(binIcon);

    fileExplorerBoundary = { 98, 98, 903, 672 };
    openedFileExplorerRectangle = { 119, 119, (float)openedFileExplorer.width, (float)openedFileExplorer.height };
}

void DrawIconsToDesktop() {
    for (const Icon& icon : icons) {
        DrawTexture(icon.texture, (int)icon.rect.x, (int)icon.rect.y, WHITE);
    }

    if (current_icon_state == FILE_EXPLORER) {
        DrawTexture(openedFileExplorer, (int)openedFileExplorerRectangle.x, (int)openedFileExplorerRectangle.y, WHITE);
    }
}

void UnloadIconTextures() {
    for (Icon& icon : icons) {
        UnloadTexture(icon.texture);
    }
    UnloadTexture(openedFileExplorer);
}

void UpdateIcons() {
    Vector2 mouse_position = GetMousePosition();

    // Prüfe Kollision mit Icons
    for (const Icon& icon : icons) {
        if (CheckCollisionPointRec(mouse_position, icon.rect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                current_icon_state = icon.state;
            }
        }
        else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }

    if (current_icon_state == FILE_EXPLORER) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_position, openedFileExplorerRectangle)) {
            isDragging = true;
            mouseOffset.x = mouse_position.x - openedFileExplorerRectangle.x;
            mouseOffset.y = mouse_position.y - openedFileExplorerRectangle.y;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragging = false;
        }

        if (isDragging) {
            openedFileExplorerRectangle.x = mouse_position.x - mouseOffset.x;
            openedFileExplorerRectangle.y = mouse_position.y - mouseOffset.y;

            // Begrenze die Position
            if (openedFileExplorerRectangle.x < fileExplorerBoundary.x)
                openedFileExplorerRectangle.x = fileExplorerBoundary.x;
            if (openedFileExplorerRectangle.y < fileExplorerBoundary.y)
                openedFileExplorerRectangle.y = fileExplorerBoundary.y;
            if (openedFileExplorerRectangle.x + openedFileExplorerRectangle.width > fileExplorerBoundary.x + fileExplorerBoundary.width) {
                openedFileExplorerRectangle.x = fileExplorerBoundary.x + fileExplorerBoundary.width - openedFileExplorerRectangle.width;
            }
            if (openedFileExplorerRectangle.y + openedFileExplorerRectangle.height > fileExplorerBoundary.y + fileExplorerBoundary.height) {
                openedFileExplorerRectangle.y = fileExplorerBoundary.y + fileExplorerBoundary.height - openedFileExplorerRectangle.height;
            }
        }
    }
}