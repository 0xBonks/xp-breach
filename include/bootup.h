#ifndef BOOTUP_H
#define BOOTUP_H

#include "homescreen.h"
#include <raylib.h>

void InitBootUpScreen();
void DrawBootUpScreen(float screenWidth, float screenHeight, Font font);
void UnloadBootUpTextures();
void DrawSettings(float screenWidth, float screenHeight, Font font, Music backgroundMusic);
void UpdateSettings();
void safeSettings();

#endif