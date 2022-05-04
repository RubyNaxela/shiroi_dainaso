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

///
/// \brief Generates a pseudo-random int number from the range specified by lower and upper bounds.
/// \param start the lower bound (inclusive)
/// \param end the upper bound (inclusive)
/// \return a pseudo-random number from the specified range
///
int ds_randomInt(int start, int end) {
    ASSERT(end >= start, dsInvalidArgumentException)
    return start + rand() % (end - start + 1);
}

///
/// \brief Generates a pseudo-random bool value.
/// \return true or false, randomly
///
bool ds_randomBool() {
    return ds_randomInt(0, 1);
}

///
/// \brief Selects a pseudo-random from the specified values.
/// \param values an array of values
/// \param count the array size
/// \return one of the number in the specified array
///
int ds_randomFrom(int* values, int count) {
    return values[ds_randomInt(0, count - 1)];
}

///
/// \brief Generates a pseudo-random float number from the range specified by lower and upper bounds.
/// \param start the lower bound (inclusive)
/// \param end the upper bound (inclusive)
/// \return a pseudo-random number from the specified range
///
float ds_randomFloat(float start, float end) {
    ASSERT(end >= start, dsInvalidArgumentException)
    return (((float) rand()) / (float) INT_MAX) * (end - start) + start;
}

///
/// \brief Generates a pseudo-random bool value with the specified chance for true
/// \return true or false, randomly, with the specified chance for true
///
bool ds_percentChance(float percentage) {
    ASSERT(percentage >= 0 and percentage <= 1, dsInvalidArgumentException)
    return ds_randomFloat(0, 1) <= percentage;
}

///
/// \brief Creates an sfFont object from the specified pathname.
/// \param filename path to the font file
/// \return pointer to the sfFont object
///
contract_new sfFont* ds_createFont(const char* filename) {
    ASSERT_FILE_EXISTS(filename)
    return sfFont_createFromFile(filename);
}

///
/// \brief Creates an sfSound object from the specified pathname.
/// \param filename path to the audio file
/// \return pointer to the sfSound object
///
contract_new sfSound* ds_createSound(const char* filename) {
    ASSERT_FILE_EXISTS(filename)
    sfSound* sound = _new(sfSound)();
    const sfSoundBuffer* buffer = sfSoundBuffer_createFromFile(filename);
    sfSound_setBuffer(sound, buffer);
    return sound;
}

///
/// \brief Creates an sfTexture object from the specified pathname.
/// \param filename path to the image file
/// \return pointer to the sfTexture object
///
contract_new sfTexture* ds_createTexture(const char* filename) {
    ASSERT_FILE_EXISTS(filename)
    return sfTexture_createFromFile(filename, null);
}

///
/// \brief Adds two vectors represented by sfVector2f objects.
/// \param vector1 first vector
/// \param vector2 second vector
/// \return sum of the specified vectors
///
contract_pure sfVector2f ds_addVector2f(sfVector2f vector1, sfVector2f vector2) {
    return (sfVector2f) {vector1.x + vector2.x, vector1.y + vector2.y};
}

///
/// \brief Multiplies a vector represented by an sfVector2f object by a float value.
/// \param vector a vector
/// \param scalar a float value
/// \return product of the specified vector and value
///
contract_pure sfVector2f ds_multiplyVector2UF(sfVector2u vector, float scalar) {
    return (sfVector2f) {scalar * (float) vector.x, scalar * (float) vector.y};
}

///
/// \brief Converts a vector represented by an sfVector2u object to an sfVector2f object with corresponding values.
/// \param vector a vector represented by an sfVector2u object
/// \return an sfVector2f object
///
contract_pure sfVector2f ds_convertVector2UF(sfVector2u vector) {
    return (sfVector2f) {(float) vector.x, (float) vector.y};
}

///
/// \brief Converts a vector represented by an sfVector2f object to an sfVector2u object with corresponding values.
/// \param vector a vector represented by an sfVector2f object
/// \return an sfVector2u object
///
contract_pure sfVector2u ds_convertVector2FU(sfVector2f vector) {
    return (sfVector2u) {(uint32_t) vector.x, (uint32_t) vector.y};
}

///
/// \brief Reads and returns the hiscore stored in the "hiscore.txt" file, or 0 if the file does not exist.
/// \return the hiscore from the "hiscore.txt" file
///
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

///
/// \brief Stores the specified hiscore in the "hiscore.txt" file.
/// \param score the score to be stored
///
void ds_updateHiScore(int score) {
    if (score > ds_getHiScore()) {
        FILE* hiScoreFile = fopen("hiscore.txt", "w");
        fprintf(hiScoreFile, "%d", score);
        fclose(hiScoreFile);
    }
}

///
/// \brief Creates an array of two dsText objects which are be displayed as the game over screen.
/// \return array of two dsText pointers
///
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