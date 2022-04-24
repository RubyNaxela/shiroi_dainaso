#ifndef DAINASO_SPAWNER_H
#define DAINASO_SPAWNER_H
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include "game_object.h"
#include "scene_decl.h"
#include "util.h"

#define dsSpawner_BASE dsGameObject_BASE void (* __spawn)(dsSpawner*);\
                       float (* __nextDelay)(); dsScene* scene; sfClock* __clock; float __delay;

typedef abstract struct dsSpawner dsSpawner;

typedef abstract struct dsSpawner {
    extends(dsGameObject)

    abstract void (* __spawn)(dsSpawner*);

    abstract float (* __nextDelay)(dsSpawner*);

    dsScene* scene;
    sfClock* __clock;
    float __delay;
} dsSpawner;

void __dsSpawner_behavior(dsSpawner* _this);

dsSpawner* dsSpawner_super(dsSpawner* _this, void(* spawn)(dsSpawner*),
                           float (* nextDelay)(dsSpawner*), dsScene* scene) {
    _this->type = dsSpawnerT;
    dsGameObject_superEmpty((dsGameObject*) _this);
    _this->behavior = (void (*)(dsGameObject*, float)) __dsSpawner_behavior;
    _this->__spawn = spawn;
    _this->__nextDelay = nextDelay;
    _this->scene = scene;
    _this->__clock = _new(sfClock)();
    _this->__delay = 0;
    return _this;
}

void __dsSpawner_behavior(dsSpawner* _this) {
    if (sfTime_asSeconds(sfClock_getElapsedTime(_this->__clock)) >= _this->__delay) {
        _this->__spawn(_this);
        _this->__delay = _this->__nextDelay(_this);
        sfClock_restart(_this->__clock);
    }
}

#endif //DAINASO_SPAWNER_H
