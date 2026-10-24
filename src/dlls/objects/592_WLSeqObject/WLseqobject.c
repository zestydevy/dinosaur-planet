#include "common.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/objtype.h"

typedef struct {
    u8 unk0;
} WLSeqObject_Data;

static int WLSeqObject_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WLSeqObject_ctor(void* dll) { }

// offset: 0xC | dtor
void WLSeqObject_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WLSeqObject_obj_Setup(Object* self, Generic_Setup* objSetup, s32 reset) {
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = WLSeqObject_animCallback;
    self->unkE0 = 20;
}

// offset: 0x50 | func: 1 | export: 1
void WLSeqObject_obj_Control(Object* self) {
    Generic_Setup* objSetup;
    Object** objects;
    s32 count;
    s32 i;
    u8 found;
    Object* obj;

    objSetup = (Generic_Setup*)self->setup;
    
    switch (objSetup->type) {
    case 0:
        if (self->unkDC || 
            mainGetBits(BIT_Galleon_Arrived_at_Warlock_Mountain) || 
            mainGetBits(BIT_Play_Seq_00EF_Scales_Escapes_With_Kyte)
        ) {
            break;
        }
        
        objects = objGetAllOfType(7, &count);

        found = FALSE;
        for (i = 0; i < count; i++) {
            if (objects[i]->id == OBJ_WL_Galleon) {
                found = TRUE;
            }
        }
        
        if (found != FALSE) {
            if (self->unkE0 == 0) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                self->unkDC = 1;
                mainSetBits(BIT_Galleon_Arrived_at_Warlock_Mountain, TRUE);
            } else {
                gDLL_28_ScreenFade->vtbl->fade_reversed(80, 1);
            }
        } else {
            self->unkE0 = 20;
            gDLL_28_ScreenFade->vtbl->fade_reversed(80, 1);
        }
        
        self->unkE0--;
        if (self->unkE0 < 0) {
            self->unkE0 = 0;
        }
        break;
    case 8:
        break;
    }
}

// offset: 0x294 | func: 2 | export: 2
void WLSeqObject_obj_Update(Object* self) { }

// offset: 0x2A0 | func: 3 | export: 3
void WLSeqObject_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2F4 | func: 4 | export: 4
void WLSeqObject_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x304 | func: 5 | export: 5
u32 WLSeqObject_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x314 | func: 6 | export: 6
u32 WLSeqObject_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WLSeqObject_Data);
}

// offset: 0x328 | func: 7
int WLSeqObject_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
/*0x0*/ static u8 dAnimBool = 0;
    s32 i;

    if (self) {}
    
    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i] == 1) {
            dAnimBool = 1 - dAnimBool;
        }
    }
    
    animData->lastMessage = 0;
    animData->unk62 = 0;
    
    return 0;
}
