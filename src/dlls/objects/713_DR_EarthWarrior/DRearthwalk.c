#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/objects/713_DRearthwalk.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/modgfx.h"
#include "sys/segment_1050.h"
#include "sys/objtype.h"
#include "prevent_bss_reordering.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
} DRearthwalk_Setup;

typedef struct {
    ObjFSA_Data unk0;
    u8 _unk0x34C[0x394 - 0x34C];
    HeadAnimation unk394;
    MoveLibData unk3B8;
    u8 _unk870[0xA52 - 0x870];
    s16 unkA52;
    u8 _unkA54[0xA56 - 0xA54];
    s16 unkA56;
    u16 unkA58;
    u8 _unkA5A[0xA5C - 0xA5A];
    s8 unkA5C;
    u8 _unkA5D[0xA60 - 0xA5D];
    u8 unkA60_0 : 1;
    u8 unkA60_1 : 1;
    u8 unkA60_2 : 1;
    u8 unkA60_3 : 1;
    u8 unkA60_4 : 1;
    u8 unkA61;
    s8 unkA62;
    s8 unkA63;
} DRearthwalk_Data;

/*0x0*/ static u32 _data_0 = 0x0854081a;
/*0x4*/ static u32 _data_4 = 0x08530852;
/*0x8*/ static u32 _data_8[] = {
    0x08170800, 0x08180800, 0x08191000, 0x081a1000
};
/*0x18*/ static u32 _data_18[] = {
    0x07bf1000, 0x08161000, 0x081c1000
};
/*0x24*/ static s16 _data_24[] = {
    0x08ed, 0x1000, 0x0855, 0x0856, 0x0857, 0x0858
};
/*0x30*/ static Vec3f _data_30[] = {
    VEC3F(-9.0f, 0.0f, -12.0f), 
    VEC3F(9.0f, 0.0f, -12.0f), 
    VEC3F(9.0f, 0.0f, 12.0f), 
    VEC3F(-9.0f, 0.0f, 12.0f)
};
/*0x60*/ static f32 _data_60[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x70*/ static Vec3f _data_70[] = {
    VEC3F(0.0f, 0.0f, 35.0f), 
    VEC3F(0.0f, 0.0f, -35.0f)
};
/*0x88*/ static f32 _data_88[] = {
    25.0f, 25.0f
};
/*0x90*/ static u32 _data_90 = 0x037b037b;
/*0x94*/ static u32 _data_94[] = {
    0x00000008, 0x00000007, 0x00000009, 0x0000000a, 0x01c80000
};
/*0xA8*/ static u32 _data_A8 = 0x00180000;
/*0xAC*/ static u32 _data_AC[] = {
    0x3b4b295f, 0x00030003
};
/*0xB4*/ static f32 _data_B4[] = {
    0, 0.03, 0.05, 0.7, 0.6, 1.2, 0.8, 1.93, 
    1.72, 2.7
};
/*0xDC*/ static u32 _data_DC[] = {
    0x00020003, 0x00170016, 0x00040000
};

/*0x0*/ static u8 _bss_0[0x20];
/*0x20*/ static u8 _bss_20[0x8];
/*0x28*/ static u8 _bss_28[0x8];
/*0x30*/ static u8 _bss_30[0x40];

static int dll_713_func_1EBC(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_0.s")

// offset: 0x8C | ctor
void dll_713_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_ctor.s")

// offset: 0xCC | dtor
void dll_713_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void dll_713_setup(Object* self, DRearthwalk_Setup* setup, s32 arg2) {
    DRearthwalk_Data* objdata;
    s32 _pad[2];
    u8 sp50[] = {1, 1, 1, 1};
    s16 sp44[] = {0x000a, 0x000a, 0x0000, 0x0000, 0x0000};
    s16 sp38[] = {0x0014, 0x0014, 0x0000, 0x0000, 0x0000};

    self->srt.yaw = setup->unk18 << 8;
    self->animCallback = dll_713_func_1EBC;
    obj_add_object_type(self, OBJTYPE_11);
    objdata = self->data;
    objdata->unkA5C = setup->unk19;
    objdata->unkA52 = 5;
    objdata->unkA62 = -1;
    if (self->shadow != NULL) {
        self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR);
        self->shadow->maxDistScale = self->shadow->scale * 0.4f;
    }
    gDLL_18_objfsa->vtbl->func0(self, &objdata->unk0, 8, 1);
    objdata->unk0.unk29C = 0.17f;
    objdata->unk0.unk4.mode = 0;
    gDLL_27->vtbl->init(&objdata->unk0.unk4, DLL27FLAG_4000000 | DLL27FLAG_2000000 | DLL27FLAG_4, DLL27FLAG_NONE, DLL27MODE_DISABLED);
    gDLL_27->vtbl->setup_hits_collider(&objdata->unk0.unk4, 2, _data_70, _data_88, 8);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk0.unk4, 4, _data_30, _data_60, sp50);
    gDLL_27->vtbl->reset(self, &objdata->unk0.unk4);
    func_8002674C(self);
    create_temp_dll(53);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->unk3B8, -4551, 5461, 2);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->unk3B8, 300, 120);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->unk3B8, sp38, sp44, 2);
    if (setup->unk19 == 1) {
        objdata->unkA56 = 1;
        objdata->unkA58 |= 0x100;
        objdata->unkA60_1 = 1;
    } else {
        objdata->unkA56 = 5;
        objdata->unkA60_4 = 1;
    }
    objdata->unkA61 = 0x1E;
}

// offset: 0x408 | func: 2 | export: 1
void dll_713_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_control.s")

// offset: 0xABC | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_ABC.s")

// offset: 0xB54 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_B54.s")

// offset: 0x107C | func: 5 | export: 2
void dll_713_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_update.s")

// offset: 0x138C | func: 6 | export: 3
void dll_713_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_print.s")

// offset: 0x160C | func: 7 | export: 4
void dll_713_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_free.s")

// offset: 0x1660 | func: 8 | export: 5
u32 dll_713_get_model_flags(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_get_model_flags.s")

// offset: 0x1670 | func: 9 | export: 6
u32 dll_713_get_data_size(Object *self, u32 a1) {
    return sizeof(DRearthwalk_Data);
}

// offset: 0x1684 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1684.s")

// offset: 0x197C | func: 11 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_197C.s")

// offset: 0x1990 | func: 12 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1990.s")

// offset: 0x19B8 | func: 13 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_19B8.s")

// offset: 0x19DC | func: 14 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_19DC.s")

// offset: 0x1BB8 | func: 15 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1BB8.s")

// offset: 0x1BE0 | func: 16 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1BE0.s")

// offset: 0x1CA4 | func: 17 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1CA4.s")

// offset: 0x1CB4 | func: 18 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1CB4.s")

// offset: 0x1D5C | func: 19 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1D5C.s")

// offset: 0x1D84 | func: 20 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1D84.s")

// offset: 0x1DAC | func: 21 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1DAC.s")

// offset: 0x1DBC | func: 22 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1DBC.s")

// offset: 0x1DC8 | func: 23 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_1DC8.s")

// offset: 0x1EBC | func: 24
static int dll_713_func_1EBC(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    DRearthwalk_Data* objdata;
    Object** objList;
    s32 i;
    s32 numObjs;
    s32 partIdx;
    DLL_IModgfx* modgfx;

    objdata = actor->data;
    actor->unkAF |= ARROW_FLAG_8_No_Targetting;
    if (((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, &objdata->unk3B8, 3, 3) != 0) {
        return 1;
    }
    if (objdata->unkA60_1 && !objdata->unkA60_2) {
        objList = obj_get_all_of_type(OBJTYPE_25, &numObjs);
        for (i = 0; i < numObjs; i++) {
            ((DLL_Unknown*)objList[i]->dll)->vtbl->func[15].withTwoArgsCustom2(objList[i], actor->srt.transl.y + 4.0f);
        }
        objdata->unkA60_2 = 1;
    }
    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
        case 10:
            break;
        case 14:
            gDLL_6_AMSFX->vtbl->play(actor, SOUND_57, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play(actor, SOUND_9E, MAX_VOLUME, NULL, NULL, 0, NULL);
            for (partIdx = 0; partIdx != 10; partIdx++) {
                gDLL_17_partfx->vtbl->spawn(actor, PARTICLE_3C, NULL, 2, -1, NULL);
            }
            modgfx = dll_load_deferred(DLL_ID_109, 1);
            modgfx->vtbl->func0(actor, 0, 0, 2, -1, 0);
            dll_unload(modgfx);
            break;
        }
    }
    return 0;
}

// offset: 0x2174 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2174.s")

// offset: 0x21A8 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_21A8.s")

// offset: 0x21DC | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_21DC.s")

// offset: 0x2454 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2454.s")

// offset: 0x2704 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2704.s")

// offset: 0x2860 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2860.s")

// offset: 0x2D80 | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2D80.s")

// offset: 0x2F90 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2F90.s")

// offset: 0x311C | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_311C.s")

// offset: 0x31F8 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_31F8.s")

// offset: 0x3210 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_3210.s")

// offset: 0x32EC | func: 36
s32 dll_713_func_32EC(Object* self, u8 arg1) {
    DRearthwalk_Data* objData;

    objData = self->data;
    switch (arg1) {
        case 5:
            objData->unkA58 |= 0x80;
            objData->unkA62 = 0xA;
            //Set Mind Read text
            if (!gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0CF_DR_Mind_Read_messages_4)) {
                break;
            }
        // fallthrough
        case 1:
            objData->unkA62 = 2;
            func_80000450(self, self, 0x22C, 0, 0, 0);
            break;
        case 2:
            func_80000450(self, self, 0x22E, 0, 0, 0);
            objData->unkA58 &= 0xFF7F;
            //Set Mind Read text
            gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0D0_DR_Mind_Read_messages_5);
    }

    return 1;
}

// offset: 0x3420 | func: 37 | export: 20
void dll_713_func_3420(Object* self, s32 arg1, s32 arg2) {
    DRearthwalk_Data* objdata = self->data;
    if (arg1 == 1) {
        objdata->unkA56 += 12;
        func_80034B54(self, &objdata->unk394, _data_24, 1);
    }
}
