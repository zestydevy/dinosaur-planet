#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/319_fishingnet.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "sys/objlib.h"

typedef struct {
    u8 flags;
    u8 visible;
    Vec3f unk4;
    Object* objCaught;
} fishingnet_Data;

typedef enum {
    FishingNet_FLAG_0 = 0,
    FishingNet_FLAG_1 = 1,
    FishingNet_FLAG_2 = 2,
    FishingNet_FLAG_4 = 4,
    FishingNet_FLAG_8 = 8
} fishingnet_Flags;

// offset: 0x0 | ctor
void fishingnet_ctor(void *dll) { }

// offset: 0xC | dtor
void fishingnet_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void fishingnet_setup(Object* self, ObjSetup *setup, s32 reset) {
    fishingnet_Data *objdata = self->data;

    objdata->visible = FALSE;
    objdata->unk4.x = 0.0f;
    objdata->unk4.y = 0.0f;
    objdata->unk4.z = 0.0f;
    objdata->objCaught = NULL;

    self->srt.transl.x = 0.0f;
    self->srt.transl.y = 0.0f;
    self->srt.transl.z = 0.0f;
}

// offset: 0x5C | func: 1 | export: 1
void fishingnet_control(Object* self) { }

// offset: 0x68 | func: 2 | export: 2
void fishingnet_update(Object* self) { }

// offset: 0x74 | func: 3 | export: 3
void fishingnet_print(Object* self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x8C | func: 4 | export: 4
void fishingnet_free(Object* self, s32 onlySelf) { }

// offset: 0x9C | func: 5 | export: 5
u32 fishingnet_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xAC | func: 6 | export: 6
u32 fishingnet_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(fishingnet_Data);
}

// offset: 0xC0 | func: 7 | export: 7
void fishingnet_set_visibility(Object* self, u8 enable) {
    fishingnet_Data *objdata = self->data;
    objdata->visible = enable;
}

// offset: 0xDC | func: 8 | export: 8
void fishingnet_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, Vec3f* heldPosition, Vec3f* endPosition) {
    fishingnet_Data* objdata;
    Object* caught;
    ObjectHitInfo* objhitInfo;

    objdata = self->data;
    objhitInfo = self->objhitInfo;
    self->srt.transl.x = heldPosition->x;
    self->srt.transl.y = heldPosition->y - 0.25f;
    self->srt.transl.z = heldPosition->z;
    endPosition->y += 0.25f;

    if (objdata->visible) {
        objAnimAdvance(self, 0.006f, gUpdateRateF, NULL);
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        self->srt.yaw = arctan2_f(heldPosition->x - endPosition->x, heldPosition->z - endPosition->z) - M_90_DEGREES;
        self->srt.roll = arctan2_f(heldPosition->y - endPosition->y, sqrtf(SQ(heldPosition->z - endPosition->z) + SQ(heldPosition->x - endPosition->x)));
        
        objhitInfo->unk40 = 16;
        objhitInfo->unk44 = 16;
        objhitInfo->unk5F = Damage_Type_Fishing_Net;
        objhitInfo->unk60 = 0;

        //Draw the object held in the net?
        if (objdata->objCaught != NULL) {
            caught = objdata->objCaught;
            objGetAttachPointWorldSpace(self, 0, &caught->srt.transl.x, &caught->srt.transl.y, &caught->srt.transl.z, 0);
            caught->globalPosition.f[0] = caught->srt.transl.f[0];
            caught->globalPosition.f[1] = caught->srt.transl.f[1];
            caught->globalPosition.f[2] = caught->srt.transl.f[2];
            caught->srt.yaw = self->srt.yaw;
            caught->srt.pitch = self->srt.roll;
            caught->srt.roll = 0;
            objprintDrawModel(caught, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x304 | func: 9 | export: 9
void fishingnet_get_transform(Object* self, SRT* oSrt) {
    fishingnet_Data* objdata = self->data;
    
    oSrt->yaw = self->srt.yaw;
    oSrt->pitch = self->srt.roll;
    oSrt->transl.x = objdata->unk4.x;
    oSrt->transl.y = objdata->unk4.y;
    oSrt->transl.z = objdata->unk4.z;
}

// offset: 0x338 | func: 10 | export: 10
u8 fishingnet_catch(Object* self, s32 flags, Object* caught) {
    fishingnet_Data* objdata;
    Object* player;
    s32 temp;

    objdata = self->data;
    player = objGetPlayer();
    temp = ((DLL_210_Player*)player->dll)->vtbl->func32(player);
    
    switch (flags) {
    case 0x1:
        objdata->objCaught = NULL;
        if (temp == 0) {
            objdata->visible = TRUE;
            ((DLL_210_Player*)player->dll)->vtbl->func31(player, 3);
        }
        objdata->flags = FishingNet_FLAG_1;
        break;
    case 0x2:
        if (caught != NULL) {
            objdata->objCaught = caught;
        }
        self->objhitInfo->unk58 &= ~1;
        /* fallthrough */
    case 0x4:
        objdata->flags = flags;
        break;
    case 0x8:
        objdata->objCaught = NULL;
        if (temp == 3) {
            objdata->visible = FALSE;
            ((DLL_210_Player*)player->dll)->vtbl->func31(player, 0);
        }
        objdata->flags = FishingNet_FLAG_8;
        break;
    case 0x20:
        if (objdata->flags == FishingNet_FLAG_1) {
            self->objhitInfo->unk58 |= 1;
        }
        break;
    case 0x40:
        self->objhitInfo->unk58 &= ~1;
        break;
    }
    
    return objdata->flags;
}

// offset: 0x4D4 | func: 11 | export: 11
s32 fishingnet_func_4D4(Object* self, s32 arg1) {
    fishingnet_Data *objdata = self->data;
    
    switch (arg1) {
    case 0:
        return (objdata->flags & FishingNet_FLAG_2) == 0;
    case 1:
        return objdata->flags & (FishingNet_FLAG_1 | FishingNet_FLAG_8);
    default:
        return 0;
    }
}
