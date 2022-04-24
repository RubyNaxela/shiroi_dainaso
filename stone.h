#ifndef DAINASO_STONE_H
#define DAINASO_STONE_H

#include <stdlib.h>
#include <SFML/System.h>
#include "game_context.h"
#include "scrolling_object.h"
#include "util.h"

typedef struct dsStone {
    extends(dsScrollingObject)
} dsStone;

_static float __dsStone_speed();

constructor(dsStone)() {
    dsStone* _this = malloc(sizeof(dsStone));
    _this->scrollingType = dsStoneT;
    dsGameObject_superSolid((dsGameObject*) _this, g.config.floor.color,
                            (sfVector2f) {(float) ds_randomInt(2, 12), 2});
    dsScrollingObject_super((dsScrollingObject*) _this, __dsStone_speed);
    return _this;
}

_static float __dsStone_speed() {
    return g.speed * 300;
}

#endif //DAINASO_STONE_H
