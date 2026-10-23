// This DLL's official name is unknown, but it's mostly used by sequence prop objects that wait around for 
// another nearby object to call a sequence that includes them (by searching using `ACTORSETTING_UNK4000`). 
// WCSunDoor is one example: not doing much itself, but opening the Sun Temple when controlled by a nearby WCSeqObject.
// Based on that, the unofficial name "SeqProp" has been used here for this DLL.

#include "game/objects/setups/103.h"
#include "sys/objects.h"
#include "sys/objprint.h"

// offset: 0x0 | ctor
void SeqProp_ctor(void* dll) { }

// offset: 0xC | dtor
void SeqProp_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SeqProp_obj_Setup(Object* self, Ctrl103_Setup* setup, s32 reset) {
    self->srt.roll = setup->roll << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.yaw = setup->yaw << 8;

    if (setup->setting != 0) {
        self->srt.scale = setup->setting / 255.0f;
        if (self->srt.scale == 0.0f) {
            self->srt.scale = 1.0f;
        }
        self->srt.scale *= self->def->scale;
    }

    self->stateFlags |= (OBJSTATE_CONTROL_DISABLED | OBJSTATE_UPDATE_DISABLED);
}

// offset: 0xD4 | func: 1 | export: 1
void SeqProp_obj_Control(Object* self) { }

// offset: 0xE0 | func: 2 | export: 2
void SeqProp_obj_Update(Object* self) { }

// offset: 0xEC | func: 3 | export: 3
void SeqProp_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void SeqProp_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x150 | func: 5 | export: 5
u32 SeqProp_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 SeqProp_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
