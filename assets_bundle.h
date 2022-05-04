#ifndef DAINASO_ASSETS_BUNDLE_H
#define DAINASO_ASSETS_BUNDLE_H

#include <string.h>
#include "macros.h"

///
/// \brief A map node that binds a string key with a pointer to an asset holding object.
///
typedef struct __dsABMapNode {
    const char* key;
    void* value;
} __dsABMapNode;

///
/// \brief Assets bundle is a dictionary (map) implemented as an array-list of map nodes.
///        Contains mappings from string keys to pointers to asset holding objects.
///
typedef struct dsAssetsBundle {
    __dsABMapNode* __array;
    int __allocated, size;
} dsAssetsBundle;

///
/// \brief Constructs an empty assets bundle with initially allocated memory for one entry.
/// \return pointer to a new dsAssetsBundle instance
///
constructor(dsAssetsBundle)() {
    dsAssetsBundle* _this = malloc(sizeof(dsAssetsBundle));
    _this->__array = malloc(sizeof(__dsABMapNode));
    _this->__allocated = 1;
    _this->size = 0;
    return _this;
}

///
/// \brief Checks if the specified assetsBundle is full, i.e. the size of the list is equal to the amount of allocated memory.
/// \param _this the formal instance method parameter
/// \return true or false, whether the assets bundle is full
///
contract_pure bool __dsAssetsBundle_isFull(const dsAssetsBundle* _this) {
    return _this->size == _this->__allocated;
}

///
/// \brief Returns the map node whose key is equal to the specified key, or null if there is no such node.
/// \param _this the formal instance method parameter
/// \param key the map node key
/// \return the node whose key is equal to the specified key
///
contract_pure __dsABMapNode* __dsAssetsBundle_getNode(const dsAssetsBundle* _this, const char* key) {
    for (__dsABMapNode* node = _this->__array; node < _this->__array + _this->size; node++)
        if (strcmp(node->key, key) == 0) return node;
    return null;
}

///
/// \brief Returns a pointer to the asset object associated with the specified key, or null if there is no such object.
/// \param _this the formal instance method parameter
/// \param key the asset key
/// \return a pointer to the asset object associated with the specified key
///
contract_pure void* dsAssetsBundle_get(const dsAssetsBundle* _this, const char* key) {
    const __dsABMapNode* node = __dsAssetsBundle_getNode(_this, key);
    return node != null ? node->value : null;
}

///
/// \brief Stores the asset object under the specified pointer, associating it with the specified key.
/// \param _this the formal instance method parameter
/// \param key the asset key
/// \param key pointer to the asset object
///
void dsAssetsBundle_register(dsAssetsBundle* _this, const char* key, void* value) {
    __dsABMapNode* existing = __dsAssetsBundle_getNode(_this, key);
    if (existing != null) existing->value = value;
    else {
        if (__dsAssetsBundle_isFull(_this)) {
            __dsABMapNode* old = _this->__array;
            _this->__array = calloc(_this->__allocated *= 2, sizeof(__dsABMapNode));
            memcpy(_this->__array, old, _this->size * sizeof(__dsABMapNode));
            free(old);
        }
        _this->__array[_this->size].key = key;
        _this->__array[_this->size++].value = value;
    }
}

#endif //DAINASO_ASSETS_BUNDLE_H
