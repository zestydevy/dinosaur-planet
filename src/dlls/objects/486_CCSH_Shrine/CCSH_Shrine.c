#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/modgfx.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/segment_1460.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    s16 unk1A;
} CCSH_Shrine_Setup;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    u8 unkE;
    u8 unkF;
    u8 unk10;
    u8 unk11;
    u8 unk12;
} CCSH_Shrine_Data;

/*0x0*/ static Texture* data_0 = NULL;

static int CCSH_Shrine_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);
static void CCSH_Shrine_process_obj_messages(Object* self);

// offset: 0x0 | ctor
void CCSH_Shrine_ctor(void* dll) { }

// offset: 0xC | dtor
void CCSH_Shrine_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCSH_Shrine_setup(Object* self, CCSH_Shrine_Setup* setup, s32 reset) {
    CCSH_Shrine_Data* objdata = self->data;
    DLL_IModgfx* modgfx;
    
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    if (setup->unk1A > 0) {
        objdata->unk0 = setup->unk1A >> 8;
    }
    objdata->unkF = 0;
    objdata->unk10 = 0;
    objdata->unk2 = 0;
    objdata->unkE = 0;
    self->animCallback = CCSH_Shrine_anim_callback;
    objInitMesgQueue(self, 4);
    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
    mainSetBits(BIT_1CF, 0);
    mainSetBits(BIT_DB_Entered_Shrine_1, 1);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    mainSetBits(BIT_1CD, 0);
    mainSetBits(BIT_1E7, 0);
    objdata->unk4 = 0xC;
    objdata->unk8 = 0x1E;
    objdata->unk2 = 0xC8;
    gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    objdata->unk6 = 0;
    objdata->unkA = 0;
    objdata->unk12 = 0;
    modgfx = dllLoadDeferred(DLL_ID_122, 1);
    objdata->unkC = modgfx->vtbl->func0(self, 0, 0, 0x402, -1, 0);
    dllFree(modgfx);
}

// offset: 0x1EC | func: 1 | export: 1
void CCSH_Shrine_control(Object* self) {
    CCSH_Shrine_Data* objdata = self->data;
    Object* player;
    Object* obj;
    DLL_IModgfx* modgfx;
    f32 doorDist;
    f32 dist;
    f32 sp2C;
    s16 volume;
    
    player = objGetPlayer();
    dist = 1000.0f;
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
    CCSH_Shrine_process_obj_messages(self);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    if (objdata->unk6 != 0) {
        objdata->unk4 += objdata->unk6;
        if (objdata->unk4 <= 12) {
            objdata->unk4 = 12;
            objdata->unk6 = 0;
        } else if (objdata->unk4 >= 70) {
            objdata->unk4 = 70;
            objdata->unk6 = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objdata->unk4);
    }
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 <= 1) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if ((objdata->unk8 >= 70) && (objdata->unkA >= 0)) {
            objdata->unk8 = 70;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unk8);
    }
    if (objdata->unk2 > 0) {
        objdata->unk2 -= gUpdateRate;
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            if (objdata->unk12 == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objdata->unk8, 0);
                objdata->unk12 = 1;
            }
        }
    } else {
        obj = objGetNearestTypeTo(OBJTYPE_Door, player, &dist);
        if ((obj != NULL) && (dist < 300.0f) && (dist > 100.0f)) {
            doorDist = obj->srt.transl.z - player->srt.transl.z;
            if (doorDist <= 0.0f) {
                if (doorDist < 0.0f) {
                    doorDist *= -1.0f;
                }
                sp2C = doorDist - 100.0f;
                if (objdata->unk8 != 0x1E) {
                    objdata->unk8 = 0x1E;
                }
                volume = (s16) ((f32) objdata->unk8 * (sp2C / 200.0f));
                if (volume <= 0) {
                    volume = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, volume);
                volume = (s16) ((f32) objdata->unk4 * ((200.0f - sp2C) / 200.0f));
                if (volume <= 0) {
                    volume = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, volume);
            }
        }
        switch (objdata->unkF) {
        case 0:
            if ((mainGetBits(BIT_5B5) == 0) && (mainGetBits(BIT_594) != 0)) {
                mainSetBits(BIT_5B5, 1);
            }
            mainSetBits(BIT_5B9, 0);
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < (f32) objdata->unk0) {
                objdata->unkF = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                modgfx = dllLoadDeferred(DLL_ID_147, 1);
                modgfx->vtbl->func0(self, 0, 0, 1, -1, 0);
                dllFree(modgfx);
                modgfx = dllLoadDeferred(DLL_ID_148, 1);
                modgfx->vtbl->func0(self, 0, 0, 1, -1, 0);
                dllFree(modgfx);
                mainSetBits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
            }
        default:
            return;
        case 1:
            if (objdata->unk10 == 1) {
                objdata->unkF = 2;
                objdata->unk2 = 0xA0;
                return;
            }
            break;
        case 2:
            if ((objdata->unkE == 0) && (mainGetBits(BIT_1CD) == 0)) {
                mainSetBits(BIT_1CD, 1);
            }
            if (mainGetBits(BIT_5B2) != 0) {
                objdata->unkE++;
                objdata->unk2 = 0x64;
                if (objdata->unkE == 1) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
                }
            }
            break;
        case 7:
            gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
            objdata->unkF = 3;
            objdata->unk2 = 0;
            objdata->unkA = -3;
            return;
        case 8:
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            objdata->unkF = 6;
            objdata->unk2 = 0;
            objdata->unkA = -3;
            return;
        case 6:
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (u8) objdata->unk8, 0);
            objdata->unkA = 1;
            gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
            dist = 10000.0f;
            obj = objGetNearestTypeTo(OBJTYPE_Baddie, self, &dist);
            if (obj != NULL) {
                objFreeObject(obj);
            }
            objdata->unkF = 0;
            objdata->unk2 = 0x190;
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_DB_Entered_Shrine_2, 1);
            mainSetBits(BIT_5B2, 0);
            mainSetBits(BIT_5B9, 1);
            modgfx = dllLoadDeferred(DLL_ID_122, 1);
            objdata->unkC = modgfx->vtbl->func0(self, 0, 0, 0x402, -1, 0);
            dllFree(modgfx);
            mainSetBits(BIT_1CD, 0);
            objdata->unkE = 0;
            objdata->unk10 = 0;
            return;
        case 3:
            dist = 10000.0f;
            obj = objGetNearestTypeTo(OBJTYPE_Baddie, self, &dist);
            if (obj != NULL) {
                objFreeObject(obj);
            }
            if (mainGetBits(BIT_1CE) != 0) {
                objdata->unk8 = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                objdata->unkF = 5;
                return;
            }
            mainSetBits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2A, 0x50, (u8) objdata->unk8, 0);
            objdata->unkA = 1;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            break;
        case 4:
            if (mainGetBits(BIT_Shrine_Do_Exit_Warp) == 0) {
                mainSetBits(BIT_Shrine_Do_Exit_Warp, 1);
            }
            mainSetBits(BIT_1CF, 0);
            mainSetBits(BIT_DB_Entered_Shrine_2, 0);
            objdata->unkF = 5;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->unk8, 0);
            mainSetBits(BIT_1CE, 1);
            gDLL_29_Gplay->vtbl->set_act(MAP_WARLOCK_MOUNTAIN, 6);
            break;
        }
    }
}

// offset: 0xC7C | func: 2 | export: 2
void CCSH_Shrine_update(Object* self) { }

// offset: 0xC88 | func: 3 | export: 3
void CCSH_Shrine_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xCDC | func: 4 | export: 4
void CCSH_Shrine_free(Object* self, s32 onlySelf) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0xD6C | func: 5 | export: 5
u32 CCSH_Shrine_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xD7C | func: 6 | export: 6
u32 CCSH_Shrine_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(CCSH_Shrine_Data);
}

// offset: 0xD90 | func: 7
static int CCSH_Shrine_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    CCSH_Shrine_Data* objdata = actor->data;
    s32 i;

    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if ((objdata->unk8 >= 70) && (objdata->unkA >= 0)) {
            objdata->unk8 = 70;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unk8);
    }
    for (i = 0; i < animObjData->messageCount; i++) {
        if (animObjData->messages[i] != 0) {
            switch (animObjData->messages[i]) {
            case 11:
                objdata->unkF = 7;
                break;
            case 1:
                func_80000860(actor, actor, 0xC3, 0);
                break;
            case 2:
                if (*D_80092A7C == -1) {
                    func_80000860(actor, actor, 0x14, 0);
                } else {
                    func_80000860(actor, actor, *D_80092A7C, 0);
                }
                break;
            case 3:
                objdata->unk10 = 1;
                break;
            case 4:
                objdata->unkF = 4;
                objdata->unk10 = 2;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                mainSetBits(BIT_1CF, 0);
                mainSetBits(BIT_DB_Entered_Shrine_1, 1);
                objdata->unkA = -3;
                break;
            case 5:
                objdata->unk10 = 3;
                objdata->unkA = -3;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                mainSetBits(BIT_1CF, 1);
                break;
            case 7:
                mainSetBits(BIT_1CF, 0);
                objdata->unkA = -3;
                break;
            case 9:
                mainSetBits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (data_0 == NULL) {
                    data_0 = blockTexanimGetTex(1);
                }
                break;
            case 8:
                mainSetBits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 10:
                objdata->unk8 = 0x64;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2D, 0x50, (u8) objdata->unk8, 0);
                break;
            }
        }
        animObjData->messages[i] = 0;
    }
    if (objdata->unkF == 7) {
        if (joyGetButtons(0) & A_BUTTON) {
            gDLL_3_Animation->vtbl->end_obj_sequence(animObjData->seqSlot);
            objdata->unkF = 8;
            objdata->unk2 = 0;
        } else if (joyGetButtons(0) & B_BUTTON) {
            gDLL_3_Animation->vtbl->end_obj_sequence(animObjData->seqSlot);
            objdata->unkF = 7;
            objdata->unk2 = 0;
        }
    }
    return 0;
}

// offset: 0x1194 | func: 8
static void CCSH_Shrine_process_obj_messages(Object* self) {
    Object* sender;
    u32 mesgID;
    void* mesgArg;
    CCSH_Shrine_Data* objdata = self->data;
    
    mesgArg = NULL;
    while (objRecvMesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x30005:
            objdata->unk6 = -3;
            break;
        case 0x30006:
            objdata->unk6 = 0x10;
            break;
        }
    }
}
