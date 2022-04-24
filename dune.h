#ifndef DAINASO_DUNE_H
#define DAINASO_DUNE_H

#include <stdlib.h>
#include <SFML/System.h>
#include "game_context.h"
#include "scrolling_object.h"
#include "util.h"

typedef struct dsDune {
    extends(dsScrollingObject)
} dsDune;

_static float __dsDune_speed();

constructor(dsDune)() {
    dsDune* _this = malloc(sizeof(dsDune));
    _this->scrollingType = dsDuneT;
    const sfTexture* texture = dsAssetsBundle_get(g.assets, "ds:texture.dune");
    const sfVector2u size = sfTexture_getSize(texture);
    dsGameObject_superSprite((dsGameObject*) _this, texture, ds_multiplyVector2UF(size, 2));
    dsScrollingObject_super((dsScrollingObject*) _this, __dsDune_speed);
    const bool flipX = ds_randomBool(), flipY = ds_randomBool();
    sfRectangleShape_setScale(_this->sprite, (sfVector2f) {flipX ? -1 : 1, flipY ? -1 : 1});
    sfRectangleShape_setOrigin(_this->sprite,
                               (sfVector2f) {flipX ? (float) size.x : 0, (float) (flipY ? 2 * size.y + 3 : size.y)});
    return _this;
}

_static float __dsDune_speed() {
    return g.speed * 300;
}

#endif //DAINASO_DUNE_H
