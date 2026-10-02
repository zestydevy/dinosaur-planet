#include "dll.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/gfx/modgfx.h"
#include "sys/map.h"
#include "sys/objprint.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object_id.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 _unk5;
} VFP_DoorSwitch_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} VFP_DoorSwitch_Setup;

static void VFP_DoorSwitch_func_23C(Object* self);
static void VFP_DoorSwitch_func_3D4(Object* self);

// offset: 0x0 | ctor
void VFP_DoorSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void VFP_DoorSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_DoorSwitch_obj_Setup(Object* self, VFP_DoorSwitch_Setup* setup, s32 reset) {
    VFP_DoorSwitch_Data* objdata;

    objdata = self->data;
    self->srt.yaw = setup->unk18 << 8;
    self->srt.roll = setup->unk19 << 8;
    self->srt.pitch = setup->unk1C;
    objdata->unk0 = (s16) setup->unk1E;
    objdata->unk2 = (s16) setup->unk1A;
    if (mainGetBits(objdata->unk0) != 0) {
        objdata->unk4 = 1;
    }
    if ((self->id == OBJ_VFP_LiftIndicat) && (objdata->unk4 != 0)) {
        self->modelInstIdx = 1;
    }
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0xDC | func: 1 | export: 1
void VFP_DoorSwitch_obj_Control(Object* self) {
    if (self->id != OBJ_VFP_LiftIndicat) {
        VFP_DoorSwitch_func_23C(self);
    } else {
        VFP_DoorSwitch_func_3D4(self);
    }
}

// offset: 0x140 | func: 2 | export: 2
void VFP_DoorSwitch_obj_Update(Object* self) { }

// offset: 0x14C | func: 3 | export: 3
void VFP_DoorSwitch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    VFP_DoorSwitch_Data* objdata = self->data;

    if (self->id != OBJ_VFP_LiftIndicat) {
        if (visibility == 0 || objdata->unk4 != 0) {
            return;
        }
    } else if (visibility == 0) {
        return;
    }

    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x1D0 | func: 4 | export: 4
void VFP_DoorSwitch_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x218 | func: 5 | export: 5
u32 VFP_DoorSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x228 | func: 6 | export: 6
u32 VFP_DoorSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(VFP_DoorSwitch_Data);
}

// offset: 0x23C | func: 7
static void VFP_DoorSwitch_func_23C(Object* self) {
    static DLL_IModgfx* data_0 = NULL;
    
    VFP_DoorSwitch_Data* objdata;
    int i;
    DLL_IModgfx* temp_v0;

    objdata = self->data;
    self->srt.roll += objdata->unk2 * (s16) gUpdateRateF;
    if (objdata->unk4 != 0 || mainGetBits(objdata->unk0) == 0) {
        return;
    } 
    gDLL_6_AMSFX->vtbl->Play(self, SOUND_B55, MAX_VOLUME, NULL, NULL, 0, NULL);
    i =  0x28;
    // FAKE
    do {} while (0);
    while (i--) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_551, NULL, 2, -1, NULL);
    }
    data_0 = dllLoad(DLL_ID_107, 1);
    data_0->vtbl->Spawn(self, 0x11, NULL, 2, -1, NULL);
    dllFree(data_0);

    objdata->unk4 = 1;
}

// offset: 0x3D4 | func: 8
static void VFP_DoorSwitch_func_3D4(Object* self) {
    VFP_DoorSwitch_Data* objdata;

    objdata = self->data;
    if (objdata->unk4 == 0) {
        if (mainGetBits(objdata->unk0) != 0) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_B55, MAX_VOLUME, NULL, NULL, 0, NULL);
            objSetModel(self, 1);
            objdata->unk4 = 1;
        }
    }
}
