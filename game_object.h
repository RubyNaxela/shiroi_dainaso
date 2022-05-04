#ifndef DAINASO_GAME_OBJECT_H
#define DAINASO_GAME_OBJECT_H

#include <stdint.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#include "game_context.h"
#include "macros.h"

#define IN_GAME_OBJECT << 0u
#define dsGameObject_BASE dsGameObjectType type; sfRectangleShape* sprite; void (* behavior)\
                          (dsGameObject*, float); void (* draw)(const dsGameObject*, sfRenderWindow*);

///
/// \brief Constants from this enum are used as metadata of dsGameObject
///        instances indicating which concrete class are they instances of.
///
typedef enum dsGameObjectType {
    dsDinosaurT = 1u IN_GAME_OBJECT, dsFloorT = 2u IN_GAME_OBJECT, dsScrollingObjectT = 3u IN_GAME_OBJECT,
    dsSpawnerT = 4u IN_GAME_OBJECT, dsTextT = 5u IN_GAME_OBJECT
} dsGameObjectType;

typedef abstract struct dsGameObject dsGameObject;

///
/// \brief The base for all game objects. These include every object visible in the game as well as technical objects.
///
typedef abstract struct dsGameObject {
    dsGameObjectType type;
    sfRectangleShape* sprite;

    ///
    /// \brief This function is called every frame. Its overrides define the behavior of the game object.
    ///
    abstract void (* behavior)(dsGameObject*, float);

    ///
    /// \brief As some game object classes have a different visual representation than the sprite field, and some are
    ///        not visible at all, Overrides of this function define how the game object should be drawn on the window.
    ///
    abstract void (* draw)(const dsGameObject*, sfRenderWindow*);
} dsGameObject;

dsGameObject* __dsGameObject_super(dsGameObject* _this, sfRectangleShape* sprite) {
    _this->sprite = sprite;
    _this->behavior = (void (*)(dsGameObject*, float)) pass;
    return _this;
}

void __dsGameObject_draw(const dsGameObject* _this, sfRenderWindow* window);

void dsGameObject_setTexture(dsGameObject* _this, const sfTexture* texture);

dsGameObject* dsGameObject_superEmpty(dsGameObject* _this) {
    _this->draw = pass;
    return __dsGameObject_super(_this, null);
}

dsGameObject* dsGameObject_superSprite(dsGameObject* _this, const sfTexture* texture, const sfVector2f size) {
    ASSERT_NOT_NULL(texture)
    sfRectangleShape* sprite = _new(sfRectangleShape)();
    sfRectangleShape_setSize(sprite, size);
    sfRectangleShape_setTexture(sprite, texture, false);
    _this->draw = __dsGameObject_draw;
    return __dsGameObject_super(_this, sprite);
}

dsGameObject* dsGameObject_superSolid(dsGameObject* _this, const sfColor color, const sfVector2f size) {
    sfRectangleShape* rectangle = _new(sfRectangleShape)();
    sfRectangleShape_setSize(rectangle, size);
    sfRectangleShape_setFillColor(rectangle, color);
    _this->draw = __dsGameObject_draw;
    return __dsGameObject_super(_this, rectangle);
}

void __dsGameObject_draw(const dsGameObject* _this, sfRenderWindow* window) {
    sfRenderWindow_drawRectangleShape(window, _this->sprite, null);
}

void dsGameObject_setTexture(dsGameObject* _this, const sfTexture* texture) {
    ASSERT_NOT_NULL(texture)
    ASSERT_NOT_NULL(_this->sprite)
    sfRectangleShape_setTexture(_this->sprite, texture, false);
}

#endif //DAINASO_GAME_OBJECT_H
