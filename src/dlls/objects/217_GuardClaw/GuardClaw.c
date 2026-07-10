#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/gfx/animseq.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

typedef void (*SomeCallback)(Gfx**, Mtx**, Vertex**, Triangle**, s32, Object*);

typedef struct {
/*00*/ SomeCallback unk0;
/*04*/ s32 unk4;
/*08*/ f32 unk8;
/*0C*/ s8 unkC;
/*0D*/ u8 unkD;
/*10*/ f32 unk10;
/*14*/ u8 unk14;
} GuardClaw_Data;

/*0x0*/ static u32 data_0[] = {
    SOUND_35F_GuardClaw_Step, 
    SOUND_360_GuardClaw_Step2, 
    SOUND_35E_SharpClaw_Staff_Swing
};
/*0xC*/ static u32 data_C[] = {
    SOUND_35D_GuardClaw_Yawn, 
    SOUND_361_GuardClaw_Go_Away, 
    SOUND_366_GuardClaw_Rawr, 
    SOUND_367_GuardClaw_SharpClaw_Only, 
    SOUND_362_GuardClaw_Hah, 
    SOUND_363_GuardClaw_Hey_You, 
    SOUND_365_GuardClaw_Come_Here
};
/*0x28*/ static s32 data_28[] = { // hitAnimStateMap
/*0*/ 9, 
/*1*/ 9, 
/*2*/ 9, 
/*3*/ 8, 
/*4*/ 9, 
/*5*/ 8, 
/*6*/ 9, 
/*7*/ 8, 
/*8*/ 9, 
/*9*/ 8, 
/*10*/ 9, 
/*11*/ 8, 
/*12*/ 9, 
/*13*/ 8, 
/*14*/ 0, 
/*15*/ 8, 
/*16*/ 9, 
/*17*/ 8, 
/*18*/ 9, 
/*19*/ 8, 
/*20*/ 9, 
/*21*/ 8, 
/*22*/ 8, 
/*23*/ 9, 
/*24*/ 8, 
/*25*/ 8, 
/*26*/ 8, 
/*27*/ 8
};
/*0x98*/ static s8 data_98[] = { // hitDamageMap
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    0, 0, 0, 0
};
/*0xB4*/ static u32 data_B4[] = {
    0x00000000, 0x00000000, 0x00000000, 
    0x41c80000, 0x00000000, 0x00000000, 
    0x00000000
};

/*0x0*/ static ObjFSA_StateCallback bss_0[14];
/*0x38*/ static ObjFSA_StateCallback bss_38[4];

static int GuardClaw_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8);

static void GuardClaw_func_1128(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void GuardClaw_func_1520(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void GuardClaw_func_C34(Object* self, Baddie* baddie, ObjFSA_Data* fsa, s32 arg3);
static void GuardClaw_func_D80(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

static s32 GuardClaw_func_1760(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_17E0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_1860(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_191C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_1AD4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_1C8C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_1DD8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_1F24(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_1FF8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_20E0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_21C8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_2564(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_25E4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_2664(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 GuardClaw_func_26E0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_2890(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_2C6C(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 GuardClaw_func_2D8C(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void GuardClaw_func_0(void) {
    bss_0[0] = GuardClaw_func_1760;
    bss_0[1] = GuardClaw_func_17E0;
    bss_0[2] = GuardClaw_func_1860;
    bss_0[3] = GuardClaw_func_191C;
    bss_0[4] = GuardClaw_func_1AD4;
    bss_0[5] = GuardClaw_func_1C8C;
    bss_0[6] = GuardClaw_func_1DD8;
    bss_0[7] = GuardClaw_func_1F24;
    bss_0[8] = GuardClaw_func_1FF8;
    bss_0[9] = GuardClaw_func_20E0;
    bss_0[10] = GuardClaw_func_21C8;
    bss_0[11] = GuardClaw_func_2564;
    bss_0[12] = GuardClaw_func_25E4;
    bss_0[13] = GuardClaw_func_2664;

    bss_38[0] = GuardClaw_func_26E0;
    bss_38[1] = GuardClaw_func_2890;
    bss_38[2] = GuardClaw_func_2C6C;
    bss_38[3] = GuardClaw_func_2D8C;
}

// offset: 0xFC | ctor
void GuardClaw_ctor(void* dll) {
    GuardClaw_func_0();
}

// offset: 0x13C | dtor
void GuardClaw_dtor(void* dll) { }

// offset: 0x148 | func: 1 | export: 0
void GuardClaw_setup(Object* self, Baddie_Setup* setup, s32 reset) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata;
    u8 var_v0;
    
    var_v0 = 6;
    if (reset != 0) {
        var_v0 |= 1;
    }
    if (!(setup->unk2B & 0x20)) {
        var_v0 |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 0xE, 4, 0x102, var_v0, 20.0f);
    self->animCallback = GuardClaw_anim_callback;
    baddie->fsa.logicState = 0;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 0);
    baddie->unk3B6 = 0;
    objdata = baddie->objdata;
    bzero(objdata, sizeof(GuardClaw_Data));
    objdata->unkC = setup->unk28;
    if (self->id == OBJ_SCguardclaw) {
        if (mainGetBits(BIT_2B9) != 0) {
            baddie->unk3E2 = 0;
        }
    } else {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
    objdata->unk0 = NULL;
}

// offset: 0x2A0 | func: 2 | export: 1
void GuardClaw_control(Object* self) {
    Baddie* baddie = self->data;
    Baddie_Setup* setup = (Baddie_Setup*)self->setup;

    if (self->unkDC == 0) {
        if (self->unkE0 == 0) {
            self->srt.transl.x = setup->base.x;
            self->srt.transl.y = setup->base.y;
            self->srt.transl.z = setup->base.z;
            gDLL_3_Animation->vtbl->start_obj_sequence(setup->unk2E, self, -1);
            self->unkE0 = 1;
            return;
        }
        if (baddie->unk3B2 & 2) {
            GuardClaw_func_C34(self, baddie, &baddie->fsa, 0);
            baddie->unk3B2 &= ~2;
        }
        if (self->id == OBJ_SCguardclaw) {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            if (mainGetBits(BIT_SC_Platform_Rises_Totem_Challenge_Begins) != 0) {
                self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
            } else {
                self->unkAF |= ARROW_FLAG_10_Greyed_Out;
            }
            if (self->unkAF & ARROW_FLAG_1_Interacted) {
                if (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_SC_Platform_Rises_Totem_Challenge_Begins) != 0) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                    mainSetBits(BIT_2B9, 1);
                    baddie->unk3E2 = 0;
                } else {
                    self->unkAF &= ~ARROW_FLAG_1_Interacted;
                    gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                }
            }
        }
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1) == 0) {
            baddie->unk3B6 = 0;
            return;
        }
        GuardClaw_func_D80(self, baddie, &baddie->fsa);
        if ((baddie->fsa.targetDist < (f32) baddie->unk3E2) || (baddie->unk3B0 & 0x10)) {
            GuardClaw_func_1128(self, baddie, &baddie->fsa);
        } else {
            GuardClaw_func_1520(self, baddie, &baddie->fsa);
        }
    }
}

// offset: 0x550 | func: 3 | export: 2
void GuardClaw_update(Object* self) {
    Baddie* baddie = self->data;
    gDLL_18_objfsa->vtbl->func2(self, &baddie->fsa, bss_0);
}

// offset: 0x5A0 | func: 4 | export: 3
void GuardClaw_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata;
    s32 _pad;
    
    if (visibility != 0) {
        objdata = baddie->objdata;
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (baddie->unk3B2 & 0x60) {
            if (baddie->unk3B2 & 0x20) {
                gDLL_32->vtbl->func2(self, 0x330, &objdata->unk10);
                gDLL_32->vtbl->func2(self, 0x330, &objdata->unk10);
                baddie->unk3B2 &= ~0x20;
            }
            gDLL_32->vtbl->func2(self, 0x32F, &objdata->unk10);
        }
        if (objdata->unk0 != NULL) {
            objdata->unk0(gdl, mtxs, vtxs, pols, objdata->unk4, self);
        }
    }
}

// offset: 0x708 | func: 5 | export: 4
void GuardClaw_free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    obj_free_object_type(self, OBJTYPE_Baddie);
    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0x7AC | func: 6 | export: 5
u32 GuardClaw_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x7BC | func: 7 | export: 6
u32 GuardClaw_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(Baddie) + sizeof(GuardClaw_Data);
}

// offset: 0x7D0 | func: 8 | export: 7
s16 GuardClaw_get_fsa_state(Object* self) {
    Baddie* baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x7E0 | func: 9 | export: 8
void GuardClaw_send_message(Object* self, u8 arg1) {
    Baddie* baddie = self->data;
    
    switch (arg1) {
    case 0x80:
        baddie->unk3B0 |= 0x80;
        break;
    case 0x82:
        baddie->fsa.hitpoints = 0;
        break;
    case 0x81:
        baddie->unk3B0 &= ~0x80;
        gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, 13);
        break;
    default:
        STUBBED_PRINTF("BADDIE:GuardClaw Unknown message [%d]\n", arg1);
        break;
    }
}

// offset: 0x884 | func: 10 | export: 10
void GuardClaw_func_884(Object* self, s32 arg1) {
    Baddie* baddie = self->data;

    if (arg1 != 0) {
        baddie->unk3B0 |= 0x10;
    } else {
        baddie->unk3B0 &= ~0x10;
    }
}

// offset: 0x8B4 | func: 11
static int GuardClaw_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    Baddie_Setup* setup = (Baddie_Setup*)actor->setup;
    Baddie* baddie = actor->data;
    GuardClaw_Data* objdata;
    s32 i;
    
    if (actor->unkDC != 0) {
        return 0;
    }
    objdata = baddie->objdata;
    if (objdata->unk10 > 0.0f) {
        objdata->unk10 += (gUpdateRateF * 0.01f);
        if (objdata->unk10 > 2.0f) {
            objdata->unk10 = 0.0f;
            baddie->unk3B2 &= ~0x60;
        }
    }
    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
        default:
            break;
        case 1:
            baddie->fsa.hitpoints = 0;
            break;
        case 2:
            objdata->unk10 = 1.0f;
            baddie->unk3B2 |= 0x60;
            break;
        }
    }
    if (actor->seqSlot != -1) {
        if (gDLL_33_BaddieControl->vtbl->func11(actor, baddie, 1) == 0) {
            return 1;
        }
        GuardClaw_func_D80(actor, baddie, &baddie->fsa);
        if ((baddie->unk3A0 != -1) && (mainGetBits(baddie->unk3A0) != 0)) {
            gDLL_3_Animation->vtbl->func21(animObjData, setup->unk2C);
            baddie->unk3A0 = -1;
        }
        switch (baddie->unk3B4) {
            case 2:
                animObjData->unk7A = 0;
                GuardClaw_func_1128(actor, baddie, &baddie->fsa);
                if (baddie->unk3B4 == 1) {
                    baddie->fsa.logicState = 0;
                    gDLL_18_objfsa->vtbl->tick(actor, &baddie->fsa, 1.0f, 1.0f, bss_0, bss_38);
                    animObjData->unk62 = 0;
                }
                break;
            case 1:
                if (gDLL_33_BaddieControl->vtbl->func12(actor, animObjData, baddie, bss_0, bss_38, 0) != 0) {
                    gDLL_33_BaddieControl->vtbl->func10(actor, &baddie->fsa, 0.0f, -1);
                }
                break;
            case 0:
            default:
                animObjData->unk7A = -1;
                animObjData->unk7A &= ~0x40;
                GuardClaw_func_1520(actor, baddie, &baddie->fsa);
                break;
        }
    }
    if (actor->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        return 0;
    }
    return baddie->unk3B4 != 0;
}

// offset: 0xC34 | func: 12
static void GuardClaw_func_C34(Object* self, Baddie* baddie, ObjFSA_Data* fsa, s32 arg3) {
    if (arg3 == 0) {
        fsa->unk4.mode = DLL27MODE_DISABLED;
        fsa->logicState = 0;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        mainSetBits(baddie->unk39E, 0);
    } else if (arg3 == 1) {
        fsa->unk4.mode = DLL27MODE_1;
        fsa->logicState = 1;
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        mainSetBits(baddie->unk39E, 1);
    } else if (arg3 == 2) {
        fsa->unk4.mode = DLL27MODE_1;
        fsa->logicState = 2;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        mainSetBits(baddie->unk39E, 1);
    } else {
        fsa->unk4.mode = DLL27MODE_1;
        fsa->logicState = 3;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        mainSetBits(baddie->unk39E, 1);
    }
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_27->vtbl->reset(self, &baddie->fsa.unk4);
}

// offset: 0xD80 | func: 13
static void GuardClaw_func_D80(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    GuardClaw_Data* objdata = baddie->objdata;
    ObjSetup* setup = self->setup;
    f32 vec[3];
    u8 sp4B;
    Object* sidekick;
    Object* player;
    
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);
    sidekick = get_sidekick();
    if (sidekick != NULL) {
        sp4B = ((DLL_ISidekick*)sidekick->dll)->vtbl->func24(sidekick);
    } else {
        sp4B = 0;
    }
    if (sp4B && (objdata->unkD == 0)) {
        GuardClaw_send_message(self, 0x80);
    } else if (!sp4B && (objdata->unkD != 0)) {
        GuardClaw_send_message(self, 0x81);
    }
    objdata->unkD = sp4B;
    baddie->unk3B0 &= ~0x40;
    player = get_player();
    if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == 8) {
        baddie->unk3B0 |= 0x40;
    } else if (baddie->unk3B0 & 0x80) {
        fsa->target = get_sidekick();
        if (fsa->target == NULL) {
            fsa->target = get_player();
        }
    } else {
        fsa->target = get_player();
    }
    if (baddie->unk3B0 & 1) {
        vec[0] = fsa->target->globalPosition.x - setup->x;
        vec[1] = fsa->target->globalPosition.y - setup->y;
        vec[2] = fsa->target->globalPosition.z - setup->z;
    } else {
        vec[0] = fsa->target->globalPosition.x - self->globalPosition.x;
        vec[1] = fsa->target->globalPosition.y - self->globalPosition.y;
        vec[2] = fsa->target->globalPosition.z - self->globalPosition.z;
    }
    fsa->targetDist = sqrtf(SQ(vec[0]) + SQ(vec[1]) + SQ(vec[2]));
    if (!(baddie->unk3B0 & 0x20)) {
        if (fsa->targetDist < (2.0f * (f32) baddie->unk3E2)) {
            if (objdata->unk14 == 0) {
                objdata->unk14 = 1;
                gDLL_5_AMSEQ2->vtbl->set(self, 0x132, NULL, 0, NULL);
            }
        } else if (objdata->unk14 != 0) {
            objdata->unk14 = 0;
            gDLL_5_AMSEQ2->vtbl->set(self, (u16) baddie->unk3A6, NULL, 0, NULL);
        }
    }
    func_800328F0(self, &baddie->unk3BC, fsa->unk278);
    func_80032A08(self, &baddie->unk3BC);
}

// offset: 0x1128 | func: 14
static void GuardClaw_func_1128(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    GuardClaw_Data* objdata = baddie->objdata;
    ObjSetup* setup = self->setup;
    Object* sidekick = get_sidekick();
    s32 _pad;
    f32 sp4C;
    f32 sp48;

    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, NULL, baddie->unk39E, data_28, data_98, 1, &baddie->unk3A8, NULL) >= 2) {
        gDLL_6_AMSFX->vtbl->play(self, data_C[4], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    if (baddie->unk3B0 & 0x50) {
        if (fsa->logicState != 3) {
            GuardClaw_func_C34(self, baddie, fsa, 3);
        }
        baddie->unk3BC.aimIsActive = 0;
    } else if (baddie->unk3B0 & 0x80) {
        if (fsa->logicState != 2) {
            GuardClaw_func_C34(self, baddie, fsa, 2);
        }
        baddie->unk3BC.aimIsActive = 0;
    } else {
        if (fsa->logicState != 1) {
            GuardClaw_func_C34(self, baddie, fsa, 1);
        }
        if (baddie->unk3B0 & 1) {
            sp4C = setup->x - (fsin16_precise(self->srt.yaw) * (f32) objdata->unkC);
            sp48 = setup->z - (fcos16_precise(self->srt.yaw) * (f32) objdata->unkC);
        } else {
            sp4C = self->srt.transl.x - (fsin16_precise(self->srt.yaw) * (f32) objdata->unkC);
            sp48 = self->srt.transl.z - (fcos16_precise(self->srt.yaw) * (f32) objdata->unkC);
        }
        objdata->unk8 = gDLL_33_BaddieControl->vtbl->func2(self, sp4C, sp48, (f32) baddie->unk3E2, get_player());
        baddie->unk3BC.aimIsActive = 1;
    }
    baddie->unk3BC.headAimX = fsa->target->globalPosition.x;
    baddie->unk3BC.headAimY = fsa->target->globalPosition.y;
    baddie->unk3BC.headAimZ = fsa->target->globalPosition.z;
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_38);
    self->animObj = baddie->unk3AC;
    if (sidekick != NULL) {
        ((DLL_ISidekick*)sidekick->dll)->vtbl->enable_command(sidekick, Sidekick_Command_INDEX_2_Distract);
    }
}

// offset: 0x1520 | func: 15
static void GuardClaw_func_1520(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    GuardClaw_Data* sp5C = baddie->objdata;
    ObjSetup* sp58 = self->setup;
    s32 _pad;
    f32 sp50;
    f32 temp;
    f32 temp_fv0;
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, NULL, baddie->unk39E, data_28, data_98, 1, &baddie->unk3A8, NULL) >= 2) {
        gDLL_6_AMSFX->vtbl->play(self, data_C[4], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    if ((fsa->unk33A != 0) && (fsa->logicState != 0)) {
        GuardClaw_func_C34(self, baddie, fsa, 0);
    }
    sp50 = fsin16_precise(self->srt.yaw + 0x4000);
    temp_fv0 = fcos16_precise(self->srt.yaw + 0x4000);
    temp = -((self->srt.transl.x * sp50) + (self->srt.transl.z * temp_fv0));
    sp5C->unk8 = -((sp58->x * sp50) + (temp_fv0 * sp58->z) + temp);
    baddie->unk3BC.aimIsActive = 0;
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, bss_0, bss_38);
    self->animObj = baddie->unk3AC;
}

// offset: 0x1760 | func: 16 | state 0
static s32 GuardClaw_func_1760(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 4, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.005f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x17E0 | func: 17 | state 1
static s32 GuardClaw_func_17E0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.005f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x1860 | func: 18 | state 2
static s32 GuardClaw_func_1860(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 6, 0.0f, 0);
        fsa->unk33A = 0;
    }
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 0, data_C);
    fsa->animTickDelta = 0.005f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x191C | func: 19 | state 3
static s32 GuardClaw_func_191C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata;

    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0, 0.0f, 0);
        fsa->unk33A = 0;
    }
    objdata = baddie->objdata;
    if (objdata->unk8 < 0) {
        fsa->animTickDelta = 0.025f;
    } else {
        if ((objdata->unk8 / 2000.0f) < 0.0f) {
            fsa->animTickDelta = -(objdata->unk8 / 2000.0f);
        } else {
            fsa->animTickDelta = (objdata->unk8 / 2000.0f);
        }
        if (fsa->animTickDelta < 0.011f) {
            fsa->animTickDelta = 0.011f;
        } else if (fsa->animTickDelta > 0.025f) {
            fsa->animTickDelta = 0.025f;
        }
    }
    fsa->unk278 = 0.0f;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 1, 0, data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 2, 1, data_0);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    return 0;
}

// offset: 0x1AD4 | func: 20 | state 4
static s32 GuardClaw_func_1AD4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata;
    
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 1, 0.0f, 0);
        fsa->unk33A = 0;
    }
    objdata = baddie->objdata;
    if (objdata->unk8 > 0) {
        fsa->animTickDelta = 0.025f;
    } else {
        if ((objdata->unk8 / 2000.0f) < 0.0f) {
            fsa->animTickDelta = -(objdata->unk8 / 2000.0f);
        } else {
            fsa->animTickDelta = (objdata->unk8 / 2000.0f);
        }
        if (fsa->animTickDelta < 0.011f) {
            fsa->animTickDelta = 0.011f;
        } else if (fsa->animTickDelta > 0.025f) {
            fsa->animTickDelta = 0.025f;
        }
    }
    fsa->unk278 = 0.0f;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 1, 0, data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 2, 1, data_0);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    return 0;
}

// offset: 0x1C8C | func: 21 | state 5
static s32 GuardClaw_func_1C8C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 2, 0.0f, 0);
        fsa->unk33A = 0;
    }
    self->objhitInfo->unk5F = 0xC;
    self->objhitInfo->unk60 = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.0085f;
    fsa->unk278 = 0.0f;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 1, 0, data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 2, 1, data_0);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    return 0;
}

// offset: 0x1DD8 | func: 22 | state 6
static s32 GuardClaw_func_1DD8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 3, 0.0f, 0);
        fsa->unk33A = 0;
    }
    self->objhitInfo->unk5F = 0xB;
    self->objhitInfo->unk60 = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.0085f;
    fsa->unk278 = 0.0f;
    gDLL_18_objfsa->vtbl->func12(self, fsa, 1, 0, data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 2, 1, data_0);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    return 0;
}

// offset: 0x1F24 | func: 23 | state 7
static s32 GuardClaw_func_1F24(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 7, 0.0f, 0);
        fsa->unk33A = 0;
    }
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, rand_next(1, 3), data_C);
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x1FF8 | func: 24 | state 8
static s32 GuardClaw_func_1FF8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 9, 0.0f, 0);
        fsa->unk33A = 0;
    }
    self->objhitInfo->unk5F = 0xB;
    self->objhitInfo->unk60 = 4;
    func_80028D2C(self);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 2, data_0);
    fsa->animTickDelta = 0.011f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x20E0 | func: 25 | state 9
static s32 GuardClaw_func_20E0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 8, 0.0f, 0);
        fsa->unk33A = 0;
    }
    self->objhitInfo->unk5F = 0xC;
    self->objhitInfo->unk60 = 4;
    func_80028D2C(self);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 2, data_0);
    fsa->animTickDelta = 0.011f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x21C8 | func: 26 | state 10
static s32 GuardClaw_func_21C8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 var_fv1_2;
    Baddie* baddie = self->data;
    ObjSetup* setup = self->setup;
    Object* sidekick = get_sidekick();
    GuardClaw_Data* objdata = baddie->objdata;
    
    if (fsa->enteredAnimState != 0) {
        if (baddie->unk3B0 & 1) {
            sp48 = setup->x - (fsin16_precise(self->srt.yaw) * (f32) objdata->unkC);
            sp44 = setup->z - (fcos16_precise(self->srt.yaw) * (f32) objdata->unkC);
        } else {
            sp48 = self->globalPosition.x - (fsin16_precise(self->srt.yaw) * (f32) objdata->unkC);
            sp44 = self->globalPosition.z - (fcos16_precise(self->srt.yaw) * (f32) objdata->unkC);
        }
        sp4C = gDLL_33_BaddieControl->vtbl->func5(self, fsa, 40.0f);
        if (sidekick != NULL) {
            var_fv1_2 = gDLL_33_BaddieControl->vtbl->func2(self, sp48, sp44, (f32) baddie->unk3E2, sidekick);
        } else {
            var_fv1_2 = rand_next(-1, 1);
        }
        if ((var_fv1_2 < 0.0f) && (sp4C & 8)) {
            if (fsa->enteredAnimState != 0) {
                func_80023D30(self, 0, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else if (sp4C & 2) {
            if (fsa->enteredAnimState != 0) {
                func_80023D30(self, 1, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else {
            if (fsa->enteredAnimState != 0) {
                func_80023D30(self, 5, 0.0f, 0);
                fsa->unk33A = 0;
            }
        }
        fsa->animTickDelta = 0.011f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    gDLL_18_objfsa->vtbl->func12(self, fsa, 1, 0, data_0);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 2, 1, data_0);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 2);
    return 0;
}

// offset: 0x2564 | func: 27 | state 11
static s32 GuardClaw_func_2564(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0xC, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.011f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x25E4 | func: 28 | state 12
static s32 GuardClaw_func_25E4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0xB, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.011f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x2664 | func: 29 | state 13
static s32 GuardClaw_func_2664(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 0xC, 1.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = -0.011f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0x26E0 | func: 30 | ai state 0
static s32 GuardClaw_func_26E0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata;
    
    self->objhitInfo->unk58 &= ~1;
    baddie->unk3B6 = 0;
    if (fsa->unk33A != 0) {
        objdata = baddie->objdata;
        if (objdata->unk8 < -10.0f) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
        } else if (objdata->unk8 > 10.0f) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
        } else {
            u8 temp_v1_2 = rand_next(0, 6);
            switch (temp_v1_2) {
            case 0:
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
                break;
            case 1:
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 2);
                break;
            default:
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
                break;
            }
        }
    }
    return 0;
}

// offset: 0x2890 | func: 31 | ai state 1
static s32 GuardClaw_func_2890(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata;
    u16 sp3E;
    s16 sp3C;
    u16 sp3A;
    u8 sp39;
    
    self->objhitInfo->unk58 |= 1;
    if ((fsa->unk33A != 0) || (fsa->animState == 0)) {
        objdata = baddie->objdata;
        sp39 = 0;
        if (objdata->unk8 < -15.0f) {
            if (objdata->unk8 < -65.0f && (gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 8)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 5);
                sp39 = 1;
            } else if (gDLL_33_BaddieControl->vtbl->func5(self, fsa, 40.0f) & 8) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 3);
                sp39 = 1;
            }
        } else if (objdata->unk8 > 15.0f) {
            if (objdata->unk8 > 65.0f && (gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f) & 2)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
                sp39 = 1;
            } else if (gDLL_33_BaddieControl->vtbl->func5(self, fsa, 40.0f) & 2) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 4);
                sp39 = 1;
            }
        }
        if (sp39 == 0) {
            gDLL_33_BaddieControl->vtbl->func4(self, fsa->target, 8U, &sp3E, &sp3C, &sp3A);
            if ((sp3A < 0x38) && (baddie->unk3B0 & 4)) {
                if (sp3E < 4) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 8);
                } else {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 9);
                }
                baddie->unk3B6 = 2;
            } else if ((baddie->unk3B6 == 0) && (baddie->unk3B0 & 2)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 7);
                baddie->unk3B6 = 1;
            } else {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
            }
        }
    }
    return 0;
}

// offset: 0x2C6C | func: 32 | ai state 2
static s32 GuardClaw_func_2C6C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    self->objhitInfo->unk58 |= 1;
    if ((fsa->unk33A != 0) || (fsa->animState == 0)) {
        if ((fsa->animState != 0xA) && (baddie->unk3B6 != 3)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xA);
        } else if (fsa->animState == 0xA) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xB);
        } else if (fsa->animState == 0xB) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xC);
        }
        baddie->unk3B6 = 3;
    }
    return 0;
}

// offset: 0x2D74 | func: 33 | export: 9
void GuardClaw_func_2D74(Object* self, s32 arg1, SomeCallback arg2) {
    Baddie* baddie = self->data;
    GuardClaw_Data* objdata = baddie->objdata;

    objdata->unk4 = arg1;
    objdata->unk0 = arg2;
}

// offset: 0x2D8C | func: 34 | ai state 3
static s32 GuardClaw_func_2D8C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    self->objhitInfo->unk58 |= 1;
    if ((fsa->unk33A != 0) || (fsa->animState == 0)) {
        if ((fsa->animState != 0xA) && (baddie->unk3B6 != 4)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xA);
        }
        baddie->unk3B6 = 4;
    }
    return 0;
}
