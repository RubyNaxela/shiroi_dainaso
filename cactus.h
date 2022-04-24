#ifndef DAINASO_CACTUS_H
#define DAINASO_CACTUS_H

#include <stdlib.h>
#include <SFML/System.h>
#include "game_context.h"
#include "scrolling_object.h"
#include "util.h"

typedef struct dsCactus {
    extends(dsScrollingObject)
} dsCactus;

_static float dsCactus_speed();

constructor(dsCactus)(bool large) {
    dsCactus* _this = malloc(sizeof(dsCactus));
    _this->scrollingType = dsCactusT;
    const sfTexture* texture = dsAssetsBundle_get(g.assets, large ? "texture.cactus.large" : "texture.cactus.small");
    const sfVector2f size = ds_convertVector2UF(sfTexture_getSize(texture));
    dsGameObject_superSprite((dsGameObject*) _this, texture, size);
    dsScrollingObject_super((dsScrollingObject*) _this, dsCactus_speed);
    sfRectangleShape_setOrigin(_this->sprite, (sfVector2f) {size.x / 2, size.y});
    return _this;
}

_static float dsCactus_speed() {
    return g.speed * 300;
}

#endif //DAINASO_CACTUS_H
