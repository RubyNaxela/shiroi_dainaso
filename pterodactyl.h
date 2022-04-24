#ifndef DAINASO_PTERODACTYL_H
#define DAINASO_PTERODACTYL_H

#include "scrolling_object.h"

typedef struct dsPterodactyl {
    extends(dsScrollingObject)

    const sfTexture* flap1Texture, * flap2Texture;
} dsPterodactyl;

_static float dsPterodactyl_speed();

void __dsPterodactyl_behavior(dsPterodactyl* _this, float deltaTime);

constructor(dsPterodactyl)() {
    dsPterodactyl* _this = malloc(sizeof(dsPterodactyl));
    _this->scrollingType = dsCactusT;
    _this->flap1Texture = dsAssetsBundle_get(g.assets, "ds:texture.pterodactyl.flap1");
    _this->flap2Texture = dsAssetsBundle_get(g.assets, "ds:texture.pterodactyl.flap2");
    const sfVector2u size = sfTexture_getSize(_this->flap1Texture);
    dsGameObject_superSprite((dsGameObject*) _this, _this->flap1Texture, ds_convertVector2UF(size));
    dsScrollingObject_super((dsScrollingObject*) _this, dsPterodactyl_speed);
    _this->behavior = (void (*)(dsGameObject*, float)) __dsPterodactyl_behavior;
    sfRectangleShape_setOrigin(_this->sprite, ds_multiplyVector2UF(size, 0.5f));
    return _this;
}

void __dsPterodactyl_behavior(dsPterodactyl* _this, const float deltaTime) {
    __dsScrollingObject_behavior((dsScrollingObject*) _this, deltaTime);
    const bool oddFrame = ((int) (sfTime_asSeconds(sfClock_getElapsedTime(g.clock))
                                  * g.config.pterodactyl.animationSpeed)) % 2;
    dsGameObject_setTexture((dsGameObject*) _this, oddFrame ? _this->flap1Texture : _this->flap2Texture);
}

_static float dsPterodactyl_speed() {
    return g.speed * 450;
}

#endif //DAINASO_PTERODACTYL_H
