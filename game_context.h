#ifndef DAINASO_GAME_CONTEXT_H
#define DAINASO_GAME_CONTEXT_H

#include <stdint.h>
#include <SFML/Graphics.h>
#include "assets_bundle.h"
#include "macros.h"

typedef struct {
    struct {
        struct {
            const sfVector2u size;
        } window;
        struct {
            const float posX, size, animationSpeed, jumpStrength;
        } dinosaur;
        struct {
            const float animationSpeed;
        } pterodactyl;
        struct {
            const float posY;
            const sfColor color;
        } floor;
        struct {
            const sfColor color;
        } scene;
        const float gravity;
        const sfColor textColor;
    } config;
    dsAssetsBundle* assets;
    sfClock* clock;
    float speed;
} __dsGameContext;

__dsGameContext g = {
        .config = {
                .window.size = {.x = 768, .y = 256},
                .dinosaur = {.posX = 128, .size = 60, .animationSpeed = 10, .jumpStrength = 500 },
                .pterodactyl = { .animationSpeed = 2.5f },
                .floor = {.posY = 180, .color = {.r = 83, .g = 83, .b = 83, .a = 255}},
                .scene = {.color = {.r = 32, .g = 33, .b = 36, .a = 255}},
                .gravity = 1600,
                .textColor = {.r = 83, .g = 83, .b = 83, .a = 255}
        },
        .speed = 1,
        .clock = null
};

#endif //DAINASO_GAME_CONTEXT_H
