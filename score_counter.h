#ifndef DAINASO_SCORE_COUNTER_H
#define DAINASO_SCORE_COUNTER_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include "assets_bundle.h"
#include "game_context.h"
#include "text.h"

typedef struct dsScoreCounter {
    extends(dsText)

    sfSound* milestoneSound;
    int score;
    bool running;
} dsScoreCounter;

void __dsScoreCounter_behavior(dsScoreCounter* _this);

void __dsScoreCounter_draw(const dsScoreCounter* _this, sfRenderWindow* window);

constructor(dsScoreCounter)() {
    dsScoreCounter* _this = malloc(sizeof(dsScoreCounter));
    dsText* super = (dsText*) _this;
    dsText_super(super);
    dsGameObject_superSolid((dsGameObject*) _this, g.config.scene.color, (sfVector2f) {60, 16});
    const sfVector2f position = {(float) g.config.window.size.x - 16, 16};
    dsText_setPosition(super, position);
    dsText_setColor(super, g.config.floor.color);
    dsText_setFontSize(super, 24);
    dsText_setHorizontalAlignment(super, dsEnd);
    _this->behavior = (void (*)(dsGameObject*, float)) __dsScoreCounter_behavior;
    _this->draw = (void (*)(const dsGameObject*, sfRenderWindow*)) __dsScoreCounter_draw;
    _this->score = 0;
    _this->milestoneSound = dsAssetsBundle_get(g.assets, "sound.milestone");
    _this->running = true;
    sfRectangleShape_setOrigin(_this->sprite, (sfVector2f) {58, 0});
    sfRectangleShape_setPosition(_this->sprite, position);
    sfRectangleShape_setFillColor(_this->sprite, g.config.scene.color);
    return _this;
}

void __dsScoreCounter_behavior(dsScoreCounter* _this) {
    _this->score = (int) (sfTime_asSeconds(sfClock_getElapsedTime(g.clock)) * 10);
    char scoreText[20];
    if (_this->score > 0 and not(_this->score % 100) and sfSound_getStatus(_this->milestoneSound) != sfPlaying)
        sfSound_play(_this->milestoneSound);
    sprintf(scoreText, "HI   %05d   %05d", ds_getHiScore(), _this->score);
    dsText_setText((dsText*) _this, scoreText);
}

void __dsScoreCounter_draw(const dsScoreCounter* _this, sfRenderWindow* window) {
    sfRenderWindow_drawText(window, _this->__text, null);
    const float time = sfTime_asSeconds(sfClock_getElapsedTime(g.clock));
    const int scoreAboveHundredMultiple = (int) ((time / 10 - floorf(time / 10)) * 100);
    if (time > 1 and scoreAboveHundredMultiple > 0 and scoreAboveHundredMultiple <= 5 and _this->running)
        sfRenderWindow_drawRectangleShape(window, _this->sprite, null);
}

int dsScoreCounter_stop(dsScoreCounter* _this) {
    _this->running = false;
    return _this->score;
}

#endif //DAINASO_SCORE_COUNTER_H
