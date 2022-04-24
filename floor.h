#ifndef DAINASO_FLOOR_H
#define DAINASO_FLOOR_H

#include <stdlib.h>
#include "game_object.h"
#include "game_context.h"

typedef struct dsFloor {
    extends(dsGameObject)
} dsFloor;

constructor(dsFloor)() {
    dsFloor* _this = malloc(sizeof(dsFloor));
    _this->type = dsFloorT;
    dsGameObject_superSolid((dsGameObject*) _this, sfColor_fromRGB(83, 83, 83),
                            (sfVector2f) {(float) g.config.window.size.x, 2});
    sfRectangleShape_setPosition(
            _this->sprite, (sfVector2f) {0, g.config.floor.posY + 0.875f * g.config.dinosaur.size});
    return _this;
}

#endif //DAINASO_FLOOR_H
