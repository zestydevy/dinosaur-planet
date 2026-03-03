#ifndef _DLLS_523_H
#define _DLLS_523_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

DLL_INTERFACE(DLL_523_SCCollectables) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*start_falling)(Object *self); //Causes a Shiny Nugget to fall from a tree
    /*8*/ void (*copy_object_position)(Object* self, Object* other); //Repositions another Object onto the current location of the collectable
};

#endif // _DLLS_523_H
