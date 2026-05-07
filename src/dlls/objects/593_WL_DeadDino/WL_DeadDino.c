#include "common.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"

typedef struct {
    s32 _unk0;
    s32 _unk4;
    f32 unk8;
    s32 _unkC;
    u8 unk10;
    u8 unk11;
    u16 unk12;
    u32 unk14;
} DeadDino_Data;

typedef struct {
    ObjSetup base;
    u16 _unk18;
    u8 unk1A;
    s16 unk1C;
    s16 _unk1E;
    s32 _unk20;
    s16 unk24;
    s16 _unk26;
    s32 unk28;
    s16 unk2C;
    s16 _unk2E;
} DeadDino_Setup;

/*0x0*/ static u32 _data_0[] = {
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0677ffff, 
    0xffffffff, 0x01000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 
    0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 
    0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static void dll_593_func_8EC(Object* arg0);

// offset: 0x0 | ctor
void dll_593_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_593_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_593_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/593_WL_DeadDino/dll_593_setup.s")

// offset: 0xD8 | func: 1 | export: 1
void dll_593_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/593_WL_DeadDino/dll_593_control.s")
//https://decomp.me/scratch/7rAKU, will add later as functions.h is going to be nuked anyway

// offset: 0x61C | func: 2 | export: 2
void dll_593_update(Object *self) { }

// offset: 0x628 | func: 3 | export: 3
void dll_593_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DeadDino_Data* objdata;

    objdata = self->data;
    if ((visibility != 0) && (objdata->unk11 == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (objdata->unk8 != 0.0f) {
            if (objdata->unk8 >= 1.8f) {
                gDLL_32->vtbl->func2(self, 0x330, NULL);
            } else {
                gDLL_32->vtbl->func2(self, 0x32F, &objdata->unk8);
            }
            objdata->unk8 += (gUpdateRateF * 0.01f);
            if (objdata->unk8 >= 2.0f) {
                gDLL_6_AMSFX->vtbl->play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
                objdata->unk11 = 1U;
                objdata->unk8 = 0.0f;
                dll_593_func_8EC(self);
            }
        }
    }
}

// offset: 0x7BC | func: 4 | export: 4
void dll_593_free(Object *self, s32 a1) { }

// offset: 0x7CC | func: 5 | export: 5
u32 dll_593_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x7DC | func: 6 | export: 6
u32 dll_593_get_data_size(Object *self, u32 a1) {
    return sizeof(DeadDino_Data);
}

// offset: 0x7F0 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/593_WL_DeadDino/dll_593_func_7F0.s")

// offset: 0x8E4 | func: 8
void dll_593_func_8E4(void) {

}

// offset: 0x8EC | func: 9
static void dll_593_func_8EC(Object* arg0) {
    DeadDino_Setup* setup;

    setup = obj_alloc_setup(sizeof(DeadDino_Setup), OBJ_MagicDustMid);
    setup->unk1A = 0x14;
    setup->unk2C = -1;
    setup->unk1C = -1;
    setup->base.x = arg0->srt.transl.x;
    setup->base.y = arg0->srt.transl.y + 40.0f;
    setup->base.z = arg0->srt.transl.z;
    setup->unk24 = -1;
    setup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    setup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    setup->base.loadDistance = 0xFF;
    setup->base.fadeDistance = 0xFF;
    setup->base.objId = OBJ_MagicDustMid;
    obj_create((ObjSetup*)setup, OBJINIT_FLAG4 | OBJINIT_STANDALONE, (s32) arg0->mapID, -1, arg0->parent);
}
