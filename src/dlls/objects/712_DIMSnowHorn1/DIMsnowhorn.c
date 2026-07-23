#include "PR/os.h"
#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/3_animation.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/496_SnowHorn.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objlib.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "types.h"
#include "prevent_bss_reordering.h"

typedef struct {
    ObjFSA_Data unk0;
    s8 unk34C[0x804 - 0x34C];
    s8 unk804;
    Vec3f unk808;
    s8 unk814[0x828 - 0x814];
    Vec3f unk828[1];
    s8 unk834[0x860 - 0x834];
    Vec3f unk860;
    s8 unk86C[0x8FC - 0x86C];
    s16 unk8FC;
    s16 unk8FE;
    s16 unk900;
    u8 unk902;
    u8 unk903;
    u8 unk904;
    u8 unk905;
    u8 unk906;
    u8 unk907;
    u8 unk908;
    u8 unk909;
    s16 unk90A;
} DIMSnowHorn_Data;

/*0x0*/ static s16 _data_0[] = {0x01c8};
/*0x4*/ static f32 _data_4[] = {
    -12, 0, -20,
    12, 0, -20,
    12, 0, 20,
    -12, 0, 20
};
/*0x34*/ static u32 _data_34[] = {
    0, 0, 0, 0
};
/*0x44*/ static f32 _data_44[] = {
    0, 0, 35, 0, 0, -35
};
/*0x5C*/ static u32 _data_5C[] = {
    0x41c80000, 0x41c80000, 0x037b037b
};
/*0x68*/ static u32 _data_68 = 0x01010101;
/*0x6C*/ static f32 _data_6C = 0.0;
/*0x70*/ static u32 _data_70 = 0x00000000;
/*0x74*/ static s16 _data_74[] = { 0x0103, 0x000b };
/*0x78*/ static f32 _data_78[] = { 0.0031, 0.005 };
/*0x80*/ static s16 _data_80[2] = {
    SnowHorn_MODANIM0_0_Idle_LOOP, 
    SnowHorn_MODANIM0_3_Walk_LOOP
};
/*0x84*/ static f32 _data_84[] = {
    0.0, 0.05, 0.03, 0.85, 0, 0, 0
};

/*0x0*/ static ObjFSA_StateCallback _bss_0[13];
/*0x34*/ static ObjFSA_StateCallback _bss_34[1];
/*0x38*/ static Texture* _bss_38[1];
/*0x3C*/ static u8 _bss_3C[0x4];
/*0x40*/ static MtxF _bss_40;

static void dll_712_func_FA0(Object* self, DIMSnowHorn_Data* objData, ObjFSA_Data* fsa);

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_0.s")

// offset: 0xC8 | ctor
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_ctor.s")
#else
void dll_712_ctor(void* dll) {
    u32 i;
    
    dll_712_func_0();

    for (i = 0; i < ARRAYCOUNT(_data_0); i++) {
        _bss_38[i] = texLoadTextureActual(_data_0[i], 0);
    }
}
#endif

// offset: 0x15C | dtor
void dll_712_dtor(void* dll) {
    u32 i;

    for (i = 0; i < ARRAYCOUNT(_data_0); i++) {
        if (_bss_38[i]) {
            texFreeTexture(_bss_38[i]);
        }
    }
}

// offset: 0x1CC | func: 1 | export: 0
void dll_712_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_setup.s")

// offset: 0x43C | func: 2 | export: 1
void dll_712_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_control.s")

// offset: 0x7C0 | func: 3
void dll_712_func_7C0(Object* self) {
    SRT xform;
    MtxF mtx;

    xform.transl.x = self->srt.transl.x;
    xform.transl.y = self->srt.transl.y;
    xform.transl.z = self->srt.transl.z;
    xform.yaw = self->srt.yaw;
    xform.pitch = self->srt.pitch;
    xform.roll = self->srt.roll;
    xform.scale = 1.0f;
    mathYprXyzMtx(&mtx, &xform);

    mathMtxXFMF(&mtx, 0.0f, -30.0f, -20.0f, 
        &self->shadow->tr.x, &self->shadow->tr.y, &self->shadow->tr.z);
}

// offset: 0x87C | func: 4
void dll_712_func_87C(Object* self, s32 arg1, s32 arg2) {
    DIMSnowHorn_Data* objData;
    ObjFSA_Data* fsa;
    Camera* cam;
    s32 sp28;

    if (arg2 != -1) {
        sp28 = (arg2 + 1) == gUpdateRate;
    } else {
        sp28 = 1;
    }
    
    cam = camGetMain();    
    
    objData = self->data;
    
    fsa = &objData->unk0;
    fsa->hitpoints = 0;
    fsa->flags &= ~0x8000;
    
    if (objData->unk902 == 2) {
        if (mainGetBits(BIT_3E2)) {
            objData->unk900 -= arg1;
        }
        
        if (mainGetBits(BIT_3E9)) {
            mainSetBits(BIT_3E9, 0);
            objData->unk900 = BIT_3E8;
        }
        
        if (objData->unk900 < 0) {
            objData->unk900 = 0;
        }
        
        fsa->xAnalogInput = joyGetStickXBuffered(0, arg2);
        fsa->yAnalogInput = joyGetStickYBuffered(0, arg2);
        fsa->unk310 = joyGetPressedBuffered(0, arg2);
        fsa->unk30C = joyGetButtonsBuffered(0, arg2);
        fsa->unk324 = cam->srt.yaw;
    } else {
        fsa->unk310 = 0;
        fsa->unk30C = 0;
        fsa->unk324 = 0;
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
    }

    fsa->flags |= 0x400000;
    if (sp28) {
        fsa->flags &= ~0x400000;
    }
    
    gDLL_18_objfsa->vtbl->tick(self, fsa, arg1, gUpdateRateF, _bss_0, _bss_34);
    dll_712_func_FA0(self, objData, fsa);
}

// offset: 0xAD4 | func: 5 | export: 2
void dll_712_update(Object *self) { }

// offset: 0xAE0 | func: 6 | export: 3
void dll_712_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_print.s")

// offset: 0xE24 | func: 7 | export: 4
void dll_712_free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_Vehicle);
}

// offset: 0xE64 | func: 8 | export: 5
u32 dll_712_get_model_flags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0xE74 | func: 9 | export: 6
u32 dll_712_get_data_size(Object *self, u32 a1) {
    return sizeof(DIMSnowHorn_Data);
}

// offset: 0xE88 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_E88.s")

// offset: 0xFA0 | func: 11
void dll_712_func_FA0(Object* self, DIMSnowHorn_Data* objData, ObjFSA_Data* fsa) {
    u8 i;
    SRT fxTransform; 
    u8 count;
    u8 bits;
    
    bits = 0;
    if (fsa->unk308 & 2) {
        bits = 1;
    }
    if (fsa->unk308 & 4) {
        bits |= 2;
    }
    if (fsa->unk308 & 8) {
        bits |= 4;
    }
    if (fsa->unk308 & 0x10) {
        bits |= 8;
    }

    i = 0;
    while (bits) {
        if (bits & 1) {
            fxTransform.transl.f[0] = objData->unk828[i].f[0];
            fxTransform.transl.f[1] = objData->unk828[i].f[1];
            fxTransform.transl.f[2] = objData->unk828[i].f[2];
            fxTransform.scale = 0.004f;

            count = mathRnd(2, 6);
            while (count) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1F9 + mathRnd(0, 1), &fxTransform, 0x10001, -1, NULL);
                count--;
            }
        }

        bits >>= 1;
        i++;
    }
}

// offset: 0x1150 | func: 12 | export: 7
s32 dll_712_func_1150(Object* self, Object* player) {
    ObjFSA_Data* fsa = &((DIMSnowHorn_Data*)self->data)->unk0;
    DIMSnowHorn_Data* objData = self->data;
    
    if (((objData->unk904 == 0) || (objData->unk904 == 2)) || (fsa->animState != 8)) {
        return 0;
    }
    
    if (self->animObj) {
        return 0; 
    } 
    
    if (((DLL_210_Player *) player->dll)->vtbl->func70(player) == 0x1E) { 
        if (joyGetPressed(0) & A_BUTTON) { 
            joyDisableButtons(0, A_BUTTON); 
            objData->unk906 |= 4;
            return 1;
        }
    }
    
    return 0;
}

// offset: 0x124C | func: 13 | export: 8
s32 dll_712_func_124C(Object* self) {
    DIMSnowHorn_Data* objData = self->data;
    return (objData->unk908 != 0) ? 1 : 2;
}

// offset: 0x1274 | func: 14 | export: 9
void dll_712_func_1274(Object* self, f32* ox, f32* oy, f32* oz) {
    DIMSnowHorn_Data* objData = self->data;
    
    *ox = objData->unk860.x;
    *oy = objData->unk860.y;
    *oz = objData->unk860.z;
}

// offset: 0x1298 | func: 15 | export: 10
s32 dll_712_func_1298(Object* self, s32 arg1) {
    DIMSnowHorn_Data* objData = self->data;

    if (objData->unk906 & 2) {
        mainSetBits(BIT_3E3, 0);
        objData->unk906 &= ~2;
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x1310 | func: 16 | export: 11
s32 dll_712_func_1310(Object* self) {
    DIMSnowHorn_Data* objData = self->data;
    return (objData->unk907 != 0) ? 2 : 1;
}

// offset: 0x1338 | func: 17 | export: 12
void dll_712_func_1338(Object* self, f32* ox, f32* oy, f32* oz) {
    MtxF mtx;
    SRT xform;

    xform.transl.x = self->srt.transl.x;
    xform.transl.y = self->srt.transl.y;
    xform.transl.z = self->srt.transl.z;
    xform.yaw = self->srt.yaw;
    xform.pitch = self->srt.pitch;
    xform.roll = self->srt.roll;
    xform.scale = 1.0f;
    mathYprXyzMtx(&mtx, &xform);
    mathMtxXFMF(&mtx, 0.0f, 80.0f, -25.0f, ox, oy, oz);
}

// offset: 0x13FC | func: 18 | export: 13
s32 dll_712_func_13FC(Object* self) {
    return 0;
}

// offset: 0x140C | func: 19 | export: 14
void dll_712_func_140C(Object* self, s32 arg1) {
    DIMSnowHorn_Data* objData = self->data;
    objData->unk902 = arg1;
}

// offset: 0x141C | func: 20 | export: 15
void dll_712_func_141C(Object* self, f32* arg1, s32* arg2) {
    *arg1 = 0.0f;
    *arg2 = 0;
}

// offset: 0x1444 | func: 21 | export: 16
f32 dll_712_func_1444(Object* self, f32* oAnimDelta) {
    DIMSnowHorn_Data* objData = self->data;
    
    if (objData->unk0.animState == 0xB) {
        *oAnimDelta = -objData->unk0.animTickDelta;
    } else {
        *oAnimDelta = 0.005f;
    }
    
    return 0.0f;
}

// offset: 0x148C | func: 22 | export: 17
s32 dll_712_func_148C(Object* self) {
    return 0;
}

// offset: 0x149C | func: 23 | export: 18
void dll_712_func_149C(Object* self) {

}

// offset: 0x14A8 | func: 24 | export: 19
void dll_712_func_14A8(Object* self, f32 arg1) {
    MtxF* jointMtx;
    f32 jointX;
    f32 jointY;
    f32 jointZ;
    SRT sp28;

    jointMtx = objGetAttachPointBoneMatrix(self, 1);
    objGetAttachPointLocalSpace(self, 1, &jointX, &jointY, &jointZ);
    
    sp28.yaw = 0;
    sp28.pitch = 0;
    sp28.roll = 0;
    sp28.transl.x = jointX;
    sp28.transl.y = jointY;
    sp28.transl.z = jointZ;
    sp28.scale = arg1 / self->def->scale;
    mathYprXyzMtx(&_bss_40,  &sp28);
    
    mathMtxCat4x3F(&_bss_40, jointMtx, &_bss_40);
    objprintSetModelMatrixOverride(&_bss_40);
}


// offset: 0x159C | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_159C.s")

// offset: 0x1860 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/712_DIMSnowHorn1/dll_712_func_1860.s")

// offset: 0x1C78 | func: 27
void dll_712_func_1C78(Object* self) {
    DIMSnowHorn_Data* objData;
    Object* player;

    objData = self->data;
    
    player = objGetPlayer();
    if (player != NULL) {
        if ((vec3Distance(&player->globalPosition, &self->globalPosition) < 300.0f) && (objData->unk902 == 0)) {
            objData->unk804 = 1;
            objData->unk808.x = player->srt.transl.x;
            objData->unk808.y = player->srt.transl.y;
            objData->unk808.z = player->srt.transl.z;
            return;
        }
    }
    
    objData->unk804 = 0;
}

// offset: 0x1D34 | func: 28
static void dll_712_func_1D34(Object* self, ObjFSA_Data* fsa) {
    fsa->speed = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->unk278 = 0.0f;
    
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
}

// offset: 0x1D68 | func: 29
s32 dll_712_func_1D68(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    switch (objData->unk904) {
    case 0:
        if (mainGetBits(BIT_F3)) {
            objData->unk906 |= 0x20;
        }
        return 2;
    case 1:
        if (mainGetBits(BIT_16F)) {
            return 9;
        } else if (mainGetBits(BIT_28)) {
            return 8;
        } else if (mainGetBits(BIT_27)) {
            return 7;
        } else {
            return 6;
        }
    case 2:
        if (mainGetBits(BIT_Horn_of_Truth)) {
            objData->unk905 = 8;
            return 4;
        } else {
            objData->unk905 = 6;
            return 4;
        }
    case 3:
        return 9;
    default:
        return 9;
    }
}

// offset: 0x1EB0 | func: 30
s32 dll_712_func_1EB0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;

    objData = self->data;
    dll_712_func_1D34(self, fsa);

    fsa->flags |= 0x200000;
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != _data_80[0]) {
            objAnimSet(self, _data_80[0], 0.0f, 0);
        }
        objData->unk8FC = mathRnd(1200, 2400);
    }
    
    objData->unk8FC -= (s32)updateRate;
    if (objData->unk8FC <= 0) {
        return -2;
    }
    
    if (!(objData->unk906 & 0x20) && (self->unkAF & 1)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x2024 | func: 31
s32 dll_712_func_2024(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 animIdx;
    DIMSnowHorn_Data* objData;

    objData = self->data;
    
    dll_712_func_1D34(self, fsa);
    fsa->flags |= 0x200000;
    
    if (fsa->enteredAnimState != 0) {
        animIdx = mathRnd(0, 1);
        fsa->animTickDelta = _data_78[animIdx];
        objAnimSet(self, _data_74[animIdx], 0.0f, 0);
    }
    
    if (fsa->unk33A != 0) {
        return -1;
    }
    
    if (!(objData->unk906 & 0x20) && (self->unkAF & ARROW_FLAG_1_Interacted)) {
        dll_anim->start_obj_sequence(5, self, -1);
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x2174 | func: 32
s32 dll_712_func_2174(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    dll_712_func_1D34(self, fsa);
    fsa->flags |= 0x200000;
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != _data_80[0]) {
            objAnimSet(self, _data_80[0], 0.0f, 0);
        }
        objData->unk8FC = mathRnd(1200, 2400);
    }
    
    objData->unk8FC -= (s32) updateRate;
    if (objData->unk8FC <= 0) {
        return -4;
    }
    
    if (self->unkAF & 1) {
        if (mainGetBits(BIT_Horn_of_Truth)) {
            objData->unk905 = 8;
        }
        
        gDLL_3_Animation->vtbl->start_obj_sequence(objData->unk905, self, -1);

        if (objData->unk905 == 6) {
            objData->unk905++;
        }
        
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x22FC | func: 33
s32 dll_712_func_22FC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;
    s32 animIdx;

    objData = self->data;
    
    dll_712_func_1D34(self, fsa);
    
    fsa->flags |= 0x200000;
    if (fsa->enteredAnimState) {
        animIdx = mathRnd(0, 1);
        fsa->animTickDelta = _data_78[animIdx];
        objAnimSet(self, _data_74[animIdx], 0.0f, 0);
    }
    
    if (fsa->unk33A) {
        return -3;
    }
    
    if (self->unkAF & 1) {
        if (mainGetBits(BIT_Horn_of_Truth)) {
            objData->unk905 = 8;
        }
        
        dll_anim->start_obj_sequence(objData->unk905, self, -1);
        if (objData->unk905 == 6) {
            objData->unk905++;
        }
        
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x2470 | func: 34
s32 dll_712_func_2470(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;

    dll_712_func_1D34(self, fsa);
    fsa->flags |= 0x200000;
    objData = self->data;
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != 0x13) {
            objAnimSet(self, 0x13, 0.0f, 0);
        }
    }
    
    if (mainGetBits(BIT_172)) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    } else {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        objData->unk905 = 0;
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        mainSetBits(BIT_Tricky_Learned_Flame, 1);
        mainSetBits(BIT_27, 1);
        joyDisableButtons(0, A_BUTTON);
    }
    
    return 0;
}

// offset: 0x25E4 | func: 35
s32 dll_712_func_25E4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;

    dll_712_func_1D34(self, fsa);
    fsa->flags |= 0x200000;
    
    objData = self->data;
    
    self->unkAF &= ~8;
    
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != 0x13) {

            objAnimSet(self, 0x13, 0.0f, 0);
        }
    }
    
    if (self->unkAF & ARROW_FLAG_4_Highlighted) {
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_DIM_Alpine_Roots)) {
            objData->unk905 = 2;
            gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
            mainSetBits(BIT_28, 1);
            mainSetBits(BIT_DIM_Alpine_Roots, mainGetBits(BIT_DIM_Alpine_Roots) - 1);
            joyDisableButtons(0, A_BUTTON);
        } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
            objData->unk905 = 1;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            joyDisableButtons(0, A_BUTTON);
        }
    }
    
    return 0;
}

// offset: 0x27D4 | func: 36
s32 dll_712_func_27D4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData;
    s8 value;

    dll_712_func_1D34(self, fsa);
    
    fsa->flags |= 0x200000;
    
    objData = self->data;
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (self->curModAnimId != 0x13) {
            objAnimSet(self, 0x13, 0.0f, 0);
        }
    }
    
    if (self->unkAF & ARROW_FLAG_4_Highlighted) {
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_DIM_Alpine_Roots)) {
            objData->unk905 = 4;
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            mainSetBits(BIT_16F, 1);
            
            value = mainGetBits(BIT_DIM_Alpine_Roots);
            if (--value < 0) {
                value = 0;
            }
            mainSetBits(BIT_DIM_Alpine_Roots, value);
        } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
            objData->unk905 = 3;
            gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
            joyDisableButtons(0, A_BUTTON);
        }
    }
    
    return 0;
}

// offset: 0x29B8 | func: 37
s32 dll_712_func_29B8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    dll_712_func_1D34(self, fsa);
    
    fsa->flags |= 0x200000;

    if (fsa->enteredAnimState) {
        fsa->animStateTime = 0;
        fsa->animTickDelta = 0.005f;
        fsa->unk2B0 = 8.0f;
        if (self->curModAnimId != _data_80[0]) {
            objAnimSet(self, _data_80[0], 0.0f, 0);
        }
    }
    
    if (((self->curModAnimId == 0x209) || (self->curModAnimId == 0x20A)) && (fsa->unk33A != 0)) {
        objAnimSet(self, _data_80[0], 0.0f, 0);
        fsa->animTickDelta = 0.005f;
    }
    
    if (fsa->analogInputPower < 0.05f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0;
    }
    
    if ((fsa->prevAnalogInputPower > 0) && (fsa->analogInputPower > 0) && (fsa->unk328 >= objData->unk8FE)) {
        return 0xB;
    }
    if ((fsa->prevAnalogInputPower > 0.1f) && (fsa->analogInputPower > 0.1f) && (fsa->unk328 < objData->unk8FE)) {
        return 0xC;
    }
    
    if (fsa->unk310 & 0x8000) {
        return 0xD;
    } else {
        return 0;
    }
}

// offset: 0x2BA0 | func: 38
s32 dll_712_func_2BA0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;
    
    fsa->flags |= 0x200000;
    self->unkAF |= 8;
    
    switch (self->curModAnimId) {
    case 0x206:
        if (fsa->unk33A) {
            if (fsa->animTickDelta > 0.0f) {
                objAnimSet(self, 0x205, 0.0f, 0);
                fsa->animTickDelta = 0.005f;
            } else {
                return 9;
            }
        }
        
        if (objData->unk900 != 0) {
            if ((fsa->animTickDelta > 0.0f) && ((fsa->unk310 != 0) || (fsa->xAnalogInput != 0.0f) || (fsa->yAnalogInput != 0.0f))) {
                fsa->animTickDelta = -fsa->animTickDelta;
            }
        }

        break;
    case 0x205:
        if ((objData->unk900 != 0) && ((fsa->unk310 != 0) || (fsa->xAnalogInput != 0.0f) || (fsa->yAnalogInput != 0.0f))) {
            objAnimSet(self, 0x207, 0.0f, 0);
            fsa->animTickDelta = 0.014f;
        }
        break;
    case 0x207:
        if (fsa->unk33A != 0) {
            return 9;
        }
        break;
    default:
        objAnimSet(self, 0x206, 0.0f, 0);
        fsa->animTickDelta = 0.014f;
        break;
    }

    return 0;
}

// offset: 0x2D90 | func: 39
s32 dll_712_func_2D90(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DIMSnowHorn_Data* objData = self->data;

    fsa->flags |= 0x200000;
    if ((fsa->unk328 < objData->unk8FE) || (fsa->analogInputPower == 0.0f)) {
        return 9;
    }

    if (fsa->unk32A < -0xAF) {
        fsa->unk32A = -fsa->unk32A;
    }
    
    if ((fsa->unk32A > 0) && (self->curModAnimId != 0x201)) {
        objAnimSet(self, 0x201, 0.0f, 0);
    } else if ((fsa->unk32A <= 0) && (self->curModAnimId != 0x200)) {
        objAnimSet(self, 0x200, 0.0f, 0);
    }
    fsa->animTickDelta = 0.012f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 8);
    
    if (fsa->unk310 & 0x8000) {
        return 0xD;
    } else {
        return 0;
    }
}

// offset: 0x2EEC | func: 40
s32 dll_712_func_2EEC(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    u8 one;
    DIMSnowHorn_Data *objData = self->data;
    f32 temp_fv0;
    f32 var_fa0;
    f32 var_fv1;
    f32 animProgress;
    s16 curModAnimId;
    f32 new_var2;
    s32 animCondition1;
    s32 returnValue;
    s32 animCondition2;
    s32 animIndex;
    f32 *temp_v0;

    fsa->flags |= 0x200000;

    if (fsa->enteredAnimState){
        self->srt.yaw += fsa->unk32A * 0xB6;
        fsa->unk328 = 0;
        fsa->unk32A = 0;
    }

    if (fsa->analogInputPower < 0.05f){
        fsa->analogInputPower = 0.0f;
        fsa->unk328 = 0;
        fsa->unk32A = 0;
    }

    if (fsa->unk328 < 90){
        self->srt.yaw += ((fsa->unk32A * arg2) / 36.0f) * 182.0f;
    } else {
        return 9;
    }

    var_fa0 = fsa->analogInputPower;
    if (var_fa0 < 0.0f){
        var_fa0 = 0.0f;
    }
    if (var_fa0 > 1.0f){
        var_fa0 = 1.0f;
    }

    //Handle SnowHorn running out of energy 
    //(@bug: causes a softlock since there's no fail state cutscene to reset you back to before the blizzard)
    if (objData->unk900 == 0){
        var_fa0 = 0; //set speed to zero?
    }

    var_fv1 = var_fa0 * 0.85f;
    if (var_fv1 < 0){
        var_fv1 = 0;
    }

    fsa->speed += ((var_fv1 - fsa->speed) / fsa->unk2B0) * arg2;
    if (self->srt.pitch > 0){
        var_fv1 -= mathSinfInterp(self->srt.pitch) * 0.3f;
    } else {
        var_fv1 -= mathSinfInterp(self->srt.pitch) * 0.15f;
    }
    if (var_fv1 < _data_84[2]){
        var_fv1 = _data_84[2];
    }

    fsa->unk278 += ((var_fv1 - fsa->unk278) / fsa->unk2B0) * arg2;

    animCondition1 = 0;
    one = 1;

    animProgress = self->animProgress;

    for (animIndex = 0; self->curModAnimId != _data_80[animIndex] && animIndex < 2; animIndex++);

    if (animIndex > 1){
        animIndex = 0;
    }

    if (self->curModAnimId == SnowHorn_MODANIM2_8_Walk_Intro){
        animIndex = 1;
    }

    temp_v0 = &_data_84[animIndex * 2];
    if (fsa->speed < temp_v0[0]){
        animCondition1 = 1;
        if (animIndex == 1){
            return 9;
        }
        animIndex -= one;
    } else if (temp_v0[1] <= fsa->speed){
        animCondition1 = 1;
        if (animIndex == 0){
            animProgress = 0.0f;
        }
        animIndex++;
    }

    animCondition2 = 1;
    if (fsa->unk33A && self->curModAnimId == SnowHorn_MODANIM2_8_Walk_Intro){
        animCondition1 = 1;
        animCondition2 = 0;
    }

    if (animCondition1){
        if ((animIndex == 1) && animCondition2){
            objAnimSet(self, SnowHorn_MODANIM2_8_Walk_Intro, animProgress, 0);
        } else {
            objAnimSet(self, _data_80[animIndex], animProgress, 0);
        }
    }
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);

    if (fsa->unk310 & 0x8000){
        return FSA_NEXTSTATE_SYNC(12);
    } else {
        return 0;
    }
}

// offset: 0x32C0 | func: 41
s32 dll_712_func_32C0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    ObjectHitInfo* objHits;
    DIMSnowHorn_Data* objData;

    objData = self->data;
    objHits = self->objhitInfo;
    
    fsa->flags |= 0x200000;
    
    dll_712_func_1D34(self, fsa);
    
    if (fsa->enteredAnimState) {
        objData->unk906 &= ~8;
        objHits->unk58 |= 0x200;
        objAnimSet(self, 0x204, 0.0f, 0);
        fsa->animTickDelta = 0.013f;
    }
    
    if ((objHits->unk58 & 0x200) && (objHits->unk9D & 2)) {
        objData->unk906 |= 8;
    }
    if (objData->unk906 & 8) {
        objHits->unk5F = 0;
        objHits->unk60 = 0;
        objHits->unk58 &= ~0x200;
    } else {
        objHits->unk5F = 0xB;
        objHits->unk60 = 1;
        objHits->unk58 |= 0x200;
    }
    
    if (self->animProgress > 0.9f) {
        return 9;
    } else {
        return 0;
    }
}

// offset: 0x3430 | func: 42
s32 dll_712_func_3430(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x3448 | func: 43 | export: 20
void dll_712_func_3448(Object* self, ObjFSA_Data* fsa, f32 updateRate) {

}

