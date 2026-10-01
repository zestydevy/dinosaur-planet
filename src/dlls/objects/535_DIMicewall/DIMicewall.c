#include "common.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    s16 gamebitMelted;
    s8 health;
    s8 melted;
    s8 flameRange;
} DIMIceWall_Data;

typedef struct {
    ObjSetup base;
    u8 _unk18;
    s16 health;
    s16 flameRange;
    s16 gamebitMelted;   
} DIMIceWall_Setup;

// offset: 0x0 | ctor
void DIMIceWall_ctor(void* dll) { }

// offset: 0xC | dtor
void DIMIceWall_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMIceWall_obj_Setup(Object* self, DIMIceWall_Setup* setup, s32 reset) {
    DIMIceWall_Data* objData = self->data;

    objData->health = setup->health;
    objData->flameRange = setup->flameRange;
    objData->gamebitMelted = setup->gamebitMelted;

    objAddObjectType(self, OBJTYPE_DIMIceWall);
    objAddObjectType(self, OBJTYPE_TrickyTarget);

    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
}

// offset: 0xA4 | func: 1 | export: 1
void DIMIceWall_obj_Control(Object* self) {
    DIMIceWall_Data* objData;
    Object* sidekick;

    objData = self->data;

    if (objData->melted) {
        return;
    }

    if ((objData->health <= 0) && (objData->melted == FALSE)) {
        if (objData->gamebitMelted != NO_GAMEBIT) {
            mainSetBits(objData->gamebitMelted, TRUE);
            objData->melted = TRUE;
        }
    } else {
        sidekick = objGetSidekick();
        if (sidekick != NULL) {
            if (vec3DistanceSquared(&self->globalPosition, &objGetPlayer()->globalPosition) <= SQ(objData->flameRange)) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->EnableCommand(sidekick, Sidekick_Command_INDEX_4_Flame);
            }
        }
    }
}

// offset: 0x1D0 | func: 2 | export: 2
void DIMIceWall_obj_Update(Object* self) { }

// offset: 0x1DC | func: 3 | export: 3
void DIMIceWall_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x1F4 | func: 4 | export: 4
void DIMIceWall_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_DIMIceWall);
    objFreeObjectType(self, OBJTYPE_TrickyTarget);
}

// offset: 0x254 | func: 5 | export: 5
u32 DIMIceWall_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x264 | func: 6 | export: 6
u32 DIMIceWall_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DIMIceWall_Data);
}

// offset: 0x278 | func: 7 | export: 7
s32 DIMIceWall_TickFlame(Object* self, s32 damage) {
    DIMIceWall_Data* objData = self->data;

    objData->health -= damage;
    if (objData->health <= 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
