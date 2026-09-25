#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/gfx/projgfx.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "dll.h"

/** @file official filename: cfcrystal.c */

typedef struct {
    ObjSetup base;
    s8 yaw8;
    s8 type;
} CFCrystal_Setup;

// size: 0x1C
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
} CFCrystal_Data_38;

typedef struct {
    f32 unk0[4];
    f32 unk10[4];
    f32 unk20[4];
    s16 unk30[4];
    CFCrystal_Data_38 unk38[10];
    s16 unk150;
    u32 unk154;
    u32 unk158;
    u32 unk15C;
    f32 unk160;
    u32 unk164;
    u8 unk168;
    u8 unk169;
    u8 unk16A;
    u8 unk16B;
} CFCrystal_Data;

void CFCrystal_func_288(Object* self);
void CFCrystal_func_11B8(Object* self);

/*0x0*/ static u32 data_0[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xC*/ static DLL_IProjgfx* data_C = NULL;
/*0x10*/ static DLL_IModgfx* data_10 = NULL;

/*0x0*/ static Object* bss_0;

// offset: 0x0 | ctor
void CFCrystal_ctor(void* dll) { }

// offset: 0xC | dtor
void CFCrystal_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFCrystal_obj_Setup(Object* self, CFCrystal_Setup* setup, s32 reset) {
    CFCrystal_Data* objdata = self->data;
    
    self->srt.yaw = setup->yaw8 << 8;
    if (setup->type == 0) {
        objdata->unk168 = 0x28;
        objdata->unk169 = 0;
        objdata->unk16A = 0;
        objdata->unk16B = 0x46;
        self->modelInstIdx = 1;
        objdata->unk154 = 0;
        objdata->unk158 = 0;
        objdata->unk15C = 0;
        objdata->unk164 = 0;
    }
    data_C = dllLoad(DLL_ID_189, 1);
    data_10 = dllLoad(DLL_ID_132, 1);
    objInitMesgQueue(self, 2);
}

// offset: 0xE8 | func: 1 | export: 1
#ifndef NON_MATCHING
void CFCrystal_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/372_CFCrystal/CFCrystal_obj_Control.s")
#else
void CFCrystal_obj_Control(Object* self) {
    CFCrystal_Setup* setup = (CFCrystal_Setup*)self->setup;

    switch (setup->type) {
    case 0:
        CFCrystal_func_288(self);
        return;
    case 1:
        CFCrystal_func_11B8(self);
        break;
    }
}
#endif

// offset: 0x160 | func: 2 | export: 2
void CFCrystal_obj_Update(Object* self) { }

// offset: 0x16C | func: 3 | export: 3
void CFCrystal_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1C0 | func: 4 | export: 4
void CFCrystal_obj_Free(Object* self, s32 onlySelf) {
    CFCrystal_Data* objdata = self->data;

    gDLL_13_Expgfx->vtbl->func5(self);
    if (objdata->unk154 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk154);
    }
    dllFree(data_C);
    dllFree(data_10);
}

// offset: 0x264 | func: 5 | export: 5
u32 CFCrystal_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x274 | func: 6 | export: 6
u32 CFCrystal_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFCrystal_Data);
}

// offset: 0x288 | func: 7
#ifndef NON_MATCHING
/*0x0*/ static const char str_0[] = "Setting  FY %f \n";
/*0x14*/ static const char str_14[] = " Cry action %d\n";
/*0x24*/ static const char str_24[] = " Cry action %d\n";
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/372_CFCrystal/CFCrystal_func_288.s")
#else
void CFCrystal_func_288(Object* self) {
    f32 var_fv0;
    Object* sender;
    CFCrystal_Data* objdata;
    s32 i;
    s32 var_s2;
    s32 var_s5;
    s32 sp8C;
    f32 temp_fv1;
    u32 mesgID;
    void* mesgArg;
    Object* player;
    SRT sp64;
    s32 temp_v0;
    CFCrystal_Data_38* temp;

    objdata = self->data;
    mesgArg = NULL;
    player = objGetPlayer();
    if ((mainGetBits(BIT_57) == 0) && (mainGetBits(BIT_4E0) == 0) 
            && (vec3Distance(&self->globalPosition, &player->globalPosition) < 310.0f)) {
        objdata->unk164 += gUpdateRate;
        if (objdata->unk164 > 180) {
            if (((DLL_210_Player*)player->dll)->vtbl->func50(player) != BIT_Spell_Forcefield) {
                func_8002635C(player, self, Damage_Type_Sword_Staff_Strike1, 1, 0);
                objdata->unk164 -= 180;
            } else {
                mainSetBits(BIT_468_Forcefield_Spell_Taking_Damage, 1);
                objdata->unk164 -= 20;
            }
        }
    }
    camUseShake();
    while (objRecvMesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x110001:
            objdata->unk0[0] = sender->srt.transl.x;
            objdata->unk10[0] = 1945.0f;
            objdata->unk20[0] = sender->srt.transl.z;
            objdata->unk30[0] = 1;
            envfxAction(self, player, 0x7F, 0);
            envfxAction(self, player, (objdata->unk30[1] + objdata->unk30[2] + objdata->unk30[3] + 0x7C), 0);
            STUBBED_PRINTF("Setting  FY %f \n", &sender->srt.transl.y);
            STUBBED_PRINTF(" Cry action %d\n", objdata->unk30[1] + objdata->unk30[2] + objdata->unk30[3] + 0x7C);
            break;
        case 0x110002:
            objdata->unk0[1] = sender->srt.transl.x;
            objdata->unk10[1] = 1945.0f;
            objdata->unk20[1] = sender->srt.transl.z;
            objdata->unk30[1] = 1;
            envfxAction(self, player, 0x7F, 0);
            envfxAction(self, player, (objdata->unk30[1] + objdata->unk30[2] + objdata->unk30[3] + 0x7C), 0);
            STUBBED_PRINTF(" Cry action %d\n", objdata->unk30[1] + objdata->unk30[2] + objdata->unk30[3] + 0x7C);
            break;
        case 0x110003:
            objdata->unk0[2] = sender->srt.transl.x;
            objdata->unk10[2] = 1945.0f;
            objdata->unk20[2] = sender->srt.transl.z;
            objdata->unk30[2] = 1;
            envfxAction(self, player, 0x7F, 0);
            break;
        case 0x110004:
            objdata->unk0[3] = sender->srt.transl.x;
            objdata->unk10[3] = sender->srt.transl.y;
            objdata->unk20[3] = sender->srt.transl.z;
            objdata->unk30[3] = 1;
            break;
        }
    }
    var_fv0++; var_fv0--; // @fake
    if (objdata->unk30[3] == 0) {
        objSendMesgMany(OBJ_CFMainCrystal, OBJMSG_SEND_FILTER_ID | OBJMSG_SEND_IGNORE_SENDER, self, 0x110004, NULL);
    }
    if ((mainGetBits(BIT_54) != 0) && (objdata->unk30[0] == 0)) {
        objSendMesgMany(OBJ_CFPowerBase, OBJMSG_SEND_FILTER_ID, self, 0x110001, NULL);
    }
    if ((mainGetBits(BIT_55) != 0) && (objdata->unk30[1] == 0)) {
        objSendMesgMany(OBJ_CFPowerBase, OBJMSG_SEND_FILTER_ID, self, 0x110002, NULL);
    }
    if ((mainGetBits(BIT_56) != 0) && (objdata->unk30[2] == 0)) {
        objSendMesgMany(OBJ_CFPowerBase, OBJMSG_SEND_FILTER_ID, self, 0x110003, NULL);
    }
    for (i = 0; i < 10; i++) { objdata->unk38[i].unk1B = 0; }
    sp8C = 0;
    var_s5 = 0;
    if (objdata->unk30[3] != 0) {
        if (mainGetBits(BIT_57) != 0) {
            if (objdata->unk30[0] != 0) {
                objdata->unk30[0] = 0x78;
            }
            if (objdata->unk30[1] != 0) {
                objdata->unk30[1] = 0x78;
            }
            if (objdata->unk30[2] != 0) {
                objdata->unk30[2] = 0x78;
            }
            objdata->unk150 = 90;
        }
        for (i = 0; i < 3; i++) {
            if (i >= 3) {
                continue;
            }
            if (objdata->unk30[i] == 0) {
                continue;
            }
            temp = &objdata->unk38[var_s5++];
            temp->unk1B = 1;
            if (objdata->unk30[i] > 30.0f) { // default.dol
                
            }
            temp->unk18 = 0x7F;
            temp->unk19 = 0x7F;
            temp->unk1A = 0xFF;
            if ((objdata->unk168 & 0xFF) > 100) { // @fake
                temp->unk0 = objdata->unk0[3];
                temp->unk8 = (objdata->unk10[3] + 15.0f);
                temp->unk10 = objdata->unk20[3];
                sp64.transl.x = objdata->unk0[i] - temp->unk0;
                sp64.transl.y = (objdata->unk10[i] + 85.0f) - temp->unk8;
                sp64.transl.z = objdata->unk20[i] - temp->unk10;
                data_C->vtbl->func0(self, i, &sp64, 1, -1, 3, NULL);
            }
            if (objdata->unk169 > 100) {
                sp64.transl.x = objdata->unk0[i] - bss_0->srt.transl.x;
                sp64.transl.y = objdata->unk10[i] - bss_0->srt.transl.y;
                sp64.transl.z = objdata->unk20[i] - bss_0->srt.transl.z;
                data_C->vtbl->func0(bss_0, i + 3, &sp64, 1, -1, 3, NULL);
                sp64.transl.x = objdata->unk0[i];
                sp64.transl.y = objdata->unk10[i];
                sp64.transl.z = objdata->unk20[i];
            }
            if (objdata->unk16B > 20) {
                sp64.transl.x = objdata->unk0[i];
                sp64.transl.y = objdata->unk10[i];
                sp64.transl.z = objdata->unk20[i];
                sp64.roll = i;
                if (mathRnd(0, 4) == 0) {
                    dll_partfx->spawn(self, PARTICLE_7F, &sp64, PARTFXFLAG_1, -1, NULL);
                }
                if (mathRnd(0, 4) == 0) {
                    dll_partfx->spawn(self, PARTICLE_7F, &sp64, PARTFXFLAG_1, -1, NULL);
                }
            }
            sp64.transl.x = objdata->unk0[i];
            sp64.transl.y = objdata->unk10[i];
            sp64.transl.z = objdata->unk20[i];
            sp64.roll = i;
            if (mathRnd(0, 2) == 0) {
                dll_partfx->spawn(self, PARTICLE_7E, &sp64, PARTFXFLAG_1, -1, NULL);
            }
            temp = &objdata->unk38[var_s5++];
            temp->unk1B = 1;
            sp8C += 1;
        }
        temp_v0 = objdata->unk30[0] + objdata->unk30[1] + objdata->unk30[2];
        if (temp_v0 < 300) {
            if (temp_v0 > 256) {
                if (mathRnd(0, 9) == 0) {
                    dll_partfx->spawn(self, PARTICLE_83, NULL, 0, -1, NULL);
                }
            } else if (temp_v0 > 128) {
                if (mathRnd(0, 6) == 0) {
                    dll_partfx->spawn(self, PARTICLE_82, NULL, 0, -1, NULL);
                }
            } else if (mathRnd(0, 3) == 0) {
                dll_partfx->spawn(self, PARTICLE_81, NULL, 0, -1, NULL);
            }
        }
        if ((objdata->unk30[0] != 0) || (objdata->unk30[1] != 0) || (objdata->unk30[2] != 0)) {
            if (objdata->unk16A > 100) {
                if ((objdata->unk30[0] != 0) && (objdata->unk30[1] != 0) && (objdata->unk30[2] != 0)) {
                    data_10->vtbl->func0(self, 0, NULL, 1, -1, NULL);
                } else {
                    dll_partfx->spawn(self, PARTICLE_7B, NULL, 0, -1, NULL);
                }
            }
            if (objdata->unk168 > 100) {
                objdata->unk168 = 0;
                if ((objdata->unk30[0] != 0) && (objdata->unk30[1] != 0) && (objdata->unk30[2] != 0)) {
                    data_10->vtbl->func0(self, 0, NULL, 1, -1, NULL);
                }
            }
            if (objdata->unk169 > 100) {
                objdata->unk169 = 0;
            }
            if (objdata->unk16A > 100) {
                objdata->unk16A = 0;
            }
            if (objdata->unk16B > 20) {
                objdata->unk16B = 0;
            }
            objdata->unk168 += gUpdateRate;
            objdata->unk169 += gUpdateRate;
            objdata->unk16A += gUpdateRate;
            objdata->unk16B += gUpdateRate;
        } else if (mathRnd(0, 20) == 0) {
            dll_partfx->spawn(self, PARTICLE_7B, NULL, 0, -1, NULL);
        }
        if (sp8C == 3) {
            if (objdata->unk150 == 0) {
                dll_amSfx->Play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->unk150 += gUpdateRate;
        }
        if (objdata->unk150 > 30) {
            mainSetBits(BIT_4E0, 1);
            mainSetBits(BIT_Player_Immune_to_Rainbow_Scarabs, 0);
        }
        if (objdata->unk150 >= 60) {
            var_fv0 = objdata->unk150 - 60;
            var_fv0 /= 30.0f;
            temp = &objdata->unk38[var_s5++];
            temp->unk1B = 1;
            temp->unk18 = 0;
            temp->unk19 = 0;
            temp->unk1A = 0;
            temp->unk0 = self->srt.transl.x;
            temp->unk8 = (self->srt.transl.y + 15.0f);
            temp->unk10 = self->srt.transl.z;
            temp->unk4 = temp->unk0;
            temp->unkC = (temp->unk8 - (250.0f * var_fv0));
            temp->unk14 = temp->unk10;
        }
        self->srt.yaw += sp8C * 0x7E * gUpdateRate;
    }
    if (sp8C != 0) {
        if (dll_amSfx->IsPlaying(objdata->unk154) == 0) {
            dll_amSfx->Stop(objdata->unk154);
            objdata->unk154 = 0;
        }
        if (objdata->unk154 == 0) {
            dll_amSfx->Play(self, SOUND_12D, MAX_VOLUME, &objdata->unk154, NULL, 0, NULL);
            objdata->unk160 = 0.66f;
        } else {
            temp_fv1 = ((f32) sp8C / 3.0f) + 0.33f;
            objdata->unk160 += ((temp_fv1 - objdata->unk160) * 0.0625f);
            if (objdata->unk150 >= 60) {
                objdata->unk160 = temp_fv1;
            }
            dll_amSfx->SetPitch(objdata->unk154, objdata->unk160);
        }
    }

    for (i = 0; i < 3; i++) {
        if (objdata->unk30[i] != 0 && objdata->unk30[i] < 128) {
            var_s2 = objdata->unk30[i];
            objdata->unk30[i] += gUpdateRate;
            if ((var_s2 == 1) && (objdata->unk30[i] >= 2) && (objdata->unk158 == 0)) {
                dll_amSfx->Play(self, SOUND_12E, MAX_VOLUME, &objdata->unk158, NULL, 0, NULL);
            }
            if ((var_s2 < 30) && (objdata->unk30[i] >= 30) && (objdata->unk15C == 0)) {
                dll_amSfx->Play(self, SOUND_12F, MAX_VOLUME, &objdata->unk15C, NULL, 0, NULL);
            }
        }
    }
    self->srt.yaw += gUpdateRate * 0x2A;
}
#endif

// offset: 0x11B8 | func: 8
void CFCrystal_func_11B8(Object* self) {
    Object* sender;
    u32 mesgID;
    void* mesgArg;

    mesgArg = NULL;
    while (objRecvMesg(self, &mesgID, &sender, &mesgArg) != 0) {
        if (mesgID == 0x110004) {
            objSendMesg(sender, 0x110004, self, NULL);
        }
    }
    bss_0 = self;
    self->srt.yaw += gUpdateRate * 0xB6;
}
