#ifndef DAINASO_OBSTACLE_SPAWNER_H
#define DAINASO_OBSTACLE_SPAWNER_H

#include <stdio.h>
#include "cactus.h"
#include "macros.h"
#include "pterodactyl.h"
#include "scene_decl.h"
#include "spawner.h"
#include "util.h"

typedef struct __dsCactiBatch {
    bool large;
    int count, spawned;
} __dsCactiBatch;

contract_pure _static __dsCactiBatch __dsCactiBatch_createRandom() {
    __dsCactiBatch batch;
    batch.large = ds_randomBool();
    batch.count = batch.large ? ds_randomInt(1, 2) : ds_randomInt(1, 3);
    batch.spawned = 0;
    return batch;
}

typedef struct dsObstacleSpawner {
    extends(dsSpawner)

    __dsCactiBatch __currentBatch;
    bool __firstSkipped, __spawnPterodactyl, __pterodactylSpawned;
} dsObstacleSpawner;

void __dsObstacleSpawner_spawn(dsObstacleSpawner* _this);

float __dsObstacleSpawner_nextDelay(dsObstacleSpawner* _this);

void __dsObstacleSpawner_createNewObstacleData(dsObstacleSpawner* _this);

constructor(dsObstacleSpawner)(dsScene* scene) {
    dsObstacleSpawner* _this = malloc(sizeof(dsObstacleSpawner));
    dsSpawner_super((dsSpawner*) _this, (void (*)(dsSpawner*)) __dsObstacleSpawner_spawn,
                    (float (*)(dsSpawner*)) __dsObstacleSpawner_nextDelay, scene);
    _this->__firstSkipped = false;
    __dsObstacleSpawner_createNewObstacleData(_this);
    return _this;
}

contract_pure bool __dsObstacleSpawner_batchFinished(dsObstacleSpawner* _this) {
    return _this->__currentBatch.spawned == _this->__currentBatch.count
           or _this->__pterodactylSpawned or _this->__spawnPterodactyl;
}

void __dsObstacleSpawner_spawnPterodactyl(dsObstacleSpawner* _this) {
    __dsObstacleSpawner_createNewObstacleData(_this);
    dsPterodactyl* pterodactyl = _new(dsPterodactyl)();
    const float factor = dsPterodactyl_speed() / dsCactus_speed(),
            posX = g.config.dinosaur.posX + factor * ((float) g.config.window.size.x - g.config.dinosaur.posX),
            posY = g.config.floor.posY - (ds_percentChance(1.0f / 3) ? -16.0f : ds_randomBool() ? 16.0f : 0.0f);
    sfRectangleShape_setPosition(pterodactyl->sprite, (sfVector2f) {posX, posY});
    dsScene_add(_this->scene, (dsGameObject*) pterodactyl);
    _this->__pterodactylSpawned = true;
}

void __dsObstacleSpawner_spawnCactus(dsObstacleSpawner* _this) {
    if (__dsObstacleSpawner_batchFinished(_this)) __dsObstacleSpawner_createNewObstacleData(_this);
    dsCactus* cactus = _new(dsCactus)(_this->__currentBatch.large);
    const float posY = g.config.floor.posY + g.config.dinosaur.size + (float) ds_randomFloat(2, 8);
    sfRectangleShape_setPosition(cactus->sprite, (sfVector2f) {(float) g.config.window.size.x, posY});
    dsScene_add(_this->scene, (dsGameObject*) cactus);
    _this->__currentBatch.spawned++;
}

void __dsObstacleSpawner_spawn(dsObstacleSpawner* _this) {
    if (not _this->__firstSkipped) {
        _this->__firstSkipped = true;
        return;
    }
    if (_this->__spawnPterodactyl) __dsObstacleSpawner_spawnPterodactyl(_this);
    else __dsObstacleSpawner_spawnCactus(_this);
}

float __dsObstacleSpawner_nextDelay(dsObstacleSpawner* _this) {
    if (__dsObstacleSpawner_batchFinished(_this)) return ds_randomFloat(0.5f, 2);
    else return (_this->__currentBatch.large ? 0.085f : 0.06f) / g.speed;
}

void __dsObstacleSpawner_createNewObstacleData(dsObstacleSpawner* _this) {
    _this->__currentBatch = __dsCactiBatch_createRandom();
    _this->__spawnPterodactyl = ds_percentChance(0.5f);
    _this->__pterodactylSpawned = false;
}

#endif //DAINASO_OBSTACLE_SPAWNER_H
