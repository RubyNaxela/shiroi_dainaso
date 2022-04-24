#ifndef DAINASO_STONE_SPAWNER_H
#define DAINASO_STONE_SPAWNER_H

#include "game_context.h"
#include "macros.h"
#include "scene_decl.h"
#include "spawner.h"
#include "stone.h"

typedef struct dsStoneSpawner {
    extends(dsSpawner)
} dsStoneSpawner;

void __dsStoneSpawner_spawn(dsStoneSpawner* _this);

float __dsStoneSpawner_nextDelay();

constructor(dsStoneSpawner)(dsScene* scene) {
    dsStoneSpawner* _this = malloc(sizeof(dsStoneSpawner));
    dsSpawner_super((dsSpawner*) _this, (void (*)(dsSpawner*)) __dsStoneSpawner_spawn,
                    __dsStoneSpawner_nextDelay, scene);
    return _this;
}

void __dsStoneSpawner_spawn(dsStoneSpawner* _this) {
    dsStone* stone = _new(dsStone)();
    const float minPosY = g.config.floor.posY + 0.875f * g.config.dinosaur.size;
    sfRectangleShape_setPosition(stone->sprite, (sfVector2f) {(float) g.config.window.size.x,
                                                              minPosY + (float) ds_randomInt(4, 16)});
    dsScene_add(_this->scene, (dsGameObject*) stone);
}

_static float __dsStoneSpawner_nextDelay() {
    return (float) ds_randomInt(1, 10) / 50 / g.speed;
}

#endif //DAINASO_STONE_SPAWNER_H
