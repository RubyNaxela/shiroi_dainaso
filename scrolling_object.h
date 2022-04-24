#ifndef DAINASO_SCROLLING_OBJECT_H
#define DAINASO_SCROLLING_OBJECT_H
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include "macros.h"
#include "game_object.h"

#define IN_SCROLLING_OBJECT << 3u
#define dsScrollingObject_BASE dsGameObject_BASE dsScrollingObjectType scrollingType;\
                               float (* __speed)(); bool markedToRemove;

typedef enum dsScrollingObjectType {
    dsCactusT = 1u IN_SCROLLING_OBJECT, dsCloudT = 2u IN_SCROLLING_OBJECT, dsDuneT = 3u IN_SCROLLING_OBJECT,
    dsPterodactylT = 4u IN_SCROLLING_OBJECT, dsStoneT = 5u IN_SCROLLING_OBJECT
} dsScrollingObjectType;

typedef abstract struct dsScrollingObject {
    extends(dsGameObject)

    dsScrollingObjectType scrollingType;

    float (* __speed)();

    bool markedToRemove;
} dsScrollingObject;

void __dsScrollingObject_behavior(dsScrollingObject* _this, float deltaTime);

void dsScrollingObject_super(dsScrollingObject* _this, float (* speed)()) {
    _this->type = dsScrollingObjectT;
    _this->behavior = (void (*)(dsGameObject*, float)) __dsScrollingObject_behavior;
    _this->__speed = speed;
    _this->markedToRemove = false;
}

void __dsScrollingObject_behavior(dsScrollingObject* _this, float deltaTime) {
    sfRectangleShape_move(_this->sprite, (sfVector2f) {-deltaTime * _this->__speed(), 0});
    const sfFloatRect bounds = sfRectangleShape_getGlobalBounds(_this->sprite);
    if (bounds.left + bounds.width < 0) _this->markedToRemove = true;
}

#endif //DAINASO_SCROLLING_OBJECT_H
