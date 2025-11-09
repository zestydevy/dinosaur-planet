#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "PR/gbi.h"
#include "PR/os.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/curves.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/rand.h"
#include "functions.h"
#include "dll.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
} GPSH_flybaddie_Setup;

typedef struct {
    Vec4f unk0;
    Vec4f unk10;
    Vec4f unk20;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s8 unk50;
    s8 unk51;
    u8 unk52;
    s8 unk53;
} GPSH_flybaddie_Data;

/*0x0*/ static u8 _data_0 = 0;

static void GPSH_flybaddie_func_654(Object* self);
static void GPSH_flybaddie_func_7F8(Object* self);

// offset: 0x0 | ctor
void GPSH_flybaddie_ctor(void *dll) { }

// offset: 0xC | dtor
void GPSH_flybaddie_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void GPSH_flybaddie_setup(Object* self, GPSH_flybaddie_Setup* setup, s32 arg2) {
    GPSH_flybaddie_Data* objdata;
    f32 sp20[3];

    objdata = self->data;
    objdata->unk38 = self->srt.transl.x;
    objdata->unk3C = self->srt.transl.y;
    objdata->unk40 = self->srt.transl.z;
    sp20[0] = self->srt.transl.x;
    sp20[1] = self->srt.transl.y;
    sp20[2] = self->srt.transl.z;
    objdata->unk0.x = sp20[0];
    objdata->unk10.x = sp20[1];
    objdata->unk20.x = sp20[2];
    objdata->unk0.y = sp20[0];
    objdata->unk10.y = sp20[1];
    objdata->unk20.y = sp20[2];
    objdata->unk0.z = sp20[0];
    objdata->unk10.z = sp20[1];
    objdata->unk20.z = sp20[2];
    objdata->unk0.w = sp20[0];
    objdata->unk10.w = sp20[1];
    objdata->unk20.w = sp20[2];
    objdata->unk34 = 0.0055f;
    objdata->unk30 = 1.0f;
    objdata->unk51 = 0;
    objdata->unk50 = 0;
    objdata->unk48 = rand_next(0, 65000);
    objdata->unk4A = 10000;
    objdata->unk53 = 1;
    objdata->unk4C = (s16) (rand_next(0, 1000) + 1000);
    objdata->unk4E = rand_next(0, 1000);
    if (setup->unk1A == 0) {
        objdata->unk44 = -130.0f;
        return;
    }
    objdata->unk44 = (f32) setup->unk1A;
}

// offset: 0x19C | func: 1 | export: 1
void GPSH_flybaddie_control(Object* self) {
    GPSH_flybaddie_Data* objdata;
    s32 i;
    s32 _pad;
    Object *sp48;

    objdata = self->data;
    if (objdata->unk4E > 0) {
        objdata->unk4E -= (s16)gUpdateRateF;
    }
    if ((objdata->unk4E != -999) && (objdata->unk4E <= 0)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_72F_Harsh_Magical_Thrum_Loop, 0x28, NULL, NULL, 0, NULL);
        objdata->unk4E = -999;
    }
    self->srt.yaw += (s16) (objdata->unk48 / 10);
    self->srt.pitch += (s16) (objdata->unk4A / 10);
    if (objdata->unk4C != 0) {
        objdata->unk4C -= (s16)gUpdateRateF;
    }
    if (objdata->unk4C <= 0) {
        GPSH_flybaddie_func_7F8(self);
        objdata->unk4C = (s16) (rand_next(0, 1000) + 1000);
    }
    if (objdata->unk30 > 1.0f) {
        objdata->unk30 -= 1.0f;
        GPSH_flybaddie_func_654(self);
    }
    self->srt.transl.x = func_80004A60(&objdata->unk0, objdata->unk30, NULL);
    self->srt.transl.y = func_80004A60(&objdata->unk10, objdata->unk30, NULL);
    self->srt.transl.z = func_80004A60(&objdata->unk20, objdata->unk30, NULL);
    objdata->unk30 += (objdata->unk34 * (f32) (_data_0 + 1) * gUpdateRateF);
    gDLL_17_partfx->vtbl->spawn(self, self->modelInstIdx + PARTICLE_286, NULL, PARTFXFLAG_1, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, self->modelInstIdx + PARTICLE_286, NULL, PARTFXFLAG_1, -1, NULL);
    if ((func_80025F40(self, &sp48, NULL, NULL) == 0xF) && (sp48 != 0)) {
        if (sp48->id != OBJ_WGSH_projball) {
            for (i = 15; i != 0; i--) {
                gDLL_17_partfx->vtbl->spawn(self, self->modelInstIdx + PARTICLE_286, NULL, PARTFXFLAG_1, -1, NULL);
            }
            objdata->unk52++;
            objdata->unk34 *= 1.2f;
            if (objdata->unk52 >= 3) {
                obj_destroy_object(self);
                _data_0++;
                if (_data_0 >= 3) {
                    main_set_bits(BIT_5A9, 1);
                }
            } else {
                func_80023A18(self, self->modelInstIdx + 1);
            }
        }
    }
}

// offset: 0x5C0 | func: 2 | export: 2
void GPSH_flybaddie_update(Object *self) { }

// offset: 0x5CC | func: 3 | export: 3
void GPSH_flybaddie_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x620 | func: 4 | export: 4
void GPSH_flybaddie_free(Object *self, s32 a1) { }

// offset: 0x630 | func: 5 | export: 5
u32 GPSH_flybaddie_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x640 | func: 6 | export: 6
u32 GPSH_flybaddie_get_data_size(Object *self, u32 a1) {
    return sizeof(GPSH_flybaddie_Data);
}

// offset: 0x654 | func: 7
static void GPSH_flybaddie_func_654(Object* self) {
    GPSH_flybaddie_Data* objdata;
    f32 sp48[3];
    SRT sp30;

    objdata = self->data;
    objdata->unk0.x = objdata->unk0.y;
    objdata->unk10.x = objdata->unk10.y;
    objdata->unk20.x = objdata->unk20.y;
    objdata->unk0.y = objdata->unk0.z;
    objdata->unk10.y = objdata->unk10.z;
    objdata->unk20.y = objdata->unk20.z;
    objdata->unk0.z = objdata->unk0.w;
    objdata->unk10.z = objdata->unk10.w;
    objdata->unk20.z = objdata->unk20.w;
    sp48[0] = 0.0f;
    sp48[1] = 0.0f;
    sp48[2] = objdata->unk44;
    objdata->unk48 += (s16)rand_next(1000, 2000);
    if (((objdata->unk4A < 4001) && (objdata->unk53 == -1)) || ((objdata->unk4A >= 28000) && (objdata->unk53 == 1))) {
        objdata->unk53 = -objdata->unk53;
    }
    objdata->unk4A += ((s16)rand_next(2000, 3000) * objdata->unk53);
    sp30.roll = 0;
    sp30.transl.x = 0.0f;
    sp30.transl.y = 0.0f;
    sp30.transl.z = 0.0f;
    sp30.scale = 1.0f;
    sp30.pitch = objdata->unk4A;
    sp30.yaw = objdata->unk48;
    rotate_vec3(&sp30, sp48);
    objdata->unk0.w = (sp48[0] + objdata->unk38);
    objdata->unk10.w = (sp48[1] + (objdata->unk3C + 20.0f));
    objdata->unk20.w = (sp48[2] + objdata->unk40);
}

typedef struct {
    ObjSetup base;
    u8 _unk18[12];
} UnkObjSetup;

// offset: 0x7F8 | func: 8
static void GPSH_flybaddie_func_7F8(Object* self) {
    ObjSetup* objsetup;
    Object* obj;
    Object* player;
    f32 dirVec[3];
    f32 magnitude;

    player = get_player();
    self->positionMirror.x = self->srt.transl.x;
    self->positionMirror.y = self->srt.transl.y;
    self->positionMirror.z = self->srt.transl.z;
    objsetup = mmAlloc(sizeof(UnkObjSetup), ALLOC_TAG_OBJECTS_COL, NULL);
    bzero(objsetup, sizeof(UnkObjSetup));
    // @bug: ? Doesn't set objId (ends up zero, which is OBJ_Sabre)
    objsetup->loadFlags = OBJSETUP_LOAD_FLAG2;
    objsetup->fadeFlags = OBJSETUP_FADE_DISABLE;
    objsetup->loadDistance = 0xFF;
    objsetup->fadeDistance = 0xFF;
    objsetup->x = self->srt.transl.x;
    objsetup->y = self->srt.transl.y;
    objsetup->z = self->srt.transl.z;
    obj = obj_create(objsetup, OBJ_INIT_FLAG1, -1, -1, NULL);
    if (obj != NULL) {
        obj->srt.flags |= 0x2000;
        dirVec[0] = player->srt.transl.x - self->srt.transl.x;
        dirVec[1] = (player->srt.transl.y + 20.0f) - self->srt.transl.y;
        dirVec[2] = player->srt.transl.z - self->srt.transl.z;

        magnitude = SQ(dirVec[0]) + SQ(dirVec[1]) + SQ(dirVec[2]);
        if (magnitude != 0.0f) {
            magnitude = sqrtf(magnitude);
            dirVec[0] /= magnitude;
            dirVec[1] /= magnitude;
            dirVec[2] /= magnitude;
        }
        obj->srt.transl.x += dirVec[0] * 10.0f;
        obj->srt.transl.y += dirVec[1] * 10.0f;
        obj->srt.transl.z += dirVec[2] * 10.0f;
        obj->speed.x = 2.0f * dirVec[0];
        obj->speed.y = 2.0f * dirVec[1];
        obj->speed.z = 2.0f * dirVec[2];
        obj->unkDC = 0xBE;
        obj->unkE0 = 0;
        obj->positionMirror.x = obj->srt.transl.x;
        obj->positionMirror.y = obj->srt.transl.y;
        obj->positionMirror.z = obj->srt.transl.z;
        gDLL_6_AMSFX->vtbl->play_sound(obj, SOUND_730_Electrified_Blast, 0x50, NULL, NULL, 0, NULL);
    }
}
