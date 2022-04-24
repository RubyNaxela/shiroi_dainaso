#ifndef DAINASO_DUNE_SPAWNER_H
#define DAINASO_DUNE_SPAWNER_H

#include <stdio.h>
#include "dune.h"
#include "macros.h"
#include "scene_decl.h"
#include "spawner.h"

typedef struct dsDuneSpawner {
    extends(dsSpawner)
} dsDuneSpawner;

void __dsDuneSpawner_spawn(dsDuneSpawner* _this);

float __dsDuneSpawner_nextDelay();

constructor(dsDuneSpawner)(dsScene* scene) {
    dsDuneSpawner* _this = malloc(sizeof(dsDuneSpawner));
    dsSpawner_super((dsSpawner*) _this, (void (*)(dsSpawner*)) __dsDuneSpawner_spawn,
                    __dsDuneSpawner_nextDelay, scene);
    return _this;
}

void __dsDuneSpawner_spawn(dsDuneSpawner* _this) {
    dsDune* dune = _new(dsDune)();
    const float posY = g.config.floor.posY + 0.875f * g.config.dinosaur.size - 3;
    sfRectangleShape_setPosition(dune->sprite, (sfVector2f) {(float) g.config.window.size.x, posY});
    dsScene_add(_this->scene, (dsGameObject*) dune);
}

_static float __dsDuneSpawner_nextDelay() {
    return (float) ds_randomInt(1, 4) / 2 / g.speed;
}

#endif //DAINASO_DUNE_SPAWNER_H
