#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
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
/*0E*/ s16 unkE;
/*10*/ u16 unk10;
/*12*/ s16 gamebit;
/*14*/ u8 unk14;
/*15*/ u8 unk15;
/*16*/ u8 unk16;
/*17*/ u8 unk17;
/*18*/ s16 soundID1;
/*1A*/ s16 soundID2;
/*1C*/ s16 unk1C;
/*1E*/ u8 unk1E;
/*1F*/ u8 unk1F;
/*20*/ f32 unk20;
/*24*/ f32 unk24;
} MediumCrate_Data;

static DLL_IModgfx *_data_0 = NULL;
static DLL_IModgfx *_data_4 = NULL;
static u32 _data_8[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static s32 medium_crate_func_C50(Object *self, Object *player, MediumCrate_Data *objdata);

void medium_crate_ctor(void *dll) { }

void medium_crate_dtor(void *dll) { }

// export 0
void medium_crate_setup(Object *self, MediumCrate_Setup *setup, s32 param3) {
    MediumCrate_Data *objdata;
    s16 local_10[] = { 0, 1, 2, 3, 4, 8 };
    s16 local_18[] = { 0, 5, 6, 7 };

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

    self->unkB0 |= 0x2000;
    self->srt.yaw = setup->unk18 << 8;

    if (self->id == OBJ_MediumBasket) {
        objdata->unk15 = local_10[objdata->unk15];
        objdata->soundID1 = SOUND_6B5;
        objdata->soundID2 = SOUND_6B6;
    } else if (self->id == OBJ_MediumMetalCrat) {
        objdata->unk15 = local_18[objdata->unk15];
        objdata->soundID1 = SOUND_372;
        objdata->soundID2 = SOUND_371;
    }

    if (self->unk64 != NULL) {
        self->unk64->flags |= 0x8000;
        self->unk64->unk2c = self->unk64->unk0 + self->unk64->unk0;
    }
}

// export 1
void medium_crate_control(Object *self) {
    Object* sp9C;
    Object* player;
    MediumCrate_Setup* setup = (MediumCrate_Setup*)self->setup;
    f32 sp88[3] = { 0.0f, 1.0f, 0.0f };
    s32 sp80;
    s32 temp_v0_4;
    s32 sp7C = -1;
    s32 sp78;
    f32 temp_fv0_2;
    f32 sp70 = 1.0f;
    s32 sp60[4] = { 8, 255, 255, 120 };
    SRT sp48;
    MediumCrate_Data* objdata;

    gDLL_7_Newday->vtbl->func5(&sp70);
    objdata = self->data;
    player = get_player();
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) == 0) {
        return;
    }
    if (objdata->unk4 > 0.0f) {
        if (objdata->unk0 != -1) {
            objdata->unk4 -= (gUpdateRateF * sp70);
            if (objdata->unk4 <= 0.0f) {
                objdata->unk4 = 0.0f;
                objdata->unkC = 0;
                func_8002674C(self);
                self->unkAF &= ~0x8;
            }
        }
    } else {
        if (objdata->unkC != 0) {
            objdata->unk20 = 0;
            objdata->unkC -= gUpdateRate;
            if (objdata->unkC <= 0) {
                if (objdata->unk0 > 0) {
                    objdata->unk4 = 1.0f;
                    gDLL_29_Gplay->vtbl->add_time(setup->base.uID, (f32) objdata->unk0);
                } else {
                    objdata->unk4 = 1.0f;
                }
                self->srt.transl.x = setup->base.x;
                self->srt.transl.y = setup->base.y;
                self->srt.transl.z = setup->base.z;
                self->positionMirror2.x = setup->base.x;
                self->positionMirror2.y = setup->base.y;
                self->positionMirror2.z = setup->base.z;
                self->speed.x = 0.0f;
                self->speed.y = 0.0f;
                self->speed.z = 0.0f;
                func_800267A4(self);
            }
            if (objdata->unkC <= 50) {
                return;
            }
        }
        
        self->srt.pitch = objdata->unk1C;
        objdata->unk1C = (s16) (s32) ((f32) objdata->unk1C * -0.5f);
        if ((self->srt.pitch < 0xA) && (self->srt.pitch >= -9)) {
            self->srt.pitch = 0;
        }
        temp_v0_4 = func_8002601C(self, &sp9C, &sp7C, &sp78, &sp48.transl.x, &sp48.transl.y, &sp48.transl.z);
        if (temp_v0_4 != 0) {
            objdata->unk17 += sp78;
            if (objdata->unk17 < 2) {
                gDLL_6_AMSFX->vtbl->play_sound(self, objdata->soundID1, MAX_VOLUME, NULL, NULL, 0, NULL);
                objdata->unk20 = 1.0f;
                objdata->unk24 = 12.0f;
                if (self->id == OBJ_MediumBasket) {
                    objdata->unk1C = rand_next(600, 800);
                }
                if (temp_v0_4 != 0xF) {
                    sp48.transl.x += gWorldX;
                    sp48.roll = 0;
                    sp48.transl.z += gWorldZ;
                    sp48.pitch = 0;
                    sp48.yaw = 0;
                    sp48.scale = 1.0f;
                    _data_4->vtbl->func0(NULL, 1, &sp48, 0x401, -1, &sp60);
                }
            } else {
                if (objdata->unk8 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk8);
                    objdata->unk8 = 0;
                }
                _data_0->vtbl->func0(self, 1, NULL, 2, -1, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(self, objdata->soundID2, MAX_VOLUME, NULL, NULL, 0, NULL);
                objdata->unkC = 50;
                objdata->unk17 = 0;
                medium_crate_func_C50(self, player, objdata);
                self->unkAF |= 8;
            }
        }
        temp_fv0_2 = vec3_distance_squared(&get_player()->positionMirror, &self->positionMirror);
        objdata->unkE -= gUpdateRate;
        if (objdata->unkE <= 0) {
            objdata->unkE = rand_next(0, 100) + 300;
            if (temp_fv0_2 < 8100.0f) {
                if ((objdata->unk15 == 5) || (objdata->unk15 == 6) || (objdata->unk15 == 7) || (objdata->unk15 == 8)) {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_64D, 0x39, NULL, NULL, 0, NULL);
                } else if (objdata->unk15 == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_6B7, 0x39, NULL, NULL, 0, NULL);
                }
            }
        } else {
            if ((objdata->unk15 == 1) || (objdata->unk15 == 2) || (objdata->unk15 == 3) || (objdata->unk15 == 4)) {
                if (objdata->unk8 == 0) {
                    if (temp_fv0_2 < 8100.0f) {
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_64C, 0x39, &objdata->unk8, NULL, 0, NULL);
                    }
                } else if (temp_fv0_2 >= 8100.0f) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk8);
                    objdata->unk8 = 0;
                }
            }
        }
        
        if (objdata->unk20 > 0.0f) {
            objdata->unk20 += (gUpdateRateF * objdata->unk24);
            if (objdata->unk20 < 0.0f) {
                objdata->unk20 = 0.0f;
                return;
            }
            if (objdata->unk20 > 120.0f) {
                objdata->unk20 = 120.0f - (objdata->unk20 - 120.0f);
                objdata->unk24 = -objdata->unk24;
            }
        }
    }
}

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

    if (self->unkE0 != 0) {
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

// TODO: move to headers
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

s32 medium_crate_func_C50(Object *self, Object *player, MediumCrate_Data *objdata) {
    s32 spawnsLeft;
    Object *obj;
    SRT srt;
    f32 magnitude;
    s32 temp;
    Scorpion_Setup *scorpionSetup;
    Scarab_Setup *scarabSetup;
    FoodPickup_Setup *foodPickupSetup;
    
    spawnsLeft = objdata->unk16;
    while (spawnsLeft != 0) {
        spawnsLeft--;

        switch (objdata->unk15) {
            case 0: {
                scorpionSetup = obj_alloc_create_info(sizeof(Scorpion_Setup), OBJ_Scorpion);
                scorpionSetup->unk18 = rand_next(-127, 126);
                scorpionSetup->base.x = rand_next(-10, 10) + self->srt.transl.x;
                scorpionSetup->base.y = self->srt.transl.y;
                scorpionSetup->base.z = rand_next(-10, 10) + self->srt.transl.z;
                scorpionSetup->unk1A = 49;
                scorpionSetup->unk19 = 7;
                obj_create((ObjSetup*)scorpionSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
                break;
            }
            case 1: {
                scarabSetup = obj_alloc_create_info(sizeof(Scarab_Setup), OBJ_Green_scarab);
                scarabSetup->base.x = self->srt.transl.x;
                scarabSetup->base.y = self->srt.transl.y;
                scarabSetup->base.z = self->srt.transl.z;
                scarabSetup->unk1A = 400;
                obj = obj_create((ObjSetup*)scarabSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0) {
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
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF & 0xFFFF & 0xFFFF;
                temp = obj->srt.yaw - temp;
                CIRCLE_WRAP(temp)
                obj->srt.yaw = temp;
                break;
            }
            case 2: {
                scarabSetup = obj_alloc_create_info(sizeof(Scarab_Setup), OBJ_Red_scarab);
                scarabSetup->unk18 = rand_next(-127, 126);
                scarabSetup->base.x = self->srt.transl.x;
                scarabSetup->base.y = self->srt.transl.y;
                scarabSetup->base.z = self->srt.transl.z;
                scarabSetup->unk1A = 400;
                obj = obj_create((ObjSetup*)scarabSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0) {
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
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF & 0xFFFF & 0xFFFF;
                temp = obj->srt.yaw - temp;
                CIRCLE_WRAP(temp)
                obj->srt.yaw = temp;
                break;
            }
            case 3: {
                scarabSetup = obj_alloc_create_info(sizeof(Scarab_Setup), OBJ_Gold_scarab);
                scarabSetup->unk18 = rand_next(-127, 126);
                scarabSetup->base.x = self->srt.transl.x;
                scarabSetup->base.y = self->srt.transl.y;
                scarabSetup->base.z = self->srt.transl.z;
                scarabSetup->unk1A = 2000;
                obj = obj_create((ObjSetup*)scarabSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
                obj->speed.x = self->srt.transl.x - player->srt.transl.x;
                obj->speed.z = self->srt.transl.z - player->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0) {
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
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF & 0xFFFF & 0xFFFF;
                temp = obj->srt.yaw - temp;
                CIRCLE_WRAP(temp)
                obj->srt.yaw = temp;
                break;
            }
            case 4: {
                scarabSetup = obj_alloc_create_info(sizeof(Scarab_Setup), OBJ_Rain_scarab);
                scarabSetup->unk18 = rand_next(-127, 126);
                scarabSetup->base.x = self->srt.transl.x;
                scarabSetup->base.y = self->srt.transl.y;
                scarabSetup->base.z = self->srt.transl.z;
                scarabSetup->unk1A = 2000;
                obj = obj_create((ObjSetup*)scarabSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
                obj->speed.x = player->srt.transl.x - self->srt.transl.x;
                obj->speed.z = player->srt.transl.z - self->srt.transl.z;
                magnitude = obj->speed.x * obj->speed.x + obj->speed.z * obj->speed.z;
                if (magnitude != 0) {
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
                temp = arctan2_f(obj->speed.x, -obj->speed.z) & 0xFFFF & 0xFFFF & 0xFFFF;
                temp = obj->srt.yaw - temp;
                CIRCLE_WRAP(temp)
                obj->srt.yaw = temp;
                break;
            }
            case 5:
            case 6: {
                if (objdata->unk15 == 5) {
                    foodPickupSetup = obj_alloc_create_info(sizeof(FoodPickup_Setup), OBJ_meatPickup);
                } else {
                    foodPickupSetup = obj_alloc_create_info(sizeof(FoodPickup_Setup), OBJ_applePickup);
                }
                foodPickupSetup->unk1A = 20;
                foodPickupSetup->unk2C = -1;
                foodPickupSetup->unk1C = -1;
                foodPickupSetup->base.x = self->srt.transl.x;
                foodPickupSetup->base.y = self->srt.transl.y + 5.0f;
                foodPickupSetup->base.z = self->srt.transl.z;
                foodPickupSetup->unk24 = -1;
                obj = obj_create((ObjSetup*)foodPickupSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
                obj->unkE0 = 21600;
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
