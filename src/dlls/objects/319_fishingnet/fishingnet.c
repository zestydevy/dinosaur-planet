#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "functions.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    Object *unk10;
} fishingnet_Data;

// offset: 0x0 | ctor
void fishingnet_ctor(void *dll) { }

// offset: 0xC | dtor
void fishingnet_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void fishingnet_setup(Object *self, ObjSetup *setup, s32 arg2) {
    fishingnet_Data *objdata;

    objdata = self->data;
    objdata->unk1 = 0;
    objdata->unk4 = 0.0f;
    objdata->unk8 = 0.0f;
    objdata->unkC = 0.0f;
    objdata->unk10 = 0;
    self->srt.transl.f[0] = 0.0f;
    self->srt.transl.f[1] = 0.0f;
    self->srt.transl.f[2] = 0.0f;
}

// offset: 0x5C | func: 1 | export: 1
void fishingnet_control(Object *self) { }

// offset: 0x68 | func: 2 | export: 2
void fishingnet_update(Object *self) { }

// offset: 0x74 | func: 3 | export: 3
void fishingnet_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x8C | func: 4 | export: 4
void fishingnet_free(Object *self, s32 a1) { }

// offset: 0x9C | func: 5 | export: 5
u32 fishingnet_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xAC | func: 6 | export: 6
u32 fishingnet_get_data_size(Object *self, u32 a1) {
    return sizeof(fishingnet_Data);
}

// offset: 0xC0 | func: 7 | export: 7
void fishingnet_func_C0(Object *self, u8 arg1) {
    fishingnet_Data *objdata = self->data;
    objdata->unk1 = arg1;
}

// offset: 0xDC | func: 8 | export: 8
void fishingnet_func_DC(Object* self, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4, Vec3f* arg5, Vec3f* arg6) {
    fishingnet_Data* objdata;
    Object* temp_v1;
    ObjectHitInfo* objhitInfo;

    objdata = self->data;
    objhitInfo = self->objhitInfo;
    self->srt.transl.f[0] = arg5->x;
    self->srt.transl.f[1] = arg5->y - 0.25f;
    self->srt.transl.f[2] = arg5->z;
    arg6->y += 0.25f;
    if (objdata->unk1 != 0) {
        func_80024108(self, 0.006f, gUpdateRateF, NULL);
        draw_object(self, arg1, arg2, arg3, arg4, 1.0f);
        self->srt.yaw = arctan2_f(arg5->x - arg6->x, arg5->z - arg6->z) - 0x4000;
        self->srt.roll = arctan2_f(arg5->y - arg6->y, sqrtf(SQ(arg5->z - arg6->z) + SQ(arg5->x - arg6->x)));
        objhitInfo->unk40 = 0x10;
        objhitInfo->unk44 = 0x10;
        objhitInfo->unk5F = 0x11;
        objhitInfo->unk60 = 0;
        if (objdata->unk10 != NULL) {
            temp_v1 = objdata->unk10;
            func_80031F6C(self, 0, &temp_v1->srt.transl.x, &temp_v1->srt.transl.y, &temp_v1->srt.transl.z, 0);
            temp_v1->positionMirror.f[0] = temp_v1->srt.transl.f[0];
            temp_v1->positionMirror.f[1] = temp_v1->srt.transl.f[1];
            temp_v1->positionMirror.f[2] = temp_v1->srt.transl.f[2];
            temp_v1->srt.yaw = self->srt.yaw;
            temp_v1->srt.pitch = self->srt.roll;
            temp_v1->srt.roll = 0;
            draw_object(temp_v1, arg1, arg2, arg3, arg4, 1.0f);
        }
    }
}

// offset: 0x304 | func: 9 | export: 9
void fishingnet_func_304(Object* self, SRT* arg1) {
    fishingnet_Data* objdata;

    objdata = self->data;
    arg1->yaw = self->srt.yaw;
    arg1->pitch = self->srt.roll;
    arg1->transl.f[0] = objdata->unk4;
    arg1->transl.f[1] = objdata->unk8;
    arg1->transl.f[2] = objdata->unkC;
}

// offset: 0x338 | func: 10 | export: 10
u8 fishingnet_func_338(Object* self, s32 arg1, Object *arg2) {
    fishingnet_Data* objdata;
    Object* player;
    s32 temp_v0_2;

    objdata = self->data;
    player = get_player();
    temp_v0_2 = ((DLL_210_Player*)player->dll)->vtbl->func32(player);
    switch (arg1) {
    case 0x1:
        objdata->unk10 = NULL;
        if (temp_v0_2 == 0) {
            objdata->unk1 = 1;
            ((DLL_210_Player*)player->dll)->vtbl->func31(player, 3);
        }
        objdata->unk0 = 1;
        break;
    case 0x2:
        if (arg2 != 0) {
            objdata->unk10 = arg2;
        }
        self->objhitInfo->unk58 &= ~0x1;
        /* fallthrough */
    case 0x4:
        objdata->unk0 = (u8) arg1;
        break;
    case 0x8:
        objdata->unk10 = NULL;
        if (temp_v0_2 == 3) {
            objdata->unk1 = 0;
            ((DLL_210_Player*)player->dll)->vtbl->func31(player, 0);
        }
        objdata->unk0 = 8;
        break;
    case 0x20:
        if (objdata->unk0 == 1) {
            self->objhitInfo->unk58 |= 1;
        }
        break;
    case 0x40:
        self->objhitInfo->unk58 &= ~0x1;
        break;
    }
    return objdata->unk0;
}

// offset: 0x4D4 | func: 11 | export: 11
s32 fishingnet_func_4D4(Object *self, s32 arg1) {
    fishingnet_Data *objdata = self->data;
    
    switch (arg1) {
    case 0:
        return (objdata->unk0 & 2) == 0;
    case 1:
        return objdata->unk0 & 9;
    default:
        return 0;
    }
}
