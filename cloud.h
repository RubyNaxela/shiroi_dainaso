#ifndef DAINASO_CLOUD_H
#define DAINASO_CLOUD_H

#include <stdlib.h>
#include <SFML/System.h>
#include "game_context.h"
#include "macros.h"
#include "scrolling_object.h"
#include "util.h"

typedef struct dsCloud {
    extends(dsScrollingObject)
} dsCloud;

_static float __dsCloud_speed();

constructor(dsCloud)() {
    dsCloud* _this = malloc(sizeof(dsCloud));
    _this->scrollingType = dsCloudT;
    const sfTexture* texture = dsAssetsBundle_get(g.assets, "ds:texture.cloud");
    dsGameObject_superSprite((dsGameObject*) _this, texture, ds_convertVector2UF(sfTexture_getSize(texture)));
    dsScrollingObject_super((dsScrollingObject*) _this, __dsCloud_speed);
    return _this;
}

_static float __dsCloud_speed() {
    return g.speed * 75;
}

#endif //DAINASO_CLOUD_H
