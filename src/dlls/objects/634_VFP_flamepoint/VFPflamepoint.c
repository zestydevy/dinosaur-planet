#include "common.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 flameCommandRange;
    s16 gamebitFlamed;
} VFP_FlamePoint_Setup;

typedef struct {
    s16 gamebitFlamed;
    s8 health;
    s8 flameWasUsed;
    u8 flameCommandRange;
} VFP_FlamePoint_Data;

// offset: 0x0 | ctor
void VFP_FlamePoint_ctor(void* dll) { }

// offset: 0xC | dtor
void VFP_FlamePoint_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_FlamePoint_setup(Object* self, VFP_FlamePoint_Setup* objSetup, s32 reset) {
    VFP_FlamePoint_Data* objData = self->data;
    
    objData->health = objSetup->unk1A;
    objData->flameCommandRange = objSetup->flameCommandRange;
    objData->gamebitFlamed = objSetup->gamebitFlamed;
    obj_add_object_type(self, OBJTYPE_TrickyTarget);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED;
}

// offset: 0x8C | func: 1 | export: 1
void VFP_FlamePoint_control(Object* self) {
    VFP_FlamePoint_Data* objData;
    VFP_FlamePoint_Setup* objSetup;
    Object* sidekick;

    objSetup = (VFP_FlamePoint_Setup*)self->setup;
    objData = self->data;
    
    if (objData->flameWasUsed) {
        return;
    }
    
    //Set gamebit after Flame used
    if ((objData->health <= 0) && (objData->flameWasUsed == FALSE)) {
        if (objData->gamebitFlamed != NO_GAMEBIT) {
            main_set_bits(objData->gamebitFlamed, TRUE);
            objData->flameWasUsed = TRUE;
        }
        return;
    }

    //Show Flame command in inventory when nearby
    sidekick = get_sidekick();
    if (sidekick != NULL) {
        if (vec3_distance_squared(&self->globalPosition, &get_player()->globalPosition) <= SQ(objSetup->flameCommandRange)) {
            ((DLL_ISidekick*)sidekick->dll)->vtbl->enable_command(sidekick, Sidekick_Command_INDEX_4_Flame);
        }
    }
}

// offset: 0x1BC | func: 2 | export: 2
void VFP_FlamePoint_update(Object* self) { }

// offset: 0x1C8 | func: 3 | export: 3
void VFP_FlamePoint_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x21C | func: 4 | export: 4
void VFP_FlamePoint_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, OBJTYPE_TrickyTarget);
}

// offset: 0x25C | func: 5 | export: 5
u32 VFP_FlamePoint_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x26C | func: 6 | export: 6
u32 VFP_FlamePoint_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(VFP_FlamePoint_Data);
}

// offset: 0x280 | func: 7 | export: 7
s32 VFP_FlamePoint_func_280(Object* self, s32 decreaseBy) {
    VFP_FlamePoint_Data* objData = self->data;
    
    objData->health -= decreaseBy;

    if (objData->health <= 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
