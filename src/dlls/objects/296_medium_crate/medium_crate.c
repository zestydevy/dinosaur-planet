#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/modgfx/106.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 gamebit;
} MediumCrate_Setup;

typedef struct {
/*00*/ s32 unk0;
/*04*/ f32 unk4;
/*08*/ u32 unk8;
/*0C*/ s16 unkC;
/*0E*/ u16 unkE;
/*10*/ u16 unk10;
/*12*/ s16 gamebit;
/*14*/ u8 unk14;
/*15*/ u8 unk15;
/*16*/ u8 unk16;
/*17*/ u8 unk17;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ u8 unk1E;
/*1F*/ u8 unk1F;
/*20*/ f32 unk20;
/*24*/ f32 unk24;
} MediumCrate_Data;

typedef struct {
    s16 unk0[6];
} idk1;
typedef struct {
    s16 unk0[4];
} idk2;

static DLL_Unknown *_data_0 = NULL; // DAT_8122b7d0
static DLL_106 *_data_4 = NULL; // DAT_8122b7d4
static u32 _data_8[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
static idk1 _data_20 = {
    { 0, 1, 2, 3, 4, 8 }
};
static idk2 _data_2C = {
    { 0, 5, 6, 7 }
};
static u32 _data_34[] = {
    0x00000000, 0x3f800000, 0x00000000
};
static u32 _data_40[] = {
    0x00000008, 0x000000ff, 0x000000ff, 0x00000078
};

void medium_crate_ctor(void *dll) { }

void medium_crate_dtor(void *dll) { }

// export 0
void medium_crate_setup(Object *self, MediumCrate_Setup *setup, s32 param3) {
    MediumCrate_Data *objdata;
    idk1 local_10;
    idk2 local_18;

    local_10 = _data_20;
    local_18 = _data_2C;

    objdata = (MediumCrate_Data*)self->data;

    self->srt.yaw = setup->unk18 << 8;
    objdata->gamebit = setup->gamebit;

    if (setup->unk1C == 0) {
        objdata->unk0 = 0;
    } else if (setup->unk1C == 0xFF) {
        objdata->unk0 = -1;
    } else {
        objdata->unk0 = setup->unk1C * 60;
    }

    if (main_get_bits(objdata->gamebit) != 0) {
        objdata->unk4 = 1.0f;
        func_800267A4(self);
    }

    objdata->unk15 = setup->unk19;

    _data_0 = dll_load(DLL_ID_107, 1, /*runConstructor=*/FALSE);
    _data_4 = dll_load(DLL_ID_106, 1, /*runConstructor=*/FALSE);

    objdata->unk8 = 0;
    objdata->unkE = rand_next(0, 100) + 300;
    objdata->unk10 = 400;
    objdata->unk16 = setup->unk1A;

    self->unk0xb0 |= 0x2000;
    self->srt.yaw = setup->unk18 << 8;

    if (self->id == OBJ_MediumBasket) {
        objdata->unk15 = local_10.unk0[objdata->unk15];
        objdata->unk18 = 0x6b5;
        objdata->unk1A = 0x6b6;
    } else if (self->id == OBJ_MediumMetalCrat) {
        objdata->unk15 = local_18.unk0[objdata->unk15];
        objdata->unk18 = 0x372;
        objdata->unk1A = 0x371;
    }

    if (self->ptr0x64 != NULL) {
        self->ptr0x64->flags |= 0x8000;
        self->ptr0x64->unk2c = self->ptr0x64->unk0 + self->ptr0x64->unk0;
    }
}

// export 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/296_medium_crate/medium_crate_control.s")

// export 2
void medium_crate_update(Object *self) { }

// export 3
void medium_crate_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    MediumCrate_Data *objdata;
    MediumCrate_Setup *setup;

    objdata = (MediumCrate_Data*)self->data;
    setup = (MediumCrate_Setup*)self->setup;

    if (!gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID)) {
        return;
    }
    if (objdata->unkC != 0 && objdata->unkC < 51) {
        return;
    }
    if (objdata->unk4 > 0.0f) {
        return;
    }

    if (self->unk_0xe0 != 0) {
        if (visibility != -1) {
            return;
        }
    } else {
        if (visibility == 0) {
            return;
        }
    }

    if (objdata->unk20 != 0) {
        func_80036FBC(200, 0, 0, objdata->unk20);
    }

    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// export 4
void medium_crate_free(Object *self, s32 param2) {
    MediumCrate_Data *objdata;

    objdata = (MediumCrate_Data*)self->data;

    gDLL_14_Modgfx->vtbl->func5(self);

    dll_unload(_data_0);
    dll_unload(_data_4);

    if (objdata->unk8 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk8);
        objdata->unk8 = 0;
    }
}

// export 5
u32 medium_crate_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// export 6
u32 medium_crate_get_data_size(Object *self, u32 currentSize) {
    return sizeof(MediumCrate_Data);
}

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    u8 _unk1C[8];
} Scorpion_Setup;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    u8 _unk1C[8];
} Scarab_Setup;

typedef struct {
    ObjSetup base;
    u8 _unk18[2];
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    u8 _unk1E[6];
    s16 unk24;
    u8 _unk26[6];
    s16 unk2C;
    u8 _unk2E[2];
} FoodPickup_Setup;

#ifndef NON_EQUIVALENT
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/296_medium_crate/medium_crate_func_C50.s")
#else
s32 medium_crate_func_C50(Object *self, Object *player, MediumCrate_Data *objdata) {
    s32 spawnsLeft;
    Object *obj;
    SRT srt;
    f32 magnitude;
    s32 temp;
    
    spawnsLeft = objdata->unk16;
    while (spawnsLeft != 0) {
        spawnsLeft--;

        switch (objdata->unk15) {
            case 0: {
                Scorpion_Setup *setup = (Scorpion_Setup*)obj_alloc_create_info(
                    sizeof(Scorpion_Setup), OBJ_Scorpion);
                setup->unk18 = rand_next(-127, 126);
                setup->base.x = rand_next(-10, 10) + self->srt.transl.x;
                setup->base.y = self->srt.transl.y;
                setup->base.z = rand_next(-10, 10) + self->srt.transl.z;
                setup->unk1A = 49;
                setup->unk19 = 7;
                obj_create((ObjSetup*)setup, OBJSETUP_FLAG_1 | OBJSETUP_FLAG_4, self->mapID, -1, self->parent);
                break;
            }
            case 1: {
                Scarab_Setup *setup = (Scarab_Setup*)obj_alloc_create_info(
                    sizeof(Scarab_Setup), OBJ_Green_scarab);
                setup->base.x = self->srt.transl.x;
                setup->base.y = self->srt.transl.y;
                setup->base.z = self->srt.transl.z;
                setup->unk1A = 400;
                obj = obj_create((ObjSetup*)setup, OBJSETUP_FLAG_1 | OBJSETUP_FLAG_4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0.0f) {
                    magnitude = sqrtf(magnitude);
                    obj->speed.x /= magnitude;
                    obj->speed.z /= magnitude;
                }
                obj->speed.x *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.z *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.y = 2.2f;
                srt.transl.x = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.z = 0.0f;
                srt.scale = 1.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.yaw = rand_next(-10000, 10000);
                rotate_vec3(&srt, obj->speed.f);
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF;
                temp = obj->srt.yaw - temp;
                if (temp > 0x8000) {
                    temp += 0xFFFF0001;
                }
                if (temp < -0x8000) {
                    temp += 0x0000FFFF;
                }
                obj->srt.yaw = temp;
                break;
            }
            case 2: {
                Scarab_Setup *setup = (Scarab_Setup*)obj_alloc_create_info(
                    sizeof(Scarab_Setup), OBJ_Red_scarab);
                setup->unk18 = rand_next(-127, 126);
                setup->base.x = self->srt.transl.x;
                setup->base.y = self->srt.transl.y;
                setup->base.z = self->srt.transl.z;
                setup->unk1A = 400;
                obj = obj_create((ObjSetup*)setup, OBJSETUP_FLAG_1 | OBJSETUP_FLAG_4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0.0f) {
                    magnitude = sqrtf(magnitude);
                    obj->speed.x /= magnitude;
                    obj->speed.z /= magnitude;
                }
                obj->speed.x *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.z *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.y = 2.2f;
                srt.transl.x = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.z = 0.0f;
                srt.scale = 1.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.yaw = rand_next(-10000, 10000);
                rotate_vec3(&srt, obj->speed.f);
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF;
                temp = obj->srt.yaw - temp;
                if (temp > 0x8000) {
                    temp += 0xFFFF0001;
                }
                if (temp < -0x8000) {
                    temp += 0x0000FFFF;
                }
                obj->srt.yaw = temp;
                break;
            }
            case 3: {
                Scarab_Setup *setup = (Scarab_Setup*)obj_alloc_create_info(
                    sizeof(Scarab_Setup), OBJ_Gold_scarab);
                setup->unk18 = rand_next(-127, 126);
                setup->base.x = self->srt.transl.x;
                setup->base.y = self->srt.transl.y;
                setup->base.z = self->srt.transl.z;
                setup->unk1A = 2000;
                obj = obj_create((ObjSetup*)setup, OBJSETUP_FLAG_1 | OBJSETUP_FLAG_4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0.0f) {
                    magnitude = sqrtf(magnitude);
                    obj->speed.x /= magnitude;
                    obj->speed.z /= magnitude;
                }
                obj->speed.x *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.z *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.y = 2.2f;
                srt.transl.x = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.z = 0.0f;
                srt.scale = 1.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.yaw = rand_next(-10000, 10000);
                rotate_vec3(&srt, obj->speed.f);
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF;
                temp = obj->srt.yaw - temp;
                if (temp > 0x8000) {
                    temp += 0xFFFF0001;
                }
                if (temp < -0x8000) {
                    temp += 0x0000FFFF;
                }
                obj->srt.yaw = temp;
                break;
            }
            case 4: {
                Scarab_Setup *setup = (Scarab_Setup*)obj_alloc_create_info(
                    sizeof(Scarab_Setup), OBJ_Rain_scarab);
                setup->unk18 = rand_next(-127, 126);
                setup->base.x = self->srt.transl.x;
                setup->base.y = self->srt.transl.y;
                setup->base.z = self->srt.transl.z;
                setup->unk1A = 2000;
                obj = obj_create((ObjSetup*)setup, OBJSETUP_FLAG_1 | OBJSETUP_FLAG_4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0.0f) {
                    magnitude = sqrtf(magnitude);
                    obj->speed.x /= magnitude;
                    obj->speed.z /= magnitude;
                }
                obj->speed.x *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.z *= (1.0f - 0.0099999998f * rand_next(0, 25));
                obj->speed.y = 2.2f;
                srt.transl.x = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.z = 0.0f;
                srt.scale = 1.0f;
                srt.roll = 0;
                srt.pitch = 0;
                srt.yaw = rand_next(-10000, 10000);
                rotate_vec3(&srt, obj->speed.f);
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF;
                temp = obj->srt.yaw - temp;
                if (temp > 0x8000) {
                    temp += 0xFFFF0001;
                }
                if (temp < -0x8000) {
                    temp += 0x0000FFFF;
                }
                obj->srt.yaw = temp;
                break;
            }
            case 5:
            case 6: {
                FoodPickup_Setup *setup;
                if (objdata->unk15 == 5) {
                    setup = (FoodPickup_Setup*)obj_alloc_create_info(
                        sizeof(FoodPickup_Setup), OBJ_meatPickup);
                } else {
                    setup = (FoodPickup_Setup*)obj_alloc_create_info(
                        sizeof(FoodPickup_Setup), OBJ_applePickup);
                }
                setup->unk1A = 20;
                setup->unk2C = -1;
                setup->unk1C = -1;
                setup->base.x = self->srt.transl.x;
                setup->base.y = self->srt.transl.y + 5.0f;
                setup->base.z = self->srt.transl.z;
                setup->unk24 = -1;
                obj = obj_create((ObjSetup*)setup, OBJSETUP_FLAG_1 | OBJSETUP_FLAG_4, self->mapID, -1, self->parent);
                obj->unk_0xe0 = 21600;
                break;
            }
            case 7:
            case 8:
                main_set_bits(objdata->gamebit, 1);
                break;
        }
    }
    
    return 0;
}
#endif
