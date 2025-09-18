#include "loading.h"
#include "raylib.h"

static Image loading_gif_image;
static Texture2D loading_gif_texture;

static int anim_frame_count = 0;
static int current_anim_frame = 0;
static int frame_delay = 8;
static int frame_counter = 0;
static bool animationFinished = false;

void InitLoadingAnimation() {
    const char* gif_path = RESOURCES_PATH "images/loading_bar_scaled_4x.gif";
    loading_gif_image = LoadImageAnim(gif_path, &anim_frame_count);

    if (loading_gif_image.data != NULL && anim_frame_count > 0) {
        loading_gif_texture = LoadTextureFromImage(loading_gif_image);
    }
}

void UpdateLoadingAnimation() {
    if (anim_frame_count <= 0) return;

    frame_counter++;
    if (frame_counter >= frame_delay) {
        current_anim_frame++;
        if (current_anim_frame >= anim_frame_count) {
            current_anim_frame = 0;
            animationFinished = true;
        }

        unsigned int next_frame_offset = loading_gif_image.width * loading_gif_image.height * 4 * current_anim_frame;
        UpdateTexture(loading_gif_texture, (unsigned char*)loading_gif_image.data + next_frame_offset);

        frame_counter = 0;
    }
}

void DrawLoadingAnimation() {
    if (loading_gif_texture.id > 0) {
        DrawTexture(
            loading_gif_texture,
            GetScreenWidth() / 2 - loading_gif_texture.width / 2,
            GetScreenHeight() / 2 - loading_gif_texture.height / 2,
            WHITE
        );
    }
}

void ReInitLoadingAnimation() {
    UnloadTexture(loading_gif_texture);
    UnloadImage(loading_gif_image);
}

void ResetLoadingAnimation() {
    current_anim_frame = 0;
    frame_counter = 0;
    animationFinished = false;
}

bool isLoadingAnimationFinished() {
    return animationFinished;
}