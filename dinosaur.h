#ifndef DAINASO_DINOSAUR_H
#define DAINASO_DINOSAUR_H

#include <stdlib.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include "game_context.h"
#include "game_object.h"
#include "util.h"

typedef struct dsDinosaur {
    extends(dsGameObject)

    const sfTexture* hitTexture, * idleTexture, * duck1Texture, * duck2Texture, * step1Texture, * step2Texture;
    sfSound* jumpSound;
    float velocityY;
    bool jumping, ducking, __ignoreJump, __firstJumpIgnored;
} dsDinosaur;

void __dsDinosaur_behavior(dsDinosaur* _this, float deltaTime);

constructor(dsDinosaur)() {
    dsDinosaur* _this = malloc(sizeof(dsDinosaur));
    _this->type = dsDinosaurT;
    _this->hitTexture = dsAssetsBundle_get(g.assets, "texture.dainaso.hit");
    _this->idleTexture = dsAssetsBundle_get(g.assets, "texture.dainaso.idle");
    _this->duck1Texture = dsAssetsBundle_get(g.assets, "texture.dainaso.duck1");
    _this->duck2Texture = dsAssetsBundle_get(g.assets, "texture.dainaso.duck2");
    _this->step1Texture = dsAssetsBundle_get(g.assets, "texture.dainaso.step1");
    _this->step2Texture = dsAssetsBundle_get(g.assets, "texture.dainaso.step2");
    _this->jumpSound = dsAssetsBundle_get(g.assets, "sound.dainaso.jump");
    dsGameObject_superSprite((dsGameObject*) _this, _this->idleTexture,
                             ds_multiplyVector2UF(sfTexture_getSize(_this->idleTexture), 2));
    _this->behavior = (void (*)(dsGameObject*, float)) __dsDinosaur_behavior;
    _this->velocityY = 0;
    _this->jumping = false;
    _this->ducking = false;
    _this->__ignoreJump = false;
    _this->__firstJumpIgnored = false;
    sfRectangleShape_setPosition(_this->sprite, (sfVector2f) {g.config.dinosaur.posX, g.config.floor.posY});
    return _this;
}

void __dsDinosaur_behavior(dsDinosaur* _this, const float deltaTime) {

    if (not _this->jumping) {
        if (sfKeyboard_isKeyPressed(sfKeySpace) or sfKeyboard_isKeyPressed(sfKeyUp)) {
            if (not _this->__ignoreJump) {
                _this->jumping = true;
                _this->velocityY = -g.config.dinosaur.jumpStrength;
                sfSound_play(_this->jumpSound);
            } else _this->__firstJumpIgnored = true;
        } else {
            if (sfKeyboard_isKeyPressed(sfKeyDown)) _this->ducking = true;
            else _this->ducking = false;
            if (_this->__firstJumpIgnored) _this->__ignoreJump = false;
        }
    }

    const bool oddFrame = ((int) (sfTime_asSeconds(sfClock_getElapsedTime(g.clock))
                                  * g.config.dinosaur.animationSpeed)) % 2;
    if (_this->jumping)
        dsGameObject_setTexture((dsGameObject*) _this, _this->idleTexture);
    else if (_this->ducking)
        dsGameObject_setTexture((dsGameObject*) _this, oddFrame ? _this->duck1Texture : _this->duck2Texture);
    else
        dsGameObject_setTexture((dsGameObject*) _this, oddFrame ? _this->step1Texture : _this->step2Texture);

    _this->velocityY += deltaTime * g.config.gravity;
    sfRectangleShape_move(_this->sprite, (sfVector2f) {0, deltaTime * _this->velocityY});

    const sfVector2f position = sfRectangleShape_getPosition(_this->sprite);
    if (position.y > g.config.floor.posY) {
        sfRectangleShape_setPosition(_this->sprite, (sfVector2f) {position.x, g.config.floor.posY});
        _this->velocityY = 0;
        _this->jumping = false;
    }
}

contract_pure bool dsDinosaur_touching(const dsDinosaur* dinosaur, const dsGameObject* obstacle) {
    const sfRectangleShape* dinosaurSprite = dinosaur->sprite, * obstacleSprite = obstacle->sprite;
    const sfFloatRect dinosaurBounds = sfRectangleShape_getGlobalBounds(dinosaurSprite),
            obstacleBounds = sfRectangleShape_getGlobalBounds(obstacleSprite);

    sfFloatRect* intersection = malloc(sizeof(sfFloatRect));
    if (sfFloatRect_intersects(&dinosaurBounds, &obstacleBounds, intersection)) {

        sfImage* dinosaurTexture = sfTexture_copyToImage(sfRectangleShape_getTexture(dinosaurSprite)),
                * obstacleTexture = sfTexture_copyToImage(sfRectangleShape_getTexture(obstacleSprite));
        const sfTransform obstacleTransform = sfRectangleShape_getInverseTransform(obstacleSprite),
                dinosaurTransform = sfRectangleShape_getInverseTransform(dinosaurSprite);

        for (int y = 0; y < (int) intersection->height; y++) {
            for (int x = 0; x < (int) intersection->width; x++) {
                const sfVector2f point = {intersection->left + (float) x, intersection->top + (float) y};
                const sfVector2u
                        obstaclePoint = ds_convertVector2FU(sfTransform_transformPoint(&obstacleTransform, point)),
                        dinosaurPoint = ds_convertVector2FU(sfTransform_transformPoint(&dinosaurTransform, point));
                if (sfImage_getPixel(dinosaurTexture, dinosaurPoint.x / 2, dinosaurPoint.y / 2).a == 255
                    and sfImage_getPixel(obstacleTexture, obstaclePoint.x, obstaclePoint.y).a == 255)
                    return true;
            }
        }

        free(intersection);
        free(dinosaurTexture);
        free(obstacleTexture);
        return false;
    } else {
        free(intersection);
        return false;
    }
}

void dsDinosaur_hit(dsDinosaur* _this) {
    sfRectangleShape_setTexture(_this->sprite, _this->hitTexture, false);
}

#endif //DAINASO_DINOSAUR_H