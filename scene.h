#ifndef DAINASO_SCENE_H
#define DAINASO_SCENE_H
#pragma ide diagnostic ignored "cert-msc32-c"

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.h>
#include "cloud_spawner.h"
#include "dinosaur.h"
#include "dune_spawner.h"
#include "floor.h"
#include "game_context.h"
#include "macros.h"
#include "obstacle_spawner.h"
#include "score_counter.h"
#include "scrolling_object.h"
#include "stone_spawner.h"

typedef struct __dsListNode __dsListNode;

struct __dsListNode {
    dsGameObject* object;
    __dsListNode* previous, * next;
};

void __dsListNode_destroy(__dsListNode* _this) {
    free(_this->object);
    free(_this);
}

typedef struct dsScene {
    __dsListNode* __head, * __tail;
    int size;
    bool playing;
    sfColor background;
    sfSound* __gameOverSound;
    dsScoreCounter* __scoreCounter;
    dsDinosaur* __player;
} dsScene;

constructor(dsScene)() {
    dsScene* _this = malloc(sizeof(dsScene));
    _this->__head = null;
    _this->__tail = null;
    _this->size = 0;
    _this->playing = true;
    _this->background = g.config.scene.color;
    _this->__gameOverSound = dsAssetsBundle_get(g.assets, "sound.dainaso.hit");
    _this->__scoreCounter = _new(dsScoreCounter)();
    _this->__player = _new(dsDinosaur)();
    dsScene_add(_this, (dsGameObject*) _this->__scoreCounter);
    dsScene_add(_this, (dsGameObject*) _new(dsFloor)());
    dsScene_add(_this, (dsGameObject*) _this->__player);
    dsScene_add(_this, (dsGameObject*) _new(dsCloudSpawner)(_this));
    dsScene_add(_this, (dsGameObject*) _new(dsDuneSpawner)(_this));
    dsScene_add(_this, (dsGameObject*) _new(dsObstacleSpawner)(_this));
    dsScene_add(_this, (dsGameObject*) _new(dsStoneSpawner)(_this));
    srand(time(null));
    if (g.clock != null) free(g.clock);
    g.clock = _new(sfClock)();
    return _this;
}

contract_pure bool dsScene_isEmpty(const dsScene* _this) {
    return _this->size == 0;
}

void dsScene_add(dsScene* _this, dsGameObject* object) {
    __dsListNode* added = malloc(sizeof(__dsListNode));
    added->object = object;
    added->next = null;
    if (dsScene_isEmpty(_this)) {
        added->previous = null;
        _this->__head = _this->__tail = added;
    } else {
        added->previous = _this->__tail;
        _this->__tail->next = added;
        _this->__tail = added;
    }
    _this->size++;
}

void dsScene_addMultiple(dsScene* _this, dsGameObject** objects, int count) {
    for (int i = 0; i < count; i++) dsScene_add(_this, objects[i]);
}

void dsScene_remove(dsScene* _this, dsGameObject* object) {
    if (_this->size > 1) {
        for (__dsListNode* node = _this->__head; node != null; node = node->next) {
            if (node->object == object) {
                if (node->previous != null) node->previous->next = node->next;
                if (node->next != null) node->next->previous = node->previous;
                __dsListNode_destroy(node);
                _this->size--;
            }
        }
    } else if (not dsScene_isEmpty(_this) and _this->__head->object == object) {
        __dsListNode_destroy(_this->__head);
        _this->__head = _this->__tail = null;
        _this->size--;
    }
}

void dsScene_gameOver(dsScene* _this) {
    _this->playing = false;
    ds_updateHiScore(dsScoreCounter_stop(_this->__scoreCounter));
    dsDinosaur_hit(_this->__player);
    dsScene_addMultiple(_this, (dsGameObject**) ds_createGameOverText(), 2);
    sfSound_play(_this->__gameOverSound);
    g.speed = 1;
}

void dsScene_iteration(dsScene* _this, float deltaTime) {
    g.speed += deltaTime * 0.005f;
    for (__dsListNode* node = _this->__head; node != null; node = node->next) {
        dsGameObject* object = node->object;
        object->behavior(object, deltaTime);
        if ((INSTANCE_OF(object, dsCactus) or INSTANCE_OF(object, dsPterodactyl))
            and dsDinosaur_touching(_this->__player, object))
            dsScene_gameOver(_this);
        if (INSTANCE_OF(object, dsScrollingObject) and ((dsScrollingObject*) object)->markedToRemove)
            dsScene_remove(_this, object);
    }
}

void dsScene_ignoreFirstJump(dsScene* _this) {
    _this->__player->__ignoreJump = true;
}

void __dsScene_drawSelected(const dsScene* _this, sfRenderWindow* window, bool (* predicate)(const dsGameObject*)) {
    for (__dsListNode* node = _this->__head; node != null; node = node->next) {
        const dsGameObject* object = node->object;
        if (predicate(object)) object->draw(object, window);
    }
}

contract_pure bool __dsScene_isBackgroundLayer(const dsGameObject* object) {
    return INSTANCE_OF(object, dsFloor) or INSTANCE_OF(object, dsCloud) or INSTANCE_OF(object, dsStone);
}

contract_pure bool __dsScene_isForegroundLayer(const dsGameObject* object) {
    return INSTANCE_OF(object, dsDinosaur) or INSTANCE_OF(object, dsCactus) or INSTANCE_OF(object, dsText);
}

contract_pure bool __dsScene_isMiddleLayer(const dsGameObject* object) {
    return not __dsScene_isBackgroundLayer(object) and not __dsScene_isForegroundLayer(object);
}

void dsScene_draw(const dsScene* _this, sfRenderWindow* window) {
    __dsScene_drawSelected(_this, window, __dsScene_isBackgroundLayer);
    __dsScene_drawSelected(_this, window, __dsScene_isMiddleLayer);
    __dsScene_drawSelected(_this, window, __dsScene_isForegroundLayer);
}

#endif //DAINASO_SCENE_H
