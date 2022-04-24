#ifndef DAINASO_CLOUD_SPAWNER_H
#define DAINASO_CLOUD_SPAWNER_H

#include <stdio.h>
#include "cloud.h"
#include "macros.h"
#include "scene_decl.h"
#include "spawner.h"

typedef struct dsCloudSpawner {
    extends(dsSpawner)
} dsCloudSpawner;

void __dsCloudSpawner_spawn(dsCloudSpawner* _this);

float __dsCloudSpawner_nextDelay();

constructor(dsCloudSpawner)(dsScene* scene) {
    dsCloudSpawner* _this = malloc(sizeof(dsCloudSpawner));
    dsSpawner_super((dsSpawner*) _this, (void (*)(dsSpawner*)) __dsCloudSpawner_spawn,
                    __dsCloudSpawner_nextDelay, scene);
    return _this;
}

void __dsCloudSpawner_spawn(dsCloudSpawner* _this) {
    dsCloud* cloud = _new(dsCloud)();
    const float posY = (float) ds_randomFloat(48, g.config.floor.posY - 16);
    sfRectangleShape_setPosition(cloud->sprite, (sfVector2f) {(float) g.config.window.size.x, posY});
    dsScene_add(_this->scene, (dsGameObject*) cloud);
}

_static float __dsCloudSpawner_nextDelay() {
    return (float) ds_randomFloat(2, 4) / g.speed;
}

#endif //DAINASO_CLOUD_SPAWNER_H
