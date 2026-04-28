#include "common.h"

#include "macros.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/segment_1050.h"

typedef struct {
    s8 unk0[0x8 - 0];
    void* unk8;
    s8 unkC[0x14 - 0xC];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s8 unk44[0x58 - 0x44];
    f32 unk58; //Vec3f?
    f32 unk5C;
    f32 unk60;
    s8 unk64[0x6C - 0x64];
    u32 unk6C; //soundHandle
    s16 unk70;
    s16 unk72;
    s16 unk74;
    s16 unk76;
    s16 unk78;
    s16 unk7A;
    s16 unk7C;
    s16 unk7E;
    s16 unk80;
    s16 unk82;
    u8 unk84;
    s8 unk85[0x8A - 0x85];
    s16 unk8A;
    s16 unk8C;
    s8 unk8E;
    s8 unk8F;
    s8 unk90[0x94 - 0x90];
} DLL251_Data; //0x94

/*0x0*/ static s16 data_0[2] = {TEXTABLE_3ED, TEXTABLE_3EC}; //Krystal
/*0x4*/ static s16 data_4[2] = {TEXTABLE_3E8, TEXTABLE_3EC}; //Sabre

/*0x8*/ static u16 data_8[] = {
    SOUND_679, SOUND_679, SOUND_679, SOUND_679, 
    SOUND_679, SOUND_679, SOUND_679, SOUND_679, 
    SOUND_679, SOUND_679, SOUND_679, SOUND_679, 
    SOUND_679, SOUND_679, SOUND_679, SOUND_679, 
    SOUND_679, SOUND_679, SOUND_679, SOUND_679, 
    SOUND_679, SOUND_679, SOUND_679, SOUND_679, 
    SOUND_678, SOUND_677_Metal_Clang, SOUND_679, SOUND_679, 
    SOUND_679, SOUND_679, 0, 0, 
    0
};
/*0x4C*/ static u8 data_4C[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/*0x70*/ static u32 data_70[] = {
    0x02050206, 0x02070000
};
/*0x78*/ static DLL_IModgfx* data_78 = NULL;
/*0x7C*/ static DLL_IModgfx* data_7C = NULL;
/*0x80*/ static u32 data_80[] = {
    0x00000008, 0x000000ff, 0x000000be, 0x00000078, 
    0x00000008, 0x000000ff, 0x000000ff, 0x00000078, 
    0x00000008, 0x000000b4, 0x000000f0, 0x000000ff, 
    0x00000008, 0x000000aa, 0x000000ff, 0x000000aa
};

/*0x0*/ static Texture* bss_0;
/*0x4*/ static Texture* bss_4;
/*0x8*/ static s16* bss_8;

// offset: 0x0 | ctor
void dll_251_ctor(void* dll) {
    Texture** tex;
    s32 i;

    if (get_player()->id == 0) {
        bss_8 = data_4;
    } else {
        bss_8 = data_0;
    }
    
    if (bss_0 == NULL) {
        for (i = 0, tex = &bss_0; i < 2; ) {
            tex[i] = tex_load(bss_8[i], 0);
            i++;
        }
    }
    
    if (data_78 == NULL) {
        data_78 = dll_load(DLL_ID_106, 1, FALSE);
    }
    
    if (data_7C == NULL) {
        data_7C = dll_load(DLL_ID_168, 1, FALSE);
    }
}

// offset: 0x140 | dtor
void dll_251_dtor(void* dll) {
    Texture** tex;
    s32 i;

    if (bss_0 != NULL) {
        for (i = 0, tex = &bss_0; i < 2; ) {
            tex_free(tex[i]);
            i++;
        }
    }
    
    if (data_78 != NULL) {
        dll_unload(data_78);
    }
    
    if (data_7C != NULL) {
        dll_unload(data_7C);
    }
}

// offset: 0x1F4 | func: 0 | export: 0
void dll_251_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_setup.s")

// offset: 0x4E0 | func: 1 | export: 1
void dll_251_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_control.s")

// offset: 0x9E0 | func: 2 | export: 2
void dll_251_update(Object *self) { }

// offset: 0x9EC | func: 3 | export: 3
void dll_251_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xA04 | func: 4 | export: 4
void dll_251_free(Object* self, s32 arg1) {
    DLL251_Data* objData;
    void* temp_a1;

    objData = self->data;
    
    if (objData->unk8 != NULL) {
        mmFree(objData->unk8);
    }
    
    if (objData->unk6C != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objData->unk6C);
        objData->unk6C = 0;
    }
    
    if (objData->unk74 != 0) {
        func_80000450(self, self, 1, 0, 0, 0);
    }
    
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0xAE4 | func: 5 | export: 5
u32 dll_251_get_model_flags(Object *self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0xAF4 | func: 6 | export: 6
u32 dll_251_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL251_Data);
}

// offset: 0xB08 | func: 7 | export: 7
void dll_251_func_B08(Object* self, f32 arg1) {
    DLL251_Data* objData = self->data;
    
    if (self->id == OBJ_staff) {
        objData->unk14 = arg1;
    }
}

// offset: 0xB30 | func: 8 | export: 8
void dll_251_func_B30(Object* self) {
    DLL251_Data* objData = self->data;
    
    if (self->id == OBJ_staff) {
        objData->unk14 = -0.15f;
    }
}

// offset: 0xB64 | func: 9 | export: 9
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_func_B64.s")
#else

/*static*/ void dll_251_func_EC0(Object* self, DLL251_Data* objData, s32 arg2, s32 arg3);

void dll_251_func_B64(Object* self, s32 arg1, s32 arg2) {
    DLL251_Data* objData = self->data;

    if (objData->unk84 & 1) {
        dll_251_func_EC0(self, objData, arg2, arg1);
    }
}
#endif

// offset: 0xBC0 | func: 10 | export: 10
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_func_BC0.s")
#else

/*static*/ void dll_251_func_1FC4(Object* arg0, DLL251_Data* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void dll_251_func_BC0(Object* self, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ObjectHitInfo* objHitInfo = self->objhitInfo;
    DLL251_Data* objData = self->data;
    /*0x80*/ u32 sp50[4][4] = {
        {8, 255, 190, 120}, 
        {8, 255, 255, 120}, 
        {8, 180, 240, 255}, 
        {8, 170, 255, 170}
    };
    s32 idx;
    SRT fxTransform;
    
    if (!(self->id == OBJ_sword) && !(self->id == OBJ_staff)) {
        return;
    }
    
    dll_251_func_1FC4(self, objData, arg1, arg2, arg3, arg4);
    
    if (objHitInfo->unk9D == 0) {
        return;
    }
    
    idx = objHitInfo->unk9C;
    if (idx < 0) {
        idx = 0;
    } else if (idx > (s32)ARRAYCOUNT(data_8)) {
        idx = ARRAYCOUNT(data_8);
    }
    
    fxTransform.roll = 0;
    fxTransform.pitch = 0;
    fxTransform.yaw = 0;
    fxTransform.scale = 1.0f;
    fxTransform.transl.x = objHitInfo->unk34;
    fxTransform.transl.y = objHitInfo->unk38;
    fxTransform.transl.z = objHitInfo->unk3C;
    
    data_78->vtbl->func0(0, 1, &fxTransform, 0x401, -1, sp50[data_4C[idx] & 0xFFFF]);
    
    gDLL_6_AMSFX->vtbl->play_sound(self, data_8[idx], MAX_VOLUME, NULL, NULL, 0, NULL);
}
#endif

// offset: 0xD98 | func: 11 | export: 11
void dll_251_func_D98(Object* self) {
    DLL251_Data* objData = self->data;
    objData->unk84 ^= 1;
}

// offset: 0xDB0 | func: 12 | export: 12
void dll_251_func_DB0(Object* self, s32 arg1) {
    DLL251_Data* objData = self->data;
    
    if (arg1 != 0) {
        objData->unk84 |= 1;
    } else {
        objData->unk84 &= ~1;
    }
}

// offset: 0xDE0 | func: 13 | export: 13
void dll_251_func_DE0(Object* self, s32 arg1) {
    DLL251_Data* objData = self->data;
    objData->unk8C = arg1;
}

// offset: 0xDF0 | func: 14 | export: 14
void dll_251_func_DF0(Object* self, s32 arg1) {
    DLL251_Data* objData = self->data;

    if (arg1 > 0xFF) {
        arg1 = 0xFF;
    }
    
    objData->unk72 = arg1;
}

// offset: 0xE0C | func: 15 | export: 18
void dll_251_func_E0C(Object* self, f32 arg1, f32 arg2) {
    DLL251_Data* objData = self->data;

    objData->unk5C = arg1;
    objData->unk60 = arg2;
}

// offset: 0xE2C | func: 16 | export: 15
void dll_251_func_E2C(Object* self, s32 arg1) {
    DLL251_Data* objData = self->data;

    objData->unk72 += arg1;
    if (objData->unk72 < 0) {
        objData->unk72 = 0;
    } else if (objData->unk72 > 0xFF) {
        objData->unk72 = 0xFF;
    }
}

// offset: 0xE64 | func: 17 | export: 16
s16 dll_251_func_E64(Object* self) {
    DLL251_Data* objData = self->data;
    return objData->unk72;
}

// offset: 0xE74 | func: 18 | export: 19
s8 dll_251_func_E74(Object* self) {
    DLL251_Data* objData = self->data;
    return objData->unk8F;
}

// offset: 0xE84 | func: 19 | export: 17
void dll_251_func_E84(Object* self, Vec3f* arg1, Vec3f* arg2) {
    DLL251_Data* objData = self->data;

    arg1->x = objData->unk18;
    arg1->y = objData->unk20;
    arg1->z = objData->unk28;

    arg2->x = objData->unk30;
    arg2->y = objData->unk38;
    arg2->z = objData->unk40;
}

// offset: 0xEC0 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_func_EC0.s")

// offset: 0x1F78 | func: 21
f32 dll_251_func_1F78(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    if ((arg1 < arg0) && (arg0 < arg2)) {
        return arg3;
    } else {
        return 0.0f;
    }
}

// offset: 0x1FC4 | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_func_1FC4.s")

// offset: 0x25A8 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/251_weapons/dll_251_func_25A8.s")
