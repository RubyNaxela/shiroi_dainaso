#ifndef DAINASO_CACTUS_H
#define DAINASO_CACTUS_H

#include <stdlib.h>
#include <SFML/System.h>
#include "game_context.h"
#include "scrolling_object.h"
#include "util.h"

///
/// \brief Represents the cactus obstacle. The cactus comes in one of two variants: small and large.
///
typedef struct dsCactus {
    extends(dsScrollingObject)
} dsCactus;

///
/// \return the speed at which a cactus is moving through the scene
///
_static float dsCactus_speed() {
    return g.speed * 300;
}

///
/// \brief Creates a new dsCactus of the specified variant.
/// \param large the cactus variant (true - large, false - small)
/// \return pointer to a new dsCactus instance
///
constructor(dsCactus)(bool large) {
    dsCactus* _this = malloc(sizeof(dsCactus));
    _this->scrollingType = dsCactusT;
    const sfTexture* texture = dsAssetsBundle_get(g.assets,
                                                  large ? "ds:texture.cactus.large" : "ds:texture.cactus.small");
    const sfVector2f size = ds_convertVector2UF(sfTexture_getSize(texture));
    dsGameObject_superSprite((dsGameObject*) _this, texture, size);
    dsScrollingObject_super((dsScrollingObject*) _this, dsCactus_speed);
    sfRectangleShape_setOrigin(_this->sprite, (sfVector2f) {size.x / 2, size.y});
    return _this;
}

#endif //DAINASO_CACTUS_H
