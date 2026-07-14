#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/animseq.h"
#include "sys/joypad.h"
#include "sys/gfx/texture.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/segment_1460.h"
#include "types.h"
#include "dll.h"

typedef struct {
/*00*/ s16 unk0;
/*02*/ s16 unk2;
/*04*/ s16 unk4;
/*06*/ s16 unk6;
/*08*/ s16 unk8;
/*0A*/ s16 unkA;
/*0C*/ s16 unkC;
/*0E*/ s8 unkE;
/*0F*/ u8 unkF;
/*10*/ u8 unk10;
/*11*/ s8 unk11;
/*12*/ u8 unk12;
/*13*/ u8 unk13;
} MMShrine_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
} Shrine_Setup;

/*0x0*/ static Texture* _data_0 = 0;
/*0x4*/ static u8 _data_4 = 1;

static int dll_466_func_C50(Object* self, Object *arg1, AnimObj_Data* arg2, s8 arg3);
static void MMshrine_func_1140(Object *arg0);

// offset: 0x0 | ctor
void MMshrine_ctor(void *dll) { }

// offset: 0xC | dtor
void MMshrine_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void MMshrine_setup(Object* self, Shrine_Setup* setup, s32 arg2) {
    MMShrine_Data* objdata;
    DLL_IModgfx* temp_v0_2;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    if (setup->unk1A > 0) {
        objdata->unk0 = (s16) (setup->unk1A >> 8);
    }
    objdata->unkF = 0;
    objdata->unk10 = 0;
    objdata->unk2 = 0;
    self->animCallback = dll_466_func_C50;
    objInitMesgQueue(self, 4);
    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
    mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
    mainSetBits(BIT_DB_Entered_Shrine_1, 1);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    mainSetBits(BIT_Test_of_Fear_Particles, 0);
    objdata->unk4 = 0xC;
    objdata->unk8 = 0x1E;
    objdata->unk2 = 0xC8;
    objdata->unk6 = 0;
    objdata->unkA = 0;
    objdata->unk12 = 0;
    objdata->unk13 = 0;
    temp_v0_2 = dllLoad(DLL_ID_122, 1);
    objdata->unkC = temp_v0_2->vtbl->func0(self, 1, 0, 0x402, -1, 0);
    dllFree(temp_v0_2);
    mainSetBits(BIT_Spell_Illusion, 1);
}

// offset: 0x1BC | func: 1 | export: 1
void MMshrine_control(Object *self) {
    MMShrine_Data *objdata;
    Object *player;
    DLL_IModgfx *temp_v0_5;
    Object *temp_v0_4;
    f32 sp3C;
    s32 var_v0;
    f32 var_fv0;

    objdata = (MMShrine_Data*)self->data;
    player = objGetPlayer();
    sp3C = 1000.0f;
    if (_data_4 != 0) {
        self->globalPosition.x = self->srt.transl.x;
        self->globalPosition.y = self->srt.transl.y;
        self->globalPosition.z = self->srt.transl.z;
        gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
        mainSetBits(BIT_DB_Entered_Shrine_2, 1);
        _data_4 = 0;
    }
    MMshrine_func_1140(self);
    if (objdata->unk6 != 0) {
        objdata->unk4 = (s16) (objdata->unk4 + objdata->unk6);
        if (objdata->unk4 < 0xD) {
            objdata->unk4 = 0xC;
            objdata->unk6 = 0;
        } else if (objdata->unk4 >= 0x46) {
            objdata->unk4 = 0x46;
            objdata->unk6 = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objdata->unk4);
    }
    if (objdata->unkA != 0) {
        objdata->unk8 = (s16) (objdata->unk8 + objdata->unkA);
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if ((objdata->unk8 >= 0x46) && (objdata->unkA >= 0)) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unk8);
    }
    if (objdata->unk2 > 0) {
        objdata->unk2 = (s16) (objdata->unk2 - gUpdateRate);
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            if (objdata->unk12 == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objdata->unk8, 0);
                objdata->unk12 = 1;
            }
        }
        if ((objdata->unkF == 2) && (objdata->unk2 < 0x29) && (objdata->unk13 == 0)) {
            objdata->unk13 = 1;
        }
    } else {
        temp_v0_4 = objGetNearestTypeTo(OBJTYPE_Door, player, &sp3C);
        if ((temp_v0_4 != NULL) && (sp3C < 300.0f) && (sp3C > 100.0f)) {
            var_fv0 = temp_v0_4->srt.transl.z - player->srt.transl.z;
            if (var_fv0 <= 0.0f) {
                if (var_fv0 < 0.0f) {
                    var_fv0 *= -1.0f;
                }
                if (objdata->unk8 != 0x1E) {
                    objdata->unk8 = 0x1E;
                }
                var_v0 = (s16) (s32) ((f32) objdata->unk8 * ((var_fv0 - 100.0f) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, var_v0);
                var_v0 = (s16) (s32) ((f32) objdata->unk4 * ((200.0f - (var_fv0 - 100.0f)) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, var_v0);
            }
        }
        switch (objdata->unkF) {
        case 0:
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < (f32) objdata->unk0) {
                objdata->unkF = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->set_camera_module(DLL_ID_CAMSEQ, 0, 0, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                temp_v0_5 = dllLoad(DLL_ID_147, 1);
                temp_v0_5->vtbl->func0(self, 1, 0, 1, -1, 0);
                dllFree(temp_v0_5);
                temp_v0_5 = dllLoad(DLL_ID_148, 1);
                temp_v0_5->vtbl->func0(self, 0, 0, 1, -1, 0);
                dllFree(temp_v0_5);
                mainSetBits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
            }
            break;
        case 1:
            if (objdata->unk10 == 1) {
                objdata->unkF = 2;
                objdata->unk2 = 0;
            }
            break;
        case 2:
            gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
            objdata->unkF = 8;
            break;
        case 8:
            gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
            objdata->unkF = 4;
            break;
        case 7:
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            objdata->unkF = 9;
            objdata->unk2 = 0;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (s16) (u8) objdata->unk8, 0);
            objdata->unkA = 1;
            break;
        case 4:
            if (mainGetBits(BIT_SP_Map_MMP) != 0) {
                objdata->unk8 = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (s16) (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                objdata->unkF = 6;
            } else {
                mainSetBits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (s16) (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            }
            break;
        case 5:
            if (mainGetBits(BIT_Shrine_Do_Exit_Warp) == 0) {
                mainSetBits(BIT_Shrine_Do_Exit_Warp, 1);
            }
            mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
            mainSetBits(BIT_DB_Entered_Shrine_2, 0);
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            objdata->unkF = 6;
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_SP_Map_MMP, 1);
            gDLL_29_Gplay->vtbl->set_act(MAP_WARLOCK_MOUNTAIN, 4);
            break;
        case 9:
            objdata->unkF = 0;
            objdata->unk10 = 0;
            objdata->unk2 = 0x190;
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_DB_Entered_Shrine_2, 1);
            temp_v0_5 = dllLoad(DLL_ID_122, 1);
            objdata->unkC = temp_v0_5->vtbl->func0(self, 0, 0, 0x402, -1, 0);
            dllFree(temp_v0_5);
            break;
        }
    }
}

// offset: 0xB3C | func: 2 | export: 2
void MMshrine_update(Object *self) { }

// offset: 0xB48 | func: 3 | export: 3
void MMshrine_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xB9C | func: 4 | export: 4
void MMshrine_free(Object *self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0xC2C | func: 5 | export: 5
u32 MMshrine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC3C | func: 6 | export: 6
u32 MMshrine_get_data_size(Object *self, s32 arg1) {
    return sizeof(MMShrine_Data);
}

// offset: 0xC50 | func: 7
static int dll_466_func_C50(Object* self, Object *arg1, AnimObj_Data* arg2, s8 arg3) {
    MMShrine_Data* objdata;
    Object* player;
    s32 i;
    u8 temp;

    objdata = self->data;
    player = objGetPlayer();
    
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    
    if (objdata->unkA) {
        objdata->unk8 += objdata->unkA;
        if (objdata->unk8 < 2 && objdata->unkA <= 0) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if (objdata->unk8 >= 0x46 && objdata->unkA >= 0) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unk8);
    }

    for (i = 0; i < arg2->messageCount; i++){
        temp = arg2->messages[i];
        if (temp != 0) {
            switch (temp) {
                case 1:
                    func_80000860(self, self, 0xC7, 0);
                    break;
                case 2:
                    if (D_80092A7C[0] == -1) {
                        func_80000860(self, self, 0x14, 0);
                    } else {
                        func_80000860(self, self, D_80092A7C[0], 0);
                    }
                    break;
                case 3:
                    objdata->unk10 = 1;
                    break;
                case 4:
                    objdata->unk2 = 0;
                    break;
                case 5:
                    objdata->unkF = 5;
                    objdata->unk10 = 2;
                    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                    break;
                case 6:
                    objdata->unk10 = 3;
                    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                    break;
                case 7:
                    mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                    break;
                case 8:
                    mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
                    objdata->unkA = -3;
                    break;
                case 10:
                    mainSetBits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                    if (_data_0 == NULL)
                        _data_0 = blockTexanimGetTex(1);
                    break;
                case 9:
                    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
                    break;
                case 11:
                    objdata->unk8 = 0x64;
                    gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2f, 0x50, (u8)objdata->unk8, 0);
                    break;
                case 12:
                    func_80000860(self, self, 0xCE, 0);
                    mainSetBits(BIT_Test_of_Fear_Particles, 1);
                    gDLL_6_AMSFX->vtbl->play(NULL, SOUND_342_Low_Whoosh, MAX_VOLUME, 0, 0, 0, 0);
                    break;
                case 13:
                    if (D_80092A7C[0] == -1) {
                        func_80000860(self, self, 0x14, 0);
                    } else {
                        func_80000860(self, self, D_80092A7C[0], 0);
                    }
                    mainSetBits(BIT_Test_of_Fear_Particles, 0);
                    break;
            }
        }
        arg2->messages[i] = 0;
    }
    
    if (objdata->unkF == 8) {
        if (vec3_distance(&self->globalPosition, &player->globalPosition) > 10.0f) {
            gDLL_3_Animation->vtbl->end_obj_sequence(arg2->seqSlot);
            objdata->unkF = 7;
        } else if (joyGetButtons(0)) {
            gDLL_3_Animation->vtbl->end_obj_sequence(arg2->seqSlot);
            objdata->unkF = 7;
        }
    }
    
    return 0;
}

// offset: 0x1140 | func: 8
static void MMshrine_func_1140(Object *self) {
    Object *sp54;
    u32 message;
    void *outMesgArg;
    MMShrine_Data *objdata;

    objdata = self->data;
    outMesgArg = 0;

    while (objRecvMesg(self, &message, &sp54, &outMesgArg)){
        switch (message) {
            case 0x30005:
                objdata->unk6 = -3;
                break;
            case 0x30006:
                objdata->unk6 = 0x10;
                break;
        }
    }
}
