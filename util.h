#ifndef DAINASO_UTIL_H
#define DAINASO_UTIL_H
#pragma ide diagnostic ignored "cert-msc30-c"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include "macros.h"
#include "text.h"

int ds_randomInt(int start, int end) {
    ASSERT(end >= start, dsInvalidArgumentException)
    return start + rand() % (end - start + 1);
}

bool ds_randomBool() {
    return ds_randomInt(0, 1);
}

float ds_randomFloat(float start, float end) {
    ASSERT(end >= start, dsInvalidArgumentException)
    return (((float) rand()) / (float) INT_MAX) * (end - start) + start;
}

bool ds_percentChance(float percentage) {
    ASSERT(percentage >= 0 and percentage <= 1, dsInvalidArgumentException)
    return ds_randomFloat(0, 1) <= percentage;
}

contract_new sfFont* ds_createFont(const char* filename) {
    ASSERT_FILE_EXISTS(filename)
    return sfFont_createFromFile(filename);
}

contract_new sfSound* ds_createSound(const char* filename) {
    ASSERT_FILE_EXISTS(filename)
    sfSound* sound = _new(sfSound)();
    const sfSoundBuffer* buffer = sfSoundBuffer_createFromFile(filename);
    sfSound_setBuffer(sound, buffer);
    return sound;
}

contract_new sfTexture* ds_createTexture(const char* filename) {
    ASSERT_FILE_EXISTS(filename)
    return sfTexture_createFromFile(filename, null);
}

contract_pure sfVector2f ds_addVector2f(sfVector2f vector1, sfVector2f vector2) {
    return (sfVector2f) {vector1.x + vector2.x, vector1.y + vector2.y};
}

contract_pure sfVector2f ds_multiplyVector2UF(sfVector2u vector, float scalar) {
    return (sfVector2f) {scalar * (float) vector.x, scalar * (float) vector.y};
}

contract_pure sfVector2f ds_convertVector2UF(sfVector2u vector) {
    return (sfVector2f) {(float) vector.x, (float) vector.y};
}

contract_pure sfVector2u ds_convertVector2FU(sfVector2f vector) {
    return (sfVector2u) {(uint32_t) vector.x, (uint32_t) vector.y};
}

int ds_getHiScore() {
    int score;
    FILE* hiScoreFile = fopen("hiscore.txt", "r");
    if (hiScoreFile == null) {
        hiScoreFile = fopen("hiscore.txt", "w");
        fprintf(hiScoreFile, "%d", 0);
        score = 0;
    } else {
        char scoreStr[6];
        fscanf(hiScoreFile, "%s", scoreStr);
        score = (int) strtol(scoreStr, null, 10);
    }
    fclose(hiScoreFile);
    return score;
}

void ds_updateHiScore(int score) {
    if (score > ds_getHiScore()) {
        FILE* hiScoreFile = fopen("hiscore.txt", "w");
        fprintf(hiScoreFile, "%d", score);
        fclose(hiScoreFile);
    }
}

contract_new dsText** ds_createGameOverText() {
    dsText** texts = calloc(2, sizeof(dsText*));
    dsText* text1 = _new(dsText)(), * text2 = _new(dsText)();

    dsText_setFontSize(text1, 32);
    dsText_setOutline(text1, (sfColor) {255, 255, 255, 255}, 1);
    dsText_setHorizontalAlignment(text1, dsCenter);
    dsText_setVerticalAlignment(text1, dsCenter);
    dsText_setText(text1, "GAME OVER");
    dsText_setPosition(text1, ds_multiplyVector2UF(g.config.window.size, 0.5f));

    dsText_setFontSize(text2, 22);
    dsText_setOutline(text2, (sfColor) {255, 255, 255, 255}, 1);
    dsText_setHorizontalAlignment(text2, dsCenter);
    dsText_setVerticalAlignment(text2, dsEnd);
    dsText_setText(text2, "PRESS SPACE TO RESTART");
    dsText_setPosition(text2, ds_addVector2f(ds_multiplyVector2UF(g.config.window.size, 0.5f),
                                             (sfVector2f) {0, 32}));
    texts[0] = text1;
    texts[1] = text2;
    return texts;
}

#endif //DAINASO_UTIL_H