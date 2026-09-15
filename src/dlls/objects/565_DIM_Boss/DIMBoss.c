#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/gfx/modgfx.h"
#include "dll.h"
#include "sys/rand.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "sys/objtype.h"

typedef struct {
    Baddie unk0;
    u8 unk3FC[0xB8];
} DLL565_Data; //4B4

typedef struct {
    s32 unk0;
    u16 unk4;
    u16 unk6;
    Object* unk8;
    SRT unkC;
    SRT unk24;
    SRT unk3C;
    SRT unk54;
    Vec3f unk6C;
    MtxF unk78;
} DLL565_DataActual; //B8

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} DIM_BossSpit_Setup; //TODO: move to header

/*0x0*/ static u32 data_0[] = {
    0x00000001, 0x00000008, 0x00000009, 0x00000009, 0x0000000a, 0x0000000a, 0x0000000a, 0x0000000a, 
    0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000006, 0x00000006, 0x00000005, 0x00000001
};
/*0x40*/ static f32 data_40[] = {
    0.005, 0.025, 0.027, 0.03, 0.01, 0.012, 0.014, 0.016, 
    0.016, 0.014, 0.012, 0.01, 0.03, 0.027, 0.025, 0.005
};
/*0x80*/ static u32 data_80[] = {
    0x0000037d, 0x00000384
};
/*0x88*/ static u32 data_88[] = {
    0x00000383, 0x00000384, 0x00000385, 0x000003ba, 0x00000386, 0x00000423
};
/*0xA0*/ static u32 data_A0[] = {
    0x000003bb, 0x000003bc, 0x000003bd, 0x000003be
};
/*0xB0*/ static u32 data_B0 = 0x00000000;
/*0xB4*/ static u8 data_B4 = 0;
/*0xB8*/ static DLL_IModgfx* data_B8 = NULL;
/*0xBC*/ static u16 data_BC[] = {
    0x000a, 0x0019, 0x0019, 0x000a, 0x0014, 0x0014, 0x0014, 0x0000
};
/*0xCC*/ static s16 data_CC[] = {
    0x000a, 0x0014, 0x0014, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
/*0xDC*/ static s32 data_DC[] = {
    0x0000025b, 0x0000025c
};
/*0xE4*/ static s32 data_E4[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff
};
/*0xF4*/ static s16 data_F4[] = {0x0003, 0x0008};
/*0xF8*/ static s16 data_F8[] = {
    0x0008, 0x0001, 0x0001, 0x0001, 0x0001, 0x0005
};
/*0x104*/ static s16 data_104[] = {
    0x0003, 0x0004, 0x0003, 0x0005, 0x0004, 0x0005, 0x0006, 0x0000
};
/*0x114*/ static s16 data_114[] = {
    0x0005, 0x0004, 0x0003, 0x0005, 0x0004, 0x0005, 0x0006, 0x0000
};
/*0x124*/ static s16 data_124[] = {
    0x0004, 0x0005, 0x0004, 0x0005, 0x0004, 0x0005, 0x0006, 0
};

/*0x0*/ static SRT bss_0;
/*0x18*/ static SRT bss_18;
/*0x30*/ static Vec3f bss_30;
/*0x30*/ static f32 bss_3C;
/*0x40*/ static MtxF bss_40;
/*0x80*/ static ObjFSA_StateCallback bss_80[12];
/*0xB0*/ static ObjFSA_StateCallback bss_B0[6];
/*0xC8*/ static MoveLibData bss_C8;

static void dll_565_func_2AE4(Object* self, f32 x, f32 y, f32 z);
static void dll_565_func_2C20(SRT* srt, Vec3f* velocity);

// offset: 0x0 | func: 0
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_func_0.s")
#else
static s32 dll_565_func_2CE8(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_2D78(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_2E48(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_2F70(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3130(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_32C0(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3444(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3598(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_36B0(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_380C(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_394C(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3AB8(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3C40(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3CB8(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3D14(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3D3C(Object* obj, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_565_func_3EA4(Object* obj, ObjFSA_Data* fsa, f32 updateRate); //Matched but needs dll_565_func_14F8 static
static s32 dll_565_func_41A8(Object* obj, ObjFSA_Data* fsa, f32 updateRate); //Matched but needs dll_565_func_14F8 static

void dll_565_func_0(void) {
    bss_80[0] = dll_565_func_2CE8;
    bss_80[1] = dll_565_func_2D78;
    bss_80[2] = dll_565_func_2E48;
    bss_80[3] = dll_565_func_2F70;
    bss_80[4] = dll_565_func_3130;
    bss_80[5] = dll_565_func_32C0;
    bss_80[6] = dll_565_func_3444;
    bss_80[7] = dll_565_func_3598;
    bss_80[8] = dll_565_func_36B0;
    bss_80[9] = dll_565_func_380C;
    bss_80[10] = dll_565_func_394C;
    bss_80[11] = dll_565_func_3AB8;

    bss_B0[0] = dll_565_func_3C40;
    bss_B0[1] = dll_565_func_3CB8;
    bss_B0[2] = dll_565_func_3D14;
    bss_B0[3] = dll_565_func_3D3C;
    bss_B0[4] = dll_565_func_3EA4;
    bss_B0[5] = dll_565_func_41A8;
}
#endif

// offset: 0xFC | ctor
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_ctor.s")
#else
void dll_565_ctor(void* dll) {
    dll_565_func_0();
}
#endif

// offset: 0x13C | dtor
void dll_565_dtor(void* dll) { }

// offset: 0x148 | func: 1 | export: 0
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_obj_Setup.s")
#else

static int dll_565_func_9E4(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

//Matched, but needs dll_565_func_9E4 static
void dll_565_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    s32 pad;
    Baddie* baddie = self->data;
/*0xBC*/ u16 data_BC[] = {
    0x000a, 0x0019, 0x0019, 0x000a, 0x0014, 0x0014, 0x0014
};
    u8 flags = 4 | 2;
/*0xCC*/ s16 data_CC[] = {
    0x000a, 0x0014, 0x0014, 0x0000, 0x0000, 0x0000, 0x0000
};
    DLL565_DataActual* objData;

    self->unkD4 = 2;
    if (reset != 0) {
        flags = 4 | 2 | 1;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0xC, 6, 0x102, flags, 40.0f);
    self->animCallback = dll_565_func_9E4;
    baddie->unk3B6 = 0;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    baddie->fsa.logicState = 0;
    baddie->fsa.hitpoints = 3;
    baddie->unk3B6 = 0;
    self->unkAF |= 0x80 | 8;
    
    if (mainGetBits(BIT_210)) {
        baddie->unk3B6 = 4;
        self->unkDC = 1;
    }
    
    if (mainGetBits(BIT_20E)) {
        baddie->unk3B6 = 3;
    }
    
    objData = baddie->objdata;
    objData->unk0 = 0;
    objData->unk4 = 0xC0C6;
    objData->unk6 = 0;
    self->curModAnimIdLayered = -1;
    objData->unk8 = 0;
    
    data_B4 = 0;
    data_B0 = 0;    
    mainSetBits(BIT_269, 0);
    mainSetBits(BIT_26A, 0);
    mainSetBits(BIT_Tricky_Unlocked_Sidekick_Commands, 1);

    mainCreateTempDLL(DLL_ID_MOVELIB);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &bss_C8, -0x271C, 0x1C71, 6);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&bss_C8, data_BC, data_BC, 6);
    bss_C8.unk4A9 |= 8;
    
    data_B8 = dllLoad(DLL_ID_106, 1);
}
#endif

// offset: 0x3F0 | func: 2 | export: 1
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_obj_Control.s")
#else
static void dll_565_func_1178(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {}
static void dll_565_func_119C(Object* self, s32 arg1, Baddie* baddie, ObjFSA_Data* fsa) {}
static void dll_565_func_1C84(Object* self, Baddie* baddie) {}

void dll_565_obj_Control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;
    Object* target;
    Object* player;

    objSetup = (Baddie_Setup*)self->setup;
    baddie = self->data;
    player = objGetPlayer();
    
    if (self->unkDC != 0) {
        return;
    }
    
    func_80028D2C(self);
    
    if (self->unkE0 == 0) {
        self->srt.transl.x = objSetup->base.x;
        self->srt.transl.y = objSetup->base.y;
        self->srt.transl.z = objSetup->base.z;
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    
    if (baddie->unk3B2 & 2) {
        gDLL_33_BaddieControl->vtbl->func9(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
        baddie->unk3B2 &= ~2;
        self->unkAF &= ~8;
        ((DLL_Unknown*)player->dll)->vtbl->func[61].withTwoArgs(player, 1);
        gDLL_2_Camera->vtbl->set_target_flag_2(1);
        self->unkAF |= 0x80;
        if (mainGetBits(0x20C) >= 3) {
            baddie->unk3B6 = 2;
            baddie->fsa.hitpoints = 3;
            self->unkAF &= ~8;
        } else {
            baddie->unk3B6 = 1;
            baddie->fsa.hitpoints = 2;
            self->unkAF &= ~8;
        }
    }
    
    if ((baddie->unk3B6 == 0) || (baddie->unk3B6 == 3)) {
        mainSetBits(0x269, 1);
        mainSetBits(0x26A, 1);
    } else {
        mainSetBits(0x269, 0);
        mainSetBits(0x26A, 0);
        
        if (baddie->unk3B2 & 4) {
            baddie->fsa.target = objGetSidekick();
        } else {
            baddie->fsa.target = objGetPlayer();
        }

        dll_565_func_1178(self, baddie, &baddie->fsa);
        dll_565_func_119C(self, NULL, baddie, &baddie->fsa);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func1(&bss_C8, baddie->fsa.target);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &bss_C8);
        dll_565_func_1C84(self, baddie);
    }
}
#endif

// offset: 0x740 | func: 3 | export: 2
void dll_565_obj_Update(Object* self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, bss_80);
}

// offset: 0x790 | func: 4 | export: 3
void dll_565_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_obj_Print.s")
#else

static void dll_565_func_2550(Object* self, Baddie* baddie);

//Matched, but needs dll_565_func_2550 static
void dll_565_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    
    if ((visibility != 0) && (self->unkDC == 0) && (baddie->unk3B6 != 3)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        dll_565_func_2550(self, baddie);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &bss_C8, 1);
    }
}
#endif

// offset: 0x854 | func: 5 | export: 4
void dll_565_obj_Free(Object* self, s32 onlySelf) {
    Object* player;
    Baddie* baddie;

    player = objGetPlayer();
    baddie = self->data;
    
    if (player != NULL) {
        ((DLL_210_Player*)player->dll)->vtbl->func61(player, 0);
        gDLL_2_Camera->vtbl->set_target_flag_2(0);
    }
    
    self->unkAF &= ~0x80;
    camIgnoreShake();
    objFreeObjectType(self, OBJTYPE_Baddie);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
    
    mainRemoveTempDLL(DLL_ID_MOVELIB);
    
    if (data_B8 != NULL) {
        dllFree(data_B8);
    }
    data_B8 = NULL;
}

// offset: 0x9A0 | func: 6 | export: 5
u32 dll_565_obj_GetModelFlags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x9B0 | func: 7 | export: 6
u32 dll_565_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DLL565_Data);
}

// offset: 0x9C4 | func: 8 | export: 7
s16 dll_565_Func_9C4(Object* self) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x9D4 | func: 9 | export: 8
void dll_565_Func_9D4(Object* self, u8 message) {

}

// offset: 0x9E4 | func: 10
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_func_9E4.s")
#else
static void dll_565_func_1178(Object* self, Baddie* baddie, ObjFSA_Data* fsa); //MATCHED
static void dll_565_func_119C(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa); //MATCHED
static void dll_565_func_1C84(Object* self, Baddie* baddie); //MATCHED

int dll_565_func_9E4(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    s32 pad2;
    ObjFSA_Data* fsa;
    Baddie_Setup* objSetup; //44
    Baddie* baddie;
    Object* player;
    s32 i;
    
    baddie = self->data;
    fsa = &baddie->fsa;
    objSetup = (Baddie_Setup*)self->setup;
    player = objGetPlayer();
    baddie->unk3B6 = 0;
    
    if (self->unkDC != 0) {
        return 0;
    }
    
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(self, animData, &bss_C8, 1, 1);

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 1:
            gDLL_16->vtbl->func2(0);
            mainSetBits(0x1DE, 1);
            break;
        case 2:
            baddie->unk3B6 = 1;
            self->unkAF &= ~8;
            ((DLL_210_Player*)player->dll)->vtbl->func61(player, 1);
            gDLL_2_Camera->vtbl->set_target_flag_2(1);
            self->unkAF |= 0x80;
            gDLL_29_Gplay->vtbl->set_obj_group_status(0x1C, 0, 0);
            break;
        case 19:
            gDLL_29_Gplay->vtbl->set_obj_group_status(0x1C, 2, 1);
            break;
        case 20:
            gDLL_29_Gplay->vtbl->set_obj_group_status(0x1C, 2, 0);
            break;
        case 6:
            data_B0 |= 0x40004;
            break;
        case 7:
            data_B0 |= 2;
            break;
        case 8:
            lfxAction(self, objGetPlayer(), 0xF1, 0, 0, 0);
            if (mainGetBits(0x20C) >= 3) {
                gDLL_5_AMSEQ2->vtbl->set(self, 0x76, NULL, 0, NULL);
            } else {
                gDLL_5_AMSEQ2->vtbl->set(self, 0x67, NULL, 0, NULL);
            }
            break;
        case 9:
            data_B0 |= 0x40;
            break;
        case 10:
            data_B0 &= ~0x40;
            break;
        case 11:
            data_B0 |= 0x80;
            break;
        case 12:
            data_B0 &= ~0x80;
            break;
        case 13:
            data_B0 |= 0x100;
            break;
        case 14:
            data_B0 &= ~0x100;
            break;
        case 15:
            data_B0 |= 0x2009;
            break;
        case 16:
            data_B0 |= 0x8021;
            break;
        case 17:
            baddie->unk3B6 = 4;
            self->unkDC = 1;
            baddie->fsa.target = NULL;
            baddie->fsa.unk4.mode = 0;
            baddie->fsa.unk33D = 0;
            func_800267A4(self);
            self->unkAF |= 8;
            ((DLL_210_Player*)player->dll)->vtbl->func61(player, 0);
            gDLL_2_Camera->vtbl->set_target_flag_2(0);
            self->unkAF &= ~0x80;
            
            if (self->setup == NULL) {
                objFreeObject(self);
            }
            
            objSendMesg(objGetPlayer(), 0xE0000, self, NULL);
            mainSetBits(baddie->unk39E, 0);
            mainSetBits(baddie->unk39C, 1);
            mainSetBits(0x210, 1);
            break;
        case 18:
            gDLL_3_Animation->vtbl->set_camera_module(0x5B, 4, (s32)self, 0x3C);
            break;
        default:
            break;
        }
    }

    
    if (self->seqSlot != -1) {
        if (!gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1)) {
            return 1;
        }
        
        dll_565_func_1178(self, baddie, fsa);
        
        if ((baddie->unk3A0 != NO_GAMEBIT) && mainGetBits(baddie->unk3A0)) {
            gDLL_3_Animation->vtbl->func21(animData, objSetup->unk2C);
            baddie->unk3A0 = -1;
        }

        switch (baddie->unk3B4) {
        case 2:
            animData->unk7A = 0;
            dll_565_func_119C(self, animData, baddie, fsa);
            if (baddie->unk3B4 == 1) {
                baddie->fsa.logicState = 0;
                gDLL_18_objfsa->vtbl->tick(self, fsa, 1.0f, 1.0f, bss_80, bss_B0);
                animData->unk62 = 0;
            }
            break;
        case 1:
            if (gDLL_33_BaddieControl->vtbl->func12(self, animData, baddie, bss_80, bss_B0, 0)) {
                gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
            }
            break;
        case 0:
            break;
        default:
            break;
        }
    }
    
    dll_565_func_1C84(self, baddie);
    
    if (self->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        return 0;
    }
    
    return baddie->unk3B4 != 0;
}
#endif

// offset: 0x1178 | func: 11
void dll_565_func_1178(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* linked = self->linkedObject;
    if (linked != NULL) {
        linked->parent = self->parent;
    }
}

// offset: 0x119C | func: 12
void dll_565_func_119C(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa) {
    u16 sp4E;
    s16 sp4C;
    u16 sp4A;
    DLL565_DataActual* objData;
    Object* sidekick;

    objData = baddie->objdata;
    sidekick = objGetSidekick();
    func_8002674C(self);
    fsa->unk4.mode = 1;
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);
    
    if (sidekick != NULL) {
        if (objData->unk0 != 0) {

            objData->unk0 += gUpdateRate;
            if (objData->unk0 >= 0x320) {
                baddie->unk3B2 &= ~4;
                objData->unk0 = 0;
                ((DLL_ISidekick*)sidekick->dll)->vtbl->Func21(sidekick, 0, 0);
            }
        } else if ((baddie->unk3B6 == 1) && (((DLL_ISidekick*)sidekick->dll)->vtbl->Func24(sidekick))) {
            baddie->unk3B2 |= 4;
            objData->unk0 = 1;
            dll_565_func_2AE4(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
        }
    }
    
    if (data_B0 & 0x20000) {
        data_B0 &= ~0x20000;
        objData = baddie->objdata;
        dll_565_func_2C20(&objData->unkC, &objData->unk6C);
    }
    if (baddie->unk3B2 & 4) {
        data_B0 |= 8;
    } else {
        if (objData->unk8 != NULL) {
            objFreeObject(objData->unk8);
            objData->unk8 = NULL;
        }
    }
    if (baddie->unk3B6 == 1) {
        ((DLL_ISidekick*)sidekick->dll)->vtbl->EnableCommand(sidekick, Sidekick_Command_INDEX_2_Distract);
        gDLL_33_BaddieControl->vtbl->func4(self, objGetPlayer(), 8, &sp4E, &sp4C, &sp4A);
        if ((sp4E == 3) || (sp4E == 4)) {
            self->unkD4 = 1;
        } else {
            self->unkD4 = 2;
        }
    } else {
        self->unkD4 = 2;
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_80, bss_B0);
    self->animObj = baddie->unk3AC;
}

// offset: 0x14F8 | func: 13
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_func_14F8.s")
#else

//Matched, but has forced BSS reordering issues
void dll_565_func_14F8(Object* self, ObjFSA_Data* fsa) {
/*0x0*/ static SRT bss_0;
    Baddie* baddie = self->data; //74
    s32 damageType; //70
    s32 hitSphereID; //6C
    s32 hitDamage; //68
    Object* hitBy; //64
    Object* target; //60
    Object* player = objGetPlayer(); //5C
    ModelInstance* modelInst;
    Vec4f* hitSphereCoords;
    u8 wasHit; //53
/*0xE4*/ s32 data_E4[] = { 0x00000006, 0x00000069, 0x00000069, 0x000000ff }; //40, 44, 48, 4C

    wasHit = FALSE;
    damageType = func_80025F40(self, &hitBy, &hitSphereID, &hitDamage);
    if (damageType == 0) {
        return;
    }
    
    data_B0 &= ~0x40;
    if (baddie->unk3B6 == 1) {
        if (!(data_B0 & 8) || (hitSphereID != 0xA)) {
            wasHit = TRUE;
        }
    } else {
        if ((baddie->unk3B6 == 2) && ((hitSphereID != 1) || (self->animProgress < 0.3f))) {
            wasHit = TRUE;
        }
    }
    
    if (wasHit) {
        dll_amSfx->Play(self, data_DC[mathRnd(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
        modelInst = self->modelInsts[self->modelInstIdx];
        hitSphereCoords = modelInst->unk24;
        bss_0.transl.x = hitSphereCoords[hitSphereID].f[1] + gWorldX;
        bss_0.transl.y = hitSphereCoords[hitSphereID].f[2];
        bss_0.transl.z = hitSphereCoords[hitSphereID].f[3] + gWorldZ;
        gDLL_17_partfx->vtbl->spawn(self, 0x328, &bss_0, 0x200001, -1, NULL);
        
        bss_0.transl.x -= self->globalPosition.f[0];
        bss_0.transl.y -= self->globalPosition.f[1];
        bss_0.transl.z -= self->globalPosition.f[2];
        bss_0.scale = 1.0f;
        bss_0.yaw = 0;
        bss_0.pitch = 0;
        bss_0.roll = 0;
        data_E4[1] += mathRnd(0, 0x9B);
        data_E4[2] += mathRnd(0, 0x9B);
        data_B8->vtbl->func0(self, 0, &bss_0, 1, -1, data_E4);
        return;
    } 
    
    if (fsa->target == NULL) {
        target = objGetPlayer();
        if (((DLL_210_Player*)target->dll)->vtbl->func66(target, 1) != 0) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 2, 0xA, -1, -1);
            fsa->target = target;
            fsa->unk33D = 0;
        }
    }
    
    if (baddie->unk3B6 == 1) {
        if (fsa->hitpoints == 3) {
            gDLL_5_AMSEQ2->vtbl->set(self, 0x68, NULL, 0, NULL);
        } else if (fsa->hitpoints == 2) {
            gDLL_5_AMSEQ2->vtbl->set(self, 0x6C, NULL, 0, NULL);
        }
    } else if (baddie->unk3B6 == 2) {
        if (fsa->hitpoints == 3) {
            gDLL_5_AMSEQ2->vtbl->set(self, 0x77, NULL, 0, NULL);
        } else if (fsa->hitpoints == 2) {
            gDLL_5_AMSEQ2->vtbl->set(self, 0x78, NULL, 0, NULL);
        }
    }
    
    fsa->unk33A = 0;
    fsa->lastHitType = damageType;
    if (--fsa->hitpoints <= 0) {
        fsa->hitpoints = 0;
        fsa->unk33D = 0;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
        self->objhitInfo->unk58 &= ~1;
        self->unkAF |= 8;
        ((DLL_210_Player*)player->dll)->vtbl->func61(player, 0);
        gDLL_2_Camera->vtbl->set_target_flag_2(0);
        self->unkAF &= ~0x80;
        mainSetBits(0x20E, 1);

        if (baddie->unk3B6 == 1) {
            mainSetBits(0x20B, 1);
        } else if (baddie->unk3B6 == 2) {
            mainSetBits(0x266, 1);
        }
    } else if (baddie->unk3B6 == 1) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xA);
    } else {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xB);
    }

    if (baddie->unk3A8 != 0) {
        dll_amSfx->Stop(baddie->unk3A8);
        baddie->unk3A8 = 0;
    }
    
    objSendMesg(hitBy, 0xE0001, self, NULL);
}

#endif

// offset: 0x1BC8 | func: 14
static void dll_565_func_1BC8(Object* self, ObjFSA_Data* fsa, s32 bitIndex, s32 arg3, s32 arg4) {
    s32 bit;

    bit = 1 << bitIndex;
    if (fsa->unk308 & bit) {
        fsa->unk308 &= ~bit;
        dll_amSfx->Play(self, data_A0[arg3], MAX_VOLUME, NULL, NULL, 0, NULL);
        data_B0 |= arg4 | 4;
    }
}

// offset: 0x1C84 | func: 15
void dll_565_func_1C84(Object* self, Baddie* baddie) {
    s16 pad;
    s32 i;
    Vec3f sp64;
    TextureAnimator* texAnim;
    DLL565_DataActual* objData;
    LFXEmitter_Data* lfxData;

    objData = baddie->objdata;
    texAnim = objExprGetTexAnimator(self, 0, 0);
    
    dll_565_func_1BC8(self, &baddie->fsa, 2, 0, 0x200);
    dll_565_func_1BC8(self, &baddie->fsa, 1, 1, 0x400);
    dll_565_func_1BC8(self, &baddie->fsa, 4, 2, 0x800);
    dll_565_func_1BC8(self, &baddie->fsa, 3, 3, 0x1000);
    
    if (data_B0 & 8) {
        texAnim->frame = ((mathSinfInterp(objData->unk4) + 1.0f) * 126.0f) + 1;
        objData->unk6 = objData->unk0 * 8;
        objData->unk4 += objData->unk6;
    } else {
        objData->unk4 = -0x3F3A;
        objData->unk6 = 0;
        if (texAnim->frame > 0) {
            texAnim->frame >>= 1;
        }
    }
    
    if (texAnim->frame != 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0x4B0, &objData->unk54, 0x200001, -1, texAnim);

        if (objData->unk8 != NULL) {
            objData->unk8->srt.transl.x = objData->unk54.transl.x;
            objData->unk8->srt.transl.y = objData->unk54.transl.y;
            objData->unk8->srt.transl.z = objData->unk54.transl.z;
            lfxData = objData->unk8->data;
            if (lfxData->unk108 != NULL) {
                gDLL_11_Newlfx->vtbl->func6(lfxData->unk108->unk10, 0, texAnim->frame >> 1);
            }
        }
        
        if (data_B0 & 0x2000) {            
            for (i = 0; i < 100; i++) {
                gDLL_17_partfx->vtbl->spawn(self, 0x4B1, &objData->unk54, 0x200001, -1, NULL);
            }
            gDLL_17_partfx->vtbl->spawn(self, 0x4B2, &objData->unk54, 0x200001, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, 0x4B3, &objData->unk54, 0x200001, -1, NULL);
        }
    }
    
    if ((data_B0 & 0x8020) || (baddie->fsa.hitpoints < 2)) {
        if (data_B0 & 0x20) {
            for (i = 0; i < 7; i++) {
                gDLL_17_partfx->vtbl->spawn(self, 0x4B4, &objData->unk3C, 0x200001, -1, NULL);
            }
        } else if ((mathRnd(0, baddie->fsa.hitpoints) == 0) && (baddie->unk3B6 == 2)) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B4, &objData->unk3C, 0x200001, -1, NULL);
        }
        
        if (data_B0 & 0x8000) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B2, &objData->unk3C, 0x200001, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, 0x4B3, &objData->unk3C, 0x200001, -1, NULL);
        }
    }
    if (data_B0 & 0x101C0) {
        if (data_B0 & 0x40) {
            for (i = 0; i < 5; i++) {
                sp64.f[0] = mathRnd(-5, 5) * 0.1f;
                sp64.f[1] = mathRnd(-5, 5) * 0.1f;
                sp64.f[2] = mathRnd(2, 8) * -0.25f;
                mathMtxXFMF(&objData->unk78, sp64.f[0], sp64.f[1], sp64.f[2], &sp64.f[0], &sp64.f[1], &sp64.f[2]);
                gDLL_17_partfx->vtbl->spawn(self, 0x4B5, &objData->unk24, 0x200001, -1, &sp64);
            }
        }
        
        if (data_B0 & 0x80) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B5, &objData->unkC, 0x200001, -1, NULL);
        }
        
        if (data_B0 & 0x100) {
            sp64.f[0] = 0.1f;
            sp64.f[1] = 0.1f;
            sp64.f[2] = mathRnd(4, 8) * -0.1f;
            mathMtxXFMF(&objData->unk78, sp64.f[0], sp64.f[1], sp64.f[2], &sp64.f[0], &sp64.f[1], &sp64.f[2]);
            gDLL_17_partfx->vtbl->spawn(self, 0x4B6, &objData->unkC, 0x200001, -1, &sp64);
        }
        
        if (data_B0 & 0x10000) {
            sp64.f[0] = 0.0f;
            sp64.f[1] = 0.0f;
            sp64.f[2] = -5.0f;
            mathMtxXFMF(&objData->unk78, sp64.f[0], sp64.f[1], sp64.f[2], &sp64.f[0], &sp64.f[1], &sp64.f[2]);
            memcpy(&objData->unk6C, &sp64, sizeof(Vec3f));
            data_B0 |= 0x20000;
        }
    }
    
    if (data_B0 & 0x4000) {
        for (i = 0; i < 50; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B7, NULL, 1, -1, NULL);
        }
    }
    
    if (data_B0 & 1) {
        camUseShake();
        camSetShakeOffset(1.5f);
    }
    
    if (data_B0 & 0x40000) {
        camUseShake();
        camSetShakeOffset(3.0f);
    }
    
    if (data_B0 & 2) {
        camUseShake();
        camSetShakeOffset(0.0f);
    }
    
    if (data_B0 & 4) {
        mainSetBits(BIT_25E, 1);
    } else {
        mainSetBits(BIT_25E, 0);
    }
    
    data_B0 &= 0x21FF0;
}

// offset: 0x2550 | func: 16
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_func_2550.s")
#else

//Matched, but has BSS reordering issues
void dll_565_func_2550(Object* self, Baddie* baddie) {
//NOTE: local static, but needs to go before initFSACallbacks' statics... maybe requires forced BSS reordering through "prevent_bss_reordering.h"?
/*18*/ static SRT bss_18; 
    DLL565_DataActual* objData;
    s32 i;

    objData = baddie->objdata;

    if (data_B0 & 0x200) {
        objGetAttachPointWorldSpace(self, 8, &bss_18.transl.x, &bss_18.transl.y, &bss_18.transl.z, 0);
        for (i = 0; i < 0xF; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B7, &bss_18, 0x200001, -1, NULL);
        }
    }
    
    if (data_B0 & 0x400) {
        objGetAttachPointWorldSpace(self, 9, &bss_18.transl.x, &bss_18.transl.y, &bss_18.transl.z, 0);
        for (i = 0; i < 0xF; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B7, &bss_18, 0x200001, -1, NULL);
        }
    }
    
    if (data_B0 & 0x800) {
        objGetAttachPointWorldSpace(self, 0xA, &bss_18.transl.x, &bss_18.transl.y, &bss_18.transl.z, 0);
        for (i = 0; i < 0xF; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B7, &bss_18, 0x200001, -1, NULL);
        }
    }
    
    if (data_B0 & 0x1000) {
        objGetAttachPointWorldSpace(self, 0xB, &bss_18.transl.x, &bss_18.transl.y, &bss_18.transl.z, 0);
        for (i = 0; i < 0xF; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x4B7, &bss_18, 0x200001, -1, NULL);
        }
    }
    
    if (data_B0 & 0x10) {
        memcpy(&bss_40, objGetAttachPointBoneMatrix(self, 0xC), sizeof(MtxF));
        bss_40.m[3][2] = 0.0f;
        bss_40.m[3][1] = bss_40.m[3][2];
        bss_40.m[3][0] = bss_40.m[3][2];
        
        for (i = 0; i < 5; i++) {
            bss_18.transl.x = mathRnd(-0x19, 0x19);
            bss_18.transl.y = mathRnd(-0x19, 0x19);
            bss_18.transl.z = -75.0f;
            bss_30.x = bss_18.transl.x / (bss_18.transl.z * 0.5f);
            bss_30.y = bss_18.transl.y / (bss_18.transl.z * 0.5f);
            bss_30.z = 2.0f;
            mathMtxXFMF(&bss_40, bss_30.x, bss_30.y, bss_30.z, &bss_30.x, &bss_30.y, &bss_30.z);
            objGetAttachPointWorldSpace(self, 0xC, &bss_18.transl.x, &bss_18.transl.y, &bss_18.transl.z, 1);
            gDLL_17_partfx->vtbl->spawn(self, 0x4B8, &bss_18, 0x200001, -1, &bss_30);
        }
    }
    
    objData->unk24.transl.x = 0;\
    objData->unk24.transl.y = -15.0f;\
    objData->unk24.transl.z = -20.0f;
    
    objGetAttachPointWorldSpace(self, 0, &objData->unk24.transl.x, &objData->unk24.transl.y, &objData->unk24.transl.z, 1);
    objGetAttachPointWorldSpace(self, 0, &objData->unkC.transl.x, &objData->unkC.transl.y, &objData->unkC.transl.z, 0);
    objGetAttachPointWorldSpace(self, 0xC, &objData->unk3C.transl.x, &objData->unk3C.transl.y, &objData->unk3C.transl.z, 0);
    objGetAttachPointWorldSpace(self, 0xD, &objData->unk54.transl.x, &objData->unk54.transl.y, &objData->unk54.transl.z, 0);
    memcpy(&objData->unk78, objGetAttachPointBoneMatrix(self, 1), 0x40);
    
    objData->unk78.m[3][2] = 0.0f;
    objData->unk78.m[3][1] = 0.0f;
    objData->unk78.m[3][0] = 0.0f;
    
    data_B0 &= ~0x1E10;
}
#endif

// offset: 0x2AE4 | func: 17
void dll_565_func_2AE4(Object* self, f32 x, f32 y, f32 z) {
    Baddie* baddie;
    DLL565_DataActual* objData;
    LFXEmitter_Setup* lfxSetup;

    baddie = self->data;
    lfxSetup = objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    lfxSetup->base.x = x;
    lfxSetup->base.y = y;
    lfxSetup->base.z = z;
    lfxSetup->unk1E = 0x141;
    lfxSetup->unk22 = -1;
    lfxSetup->unk18 = mathRnd(-500, 500) + 1500;
    lfxSetup->unk1C = mathRnd(-500, 500) + 1500;
    lfxSetup->base.loadFlags = 1;
    lfxSetup->base.loadDistance = 0xFF;
    lfxSetup->base.fadeDistance = 0xFF;
    
    objData = baddie->objdata;
    if (objData->unk8 != NULL) {
        objFreeObject(objData->unk8);
    }
    objData->unk8 = objSetupObject(&lfxSetup->base, 4 | 1, self->mapID, -1, self->parent);
}

// offset: 0x2C20 | func: 18
void dll_565_func_2C20(SRT* srt, Vec3f* velocity) {
    DIM_BossSpit_Setup* spitSetup;
    Object* spit;

    spitSetup = (DIM_BossSpit_Setup*)objAllocSetup(sizeof(DIM_BossSpit_Setup), OBJ_DIM_BossSpit);
    spitSetup->base.x = srt->transl.x;
    spitSetup->base.y = srt->transl.y;
    spitSetup->base.z = srt->transl.z;
    spitSetup->base.loadFlags = 1;
    spitSetup->base.fadeFlags = 1;
    spitSetup->base.loadDistance = 0xFF;
    spitSetup->base.fadeDistance = 0xFF;
    spitSetup->unk1E = -1;
    spitSetup->unk20 = -1;
    spit = objSetupObject(&spitSetup->base, 4 | 1, -1, -1, NULL);
    if (spit != NULL) {
        spit->velocity.x = velocity->x;
        spit->velocity.y = velocity->y;
        spit->velocity.z = velocity->z;
    }
}

// offset: 0x2CE8 | func: 19
s32 dll_565_func_2CE8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        if (fsa->enteredAnimState) {
            objAnimSet(self, 1, 0.0f, 0);
            fsa->unk33A = 0;
        }
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        self->curModAnimIdLayered = -1;
    }
    
    return 0;
}

// offset: 0x2D78 | func: 20
s32 dll_565_func_2D78(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 2, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.018f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    
    return 0;
}

// offset: 0x2E48 | func: 21
s32 dll_565_func_2E48(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u16 idx;
    s16 sp2C;
    u16 sp2A;

    fsa->unk278 = 0.0f;
    
    if (fsa->unk33A || fsa->enteredAnimState || (self->curModAnimId == 1)) {
        gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 0x10, &idx, &sp2C, &sp2A);
        objAnimSet(self, data_0[idx], 0.0f, 0);
        fsa->animTickDelta = data_40[idx];
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 8);
    
    return 0;
}

// offset: 0x2F70 | func: 22
s32 dll_565_func_2F70(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 9, 1, -1);
    
    if (fsa->enteredAnimState) {
        self->curModAnimIdLayered = -1;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        if (mathRnd(0, 1)) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0xB, 0.0f, 0);
                fsa->unk33A = 0;
            }
            fsa->animTickDelta = 0.005f;
        } else {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0x10, 0.0f, 0);
                fsa->unk33A = 0;
            }
            fsa->animTickDelta = 0.006f;
        }
    }
    
    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        data_B0 |= 5;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 1), data_88);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 0xF0);
    
    return 0;
}

// offset: 0x3130 | func: 23
s32 dll_565_func_3130(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.005f;
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0x11, 0.0f, 0);
            fsa->unk33A = 0;
        }
        self->curModAnimIdLayered = -1;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    
    if (self->animProgress > 0.55f) {
        data_B0 &= ~0x40;
    } else if (self->animProgress > 0.35f) {
        data_B0 |= 0x40;
    }
    
    if (fsa->unk308 & 0x200) {
        data_B0 |= 0x10000;
        fsa->unk308 &= ~0x200;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 3, data_88);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 0xF0);
    
    return 0;
}

// offset: 0x32C0 | func: 24
s32 dll_565_func_32C0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.025f;
        
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0x11, 0.0f, 0);
            fsa->unk33A = 0;
        }
        
        self->curModAnimIdLayered = -1;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    
    if (self->animProgress > 0.55f) {
        data_B0 &= ~0x40;
    } else if (self->animProgress > 0.25f) {
        data_B0 |= 0x40;
    }
    
    if (fsa->unk308 & 1) {
        data_B0 |= 0x10000;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 3, data_88);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 0xF0);
    
    return 0;
}

// offset: 0x3444 | func: 25
s32 dll_565_func_3444(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.0025f;
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0x12, 0.0f, 0);
            fsa->unk33A = 0;
        }
        self->curModAnimIdLayered = -1;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    
    if ((self->animProgress > 0.95f) || fsa->unk33A) {
        return 8;
    }
    
    if (self->animProgress > 0.3f) {
        data_B0 |= 0x10;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 5, data_88);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 0xF0);
    
    return 0;
}

// offset: 0x3598 | func: 26
s32 dll_565_func_3598(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 9, 1, -1);

    if (fsa->enteredAnimState) {
        fsa->animTickDelta = 0.006f;
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0x13, 0.0f, 0);
            fsa->unk33A = 0;
        }
        
        self->curModAnimIdLayered = -1;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 1, data_88);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 0xF0);
    
    return 0;
}

// offset: 0x36B0 | func: 27
s32 dll_565_func_36B0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        self->curModAnimIdLayered = -1;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        fsa->animTickDelta = 0.005f;
        if (mathRnd(0, 1)) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0xD, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else if (fsa->enteredAnimState) {
            objAnimSet(self, 0xC, 0.0f, 0);
            fsa->unk33A = 0;
        }
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 0, data_88);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 7, 1, data_88);
    
    return 0;
}

// offset: 0x380C | func: 28
s32 dll_565_func_380C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->animTickDelta = 0.008f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0xF, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->unk308 & 1) {
        data_B0 |= 0x4004;
        dll_amSfx->Play(self, 0x3BF, MAX_VOLUME, NULL, NULL, 0, NULL);
        camUseShake();
        camSetShakeOffset(7.0f);
        mainSetBits(BIT_26B, 1);
    }
    
    return 0;
}

// offset: 0x394C | func: 29
s32 dll_565_func_394C(Object* self, ObjFSA_Data* arg1, f32 updateRate) {
    Baddie* baddie;
    DLL565_DataActual* objData;

    baddie = self->data;
    
    if (arg1->enteredAnimState) {
        data_B0 |= 0x2000;
        dll_amSfx->Play(self, 0x380, MAX_VOLUME, NULL, NULL, 0, NULL);
        camUseShake();
        camSetShakeOffset(4.0f);
        self->curModAnimIdLayered = -1;
        arg1->animTickDelta = 0.01f;
        arg1->unk278 = 0.0f;
        arg1->unk27C = 0.0f;
        
        if (arg1->enteredAnimState) {
            objAnimSet(self, 0xE, 0.0f, 0);
            arg1->unk33A = 0;
        }
        
        if (baddie->unk3B6 == 1) {
            objData = baddie->objdata;
            objData->unk0 = 0x320;
        }
    }
    
    gDLL_18_objfsa->vtbl->func12(self, arg1, 0, 1, data_80);
    
    return 0;
}

// offset: 0x3AB8 | func: 30
s32 dll_565_func_3AB8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (self->animProgress > 0.9f) {
        data_B0 &= ~0x20;
    }
    
    if (fsa->enteredAnimState) {
        data_B0 |= 0x8020;
        dll_amSfx->Play(self, 0x380, MAX_VOLUME, NULL, NULL, 0, NULL);
        camUseShake();
        camSetShakeOffset(4.0f);
        self->curModAnimIdLayered = -1;
        fsa->animTickDelta = (fsa->hitpoints + 1) * 0.002f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        if (fsa->enteredAnimState) {
            objAnimSet(self, 0x15, 0.0f, 0);
            fsa->unk33A = 0;
        }
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 0, data_80);
    
    return 0;
}

// offset: 0x3C40 | func: 31
s32 dll_565_func_3C40(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    switch (baddie->unk3B6) {
    case 1:
        return 5;
    case 2:
        return 6;
    case 4:
        return 4;
    case 0:
        return 2;
    case 3:
        return 2;
    }
    
goto label; label: //TODO: is there any way to remove this? (https://decomp.me/scratch/33s3w)
    return 2;
}

// offset: 0x3CB8 | func: 32
s32 dll_565_func_3CB8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->unk33A) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
    }
    
    return 0;
}

// offset: 0x3D14 | func: 33
s32 dll_565_func_3D14(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->unk33A) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x3D3C | func: 34
s32 dll_565_func_3D3C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* player;
    Baddie* baddie;

    player = objGetPlayer();
    baddie = self->data;
    if (fsa->enteredLogicState) {
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        ((DLL_210_Player*)player->dll)->vtbl->func61(player, 0);
        gDLL_2_Camera->vtbl->set_target_flag_2(0);
        self->unkAF &= ~ARROW_FLAG_80_Ignore_TranslateY;
        
        if (self->setup == NULL) {
            objFreeObject(self);
        }
        
        objSendMesg(objGetPlayer(), 0xE0000, self, 0);
        mainSetBits(baddie->unk39E, 0);
        mainSetBits(baddie->unk39C, 1);
    }
    
    return 0;
}

// offset: 0x3EA4 | func: 35
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_func_3EA4.s")
#else

static void dll_565_func_14F8(Object* self, ObjFSA_Data* fsa) {}

//Matched, but needs dll_565_func_14F8 (also matched, but has BSS ordering problems) static 

s32 dll_565_func_3EA4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    u16 sp3A;
    s16 sp38;
    u16 sp36;

    baddie = self->data;
    if ((fsa->unk33A != 0) || (fsa->enteredLogicState != 0)) {
        gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 0x10, &sp3A, &sp38, &sp36);
        fsa->unk33A = 0;
        if (sp36 < 90) {
            if ((sp36 > 30) && ((sp3A == 3) || (sp3A == 4) || (sp3A == 11) || (sp3A == 12))) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 9);
            }
        } else if ((sp3A == 0) || (sp3A == 15)) {
            fsa->unk33A = 0;
            if ((sp36 > 240) && (gDLL_33_BaddieControl->vtbl->func5(self, fsa, 100.0f) & 1)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_F8[mathRnd(0, 5)]);
            } else if (baddie->unk3B2 & 4) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_F4[mathRnd(0, 1)]);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
            }
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
        }
    }
    
    if ((fsa->animState != 1) && (fsa->animState != 4) && (fsa->animState != 5)) {
        bss_C8.unk4A9 |= 1;
    } else {
        bss_C8.unk4A9 &= ~1;
    }
    
    dll_565_func_14F8(self, fsa);
    
    return 0;
}

#endif

// offset: 0x41A8 | func: 36
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/565_DIM_Boss/dll_565_func_41A8.s")
#else

//Matched, but needs dll_565_func_14F8 (also matched, but has BSS ordering problems) static 

s32 dll_565_func_41A8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u16 sp36;
    s16 sp34;
    u16 sp32;

    if (fsa->unk33A || fsa->enteredLogicState) {
        gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 0x10, &sp36, &sp34, &sp32);
        fsa->unk33A = 0;
        if (sp32 < 0x5A) {
            if ((sp32 >= 0x1F) && ((sp36 == 3) || (sp36 == 4) || (sp36 == 0xB) || (sp36 == 0xC))) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 9);
            }
        } else if ((sp36 == 0) || (sp36 == 0xF)) {
            fsa->unk33A = 0;
            if ((sp32 >= 0x1AA) && (gDLL_33_BaddieControl->vtbl->func5(self, fsa, 200.0f) & 1)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_F8[mathRnd(0, 5)]);
            } else if (sp32 < 0xFA) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
            } else {
                if (data_B4 >= 7) {
                    data_B4 = 0;
                }

                switch (fsa->hitpoints) {
                case 3:
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_104[data_B4++]);
                    break;
                case 2:
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_114[data_B4++]);
                    break;
                case 1:
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, data_124[data_B4++]);
                    break;
                default:
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
                    break;
                }
            }
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
        }
    }
    
    if ((fsa->animState == 3) || (fsa->animState == 7)) {
        bss_C8.unk4A9 |= 1;
    } else {
        bss_C8.unk4A9 &= ~1;
    }
    
    dll_565_func_14F8(self, fsa);
    
    return 0;
}
#endif
