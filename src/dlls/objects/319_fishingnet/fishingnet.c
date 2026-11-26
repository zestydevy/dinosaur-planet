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

// offset: 0x0 | ctor
void fishingnet_ctor(void *dll) { }

// offset: 0xC | dtor
void fishingnet_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void fishingnet_obj_Setup(Object* self, ObjSetup *setup, s32 reset) {
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
void fishingnet_obj_Control(Object* self) { }

// offset: 0x68 | func: 2 | export: 2
void fishingnet_obj_Update(Object* self) { }

// offset: 0x74 | func: 3 | export: 3
void fishingnet_obj_Print(Object* self, Gfx **gdl, Mtx **mtxs, Vtx **vtxs, DLTri **pols, s8 visibility) { }

// offset: 0x8C | func: 4 | export: 4
void fishingnet_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x9C | func: 5 | export: 5
u32 fishingnet_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xAC | func: 6 | export: 6
u32 fishingnet_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(fishingnet_Data);
}

// offset: 0xC0 | func: 7 | export: 7
void fishingnet_SetVisibility(Object* self, u8 visible) {
    fishingnet_Data *objdata = self->data;
    objdata->visible = visible;
}

// offset: 0xDC | func: 8 | export: 8
void fishingnet_Draw(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, Vec3f* heldPosition, Vec3f* lookatPosition) {
    fishingnet_Data* objdata;
    Object* caught;
    ObjectHitInfo* objhitInfo;

    objdata = self->data;
    objhitInfo = self->objhitInfo;
    self->srt.transl.x = heldPosition->x;
    self->srt.transl.y = heldPosition->y - 0.25f;
    self->srt.transl.z = heldPosition->z;
    lookatPosition->y += 0.25f;

    if (objdata->visible) {
        objAnimAdvance(self, 0.006f, gUpdateRateF, NULL);
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        self->srt.yaw = mathAtan2f(heldPosition->x - lookatPosition->x, heldPosition->z - lookatPosition->z) - M_90_DEGREES;
        self->srt.roll = mathAtan2f(heldPosition->y - lookatPosition->y, sqrtf(SQ(heldPosition->z - lookatPosition->z) + SQ(heldPosition->x - lookatPosition->x)));
        
        objhitInfo->unk40 = 16;
        objhitInfo->unk44 = 16;
        objhitInfo->unk5F = Damage_Type_Fishing_Net;
        objhitInfo->unk60 = 0;

        //Draw the object held in the net?
        if (objdata->objCaught != NULL) {
            caught = objdata->objCaught;
            objGetAttachPointWorldSpace(self, 0, &caught->srt.transl.x, &caught->srt.transl.y, &caught->srt.transl.z, 0);
            caught->globalPosition.x = caught->srt.transl.x;
            caught->globalPosition.y = caught->srt.transl.y;
            caught->globalPosition.z = caught->srt.transl.z;
            caught->srt.yaw = self->srt.yaw;
            caught->srt.pitch = self->srt.roll;
            caught->srt.roll = 0;
            objprintDrawModel(caught, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x304 | func: 9 | export: 9
void fishingnet_GetTransform(Object* self, SRT* oSrt) {
    fishingnet_Data* objdata = self->data;
    
    oSrt->yaw = self->srt.yaw;
    oSrt->pitch = self->srt.roll;
    oSrt->transl.x = objdata->unk4.x;
    oSrt->transl.y = objdata->unk4.y;
    oSrt->transl.z = objdata->unk4.z;
}

// offset: 0x338 | func: 10 | export: 10
s32 fishingnet_State(Object* self, s32 flags, Object* caught) {
    fishingnet_Data* objdata;
    Object* player;
    s32 temp;

    objdata = self->data;
    player = objGetPlayer();
    temp = ((DLL_210_Player*)player->dll)->vtbl->func32(player);
    
    switch (flags) {
    case FISHINGNET_FLAG_Unk1:
        objdata->objCaught = NULL;
        if (temp == 0) {
            objdata->visible = TRUE;
            ((DLL_210_Player*)player->dll)->vtbl->func31(player, 3);
        }
        objdata->flags = FISHINGNET_FLAG_Unk1;
        break;
    case FISHINGNET_FLAG_Unk2:
        if (caught != NULL) {
            objdata->objCaught = caught;
        }
        self->objhitInfo->unk58 &= ~1;
        /* fallthrough */
    case FISHINGNET_FLAG_Unk4:
        objdata->flags = flags;
        break;
    case FISHINGNET_FLAG_Unk8:
        objdata->objCaught = NULL;
        if (temp == 3) {
            objdata->visible = FALSE;
            ((DLL_210_Player*)player->dll)->vtbl->func31(player, 0);
        }
        objdata->flags = FISHINGNET_FLAG_Unk8;
        break;
    case FISHINGNET_FLAG_Unk20:
        if (objdata->flags == FISHINGNET_FLAG_Unk1) {
            self->objhitInfo->unk58 |= 1;
        }
        break;
    case FISHINGNET_FLAG_Unk40:
        self->objhitInfo->unk58 &= ~1;
        break;
    }
    
    return objdata->flags;
}

// offset: 0x4D4 | func: 11 | export: 11
s32 fishingnet_Func4D4(Object* self, s32 arg1) {
    fishingnet_Data* objdata = self->data;
    
    switch (arg1) {
    case 0:
        return (objdata->flags & FISHINGNET_FLAG_Unk2) == 0;
    case 1:
        return objdata->flags & (FISHINGNET_FLAG_Unk1 | FISHINGNET_FLAG_Unk8);
    default:
        return 0;
    }
}
