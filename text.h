#ifndef DAINASO_TEXT_H
#define DAINASO_TEXT_H
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include <stdlib.h>
#include <SFML/Graphics.h>
#include "assets_bundle.h"
#include "game_context.h"
#include "game_object.h"
#include "macros.h"

typedef enum dsTextAlignment {
    dsStart, dsCenter, dsEnd
} dsTextAlignment;

#define dsText_BASE dsGameObject_BASE sfText* __text; dsTextAlignment __alignment;

typedef struct dsText {
    extends(dsGameObject)

    sfText* __text;
    dsTextAlignment __alignment, __verticalAlignment;
} dsText;

void dsText_setHorizontalAlignment(dsText* _this, dsTextAlignment alignment);

void dsText_setVerticalAlignment(dsText* _this, dsTextAlignment alignment);

void __dsText_draw(const dsText* _this, sfRenderWindow* window);

void dsText_super(dsText* _this) {
    dsGameObject_superEmpty((dsGameObject*) _this);
    _this->behavior = (void (*)(dsGameObject*, float)) pass;
    _this->draw = (void (*)(const dsGameObject*, sfRenderWindow*)) __dsText_draw;
    _this->__text = _new(sfText)();
    dsText_setHorizontalAlignment(_this, dsStart);
    dsText_setVerticalAlignment(_this, dsStart);
    sfText_setFillColor(_this->__text, g.config.textColor);
    sfText_setFont(_this->__text, dsAssetsBundle_get(g.assets, "ds:font.pixelboy"));
}

constructor(dsText)() {
    dsText* _this = malloc(sizeof(dsText));
    _this->type = dsTextT;
    dsText_super(_this);
    return _this;
}

void dsText_setHorizontalAlignment(dsText* _this, dsTextAlignment alignment) {
    _this->__alignment = alignment;
    const sfFloatRect bounds = sfText_getGlobalBounds(_this->__text);
    const sfVector2f origin = sfText_getOrigin(_this->__text);
    if (alignment == dsStart) sfText_setOrigin(_this->__text, (sfVector2f) {0, origin.y});
    else if (alignment == dsCenter) sfText_setOrigin(_this->__text, (sfVector2f) {bounds.width / 2, origin.y});
    else sfText_setOrigin(_this->__text, (sfVector2f) {bounds.width, origin.y});
}

void dsText_setVerticalAlignment(dsText* _this, dsTextAlignment alignment) {
    _this->__verticalAlignment = alignment;
    const sfFloatRect bounds = sfText_getGlobalBounds(_this->__text);
    const sfVector2f origin = sfText_getOrigin(_this->__text);
    if (alignment == dsStart) sfText_setOrigin(_this->__text, (sfVector2f) {origin.x, bounds.height});
    else if (alignment == dsCenter) sfText_setOrigin(_this->__text, (sfVector2f) {origin.x, 1.5f * bounds.height});
    else sfText_setOrigin(_this->__text, (sfVector2f) {origin.x, 2 * bounds.height});
}

void dsText_setText(dsText* _this, const char* text) {
    sfText_setString(_this->__text, text);
    dsText_setHorizontalAlignment(_this, _this->__alignment);
    dsText_setVerticalAlignment(_this, _this->__verticalAlignment);
}

void dsText_setPosition(dsText* _this, const sfVector2f position) {
    sfText_setPosition(_this->__text, position);
}

void dsText_setColor(dsText* _this, const sfColor color) {
    sfText_setFillColor(_this->__text, color);
}

void dsText_setOutline(dsText* _this, const sfColor color, float thickness) {
    sfText_setOutlineColor(_this->__text, color);
    sfText_setOutlineThickness(_this->__text, thickness);
}

void dsText_setFontSize(dsText* _this, int size) {
    sfText_setCharacterSize(_this->__text, size);
}

void __dsText_draw(const dsText* _this, sfRenderWindow* window) {
    sfRenderWindow_drawText(window, _this->__text, null);
}

#endif //DAINASO_TEXT_H