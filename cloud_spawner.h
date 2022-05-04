#ifndef DAINASO_CLOUD_SPAWNER_H
#define DAINASO_CLOUD_SPAWNER_H

#include <stdio.h>
#include "cloud.h"
#include "macros.h"
#include "scene_decl.h"
#include "spawner.h"

///
/// \brief An invisible object that spawn clouds periodically.
///
typedef struct dsCloudSpawner {
    extends(dsSpawner)
} dsCloudSpawner;

void __dsCloudSpawner_spawn(dsCloudSpawner* _this);

_static float __dsCloudSpawner_nextDelay();

///
/// \brief Constructs a dsCloudSpawner with associated scene.
/// \return pointer to a new dsCloudSpawner instance
///
constructor(dsCloudSpawner)(dsScene* scene) {
    dsCloudSpawner* _this = malloc(sizeof(dsCloudSpawner));
    dsSpawner_super((dsSpawner*) _this, (void (*)(dsSpawner*)) __dsCloudSpawner_spawn,
                    __dsCloudSpawner_nextDelay, scene);
    return _this;
}

///
/// \brief Creates a new cloud and adds it to the associated scene, placing it right behind the edge of
/// the window at a random height between the floor and the top of the window, including some margin.
/// \param _this the formal instance method parameter
///
void __dsCloudSpawner_spawn(dsCloudSpawner* _this) {
    dsCloud* cloud = _new(dsCloud)();
    const float posY = (float) ds_randomFloat(48, g.config.floor.posY - 16);
    sfRectangleShape_setPosition(cloud->sprite, (sfVector2f) {(float) g.config.window.size.x, posY});
    dsScene_add(_this->scene, (dsGameObject*) cloud);
}

///
/// \return the delay between spawning two clouds; a random value from 2 to 4 seconds,
/// divided by the global game speed multiplier to keep the actual average distance even
///
_static float __dsCloudSpawner_nextDelay() {
    return (float) ds_randomFloat(2, 4) / g.speed;
}

#endif //DAINASO_CLOUD_SPAWNER_H
