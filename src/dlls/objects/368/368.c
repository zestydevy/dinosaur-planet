#include "game/gamebits.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    u8 _unk18;
    u8 unk19;
} DLL368_Setup;

static int dll_368_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);

// offset: 0x0 | ctor
void dll_368_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_368_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_368_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    self->srt.yaw = 0;
    self->animCallback = dll_368_animCallback;
    objInitMesgQueue(self, 2);
}

// offset: 0x6C | func: 1 | export: 1
void dll_368_obj_Control(Object* self) {
    DLL368_Setup* setup = (DLL368_Setup*)self->setup;
    Object* player;
    f32 zDiff;
    f32 ftemp;
    f32 yDiff;

    switch (setup->unk19) {
    case 0:
        if (mainGetBits(BIT_Played_Seq_0041_Scales_Kills_The_Queen) == 0) {
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            self->unkDC = 0;
            return;
        }
    default:
        return;
    case 1:
        gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
        self->unkDC = 0;
        return;
    case 2:
        gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
        self->unkDC = 0;
        return;
    case 3:
        if ((self->unkE0 == 0) && (mainGetBits(BIT_61) != 0)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            self->unkDC = 0;
            self->unkE0 = 1;
            return;
        }
        break;
    case 4:
        if ((mainGetBits(BIT_67) == 0) && (mainGetBits(BIT_66) != 0)) {
            mainSetBits(BIT_67, 1);
            gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
            self->unkDC = 0;
            return;
        }
        break;
    case 5:
        gDLL_3_Animation->vtbl->start_obj_sequence(6, self, -1);
        break;
    case 6:
        if ((D_800B4A5E != -1) && (self->unkDC == 0)) {
            player = objGetPlayer();
            ftemp = player->srt.transl.x - self->srt.transl.x;
            yDiff = player->srt.transl.y - self->srt.transl.y;
            zDiff = player->srt.transl.z - self->srt.transl.z;
            ftemp = (SQ(ftemp) + SQ(yDiff) + SQ(zDiff));
            if (ftemp < 22500.0f) {
                gDLL_3_Animation->vtbl->start_obj_sequence(7, self, -1);
                D_80092A78 = 2;
                self->unkDC = 1;
                return;
            }
        }
        break;
    case 7:
        if (mainGetBits(BIT_20A) != 0) {
            gDLL_3_Animation->vtbl->start_obj_sequence(8, self, -1);
            mainSetBits(BIT_20A, 0);
        }
        break;
    }
}

// offset: 0x390 | func: 2 | export: 2
void dll_368_obj_Update(Object* self) { }

// offset: 0x39C | func: 3 | export: 3
void dll_368_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3F0 | func: 4 | export: 4
void dll_368_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x400 | func: 5 | export: 5
u32 dll_368_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x410 | func: 6 | export: 6
u32 dll_368_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x424 | func: 7
static int dll_368_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    DLL368_Setup* setup = (DLL368_Setup*)self->setup;

    switch (setup->unk19) {
    Object* sender;
    u32 mesgID;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    case 0:
        if ((self->unkDC == 0) 
                && (mainGetBits(BIT_Played_Seq_0041_Scales_Kills_The_Queen) == 0) 
                && (mainGetBits(BIT_Play_Seq_0041_Scales_Kills_The_Queen) != 0)) {
            gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
            self->unkDC = 1;
            self->unkE0 = 1;
        }
        while (objRecvMesg(self, &mesgID, &sender, NULL) != 0) {
            if (mesgID == 0xA0005) {
                mainSetBits(BIT_Played_Seq_0041_Scales_Kills_The_Queen, 1);
                lfxAction(self, self, 0x18, 0, 0, NULL);
            }
        }
        break;
    case 6:
        if (animObjData->lastMessage == 1) {
            gDLL_3_Animation->vtbl->set_camera_module(DLL_ID_CAMPATH, 0, 0, 0x78);
            animObjData->lastMessage = 0;
        }
        break;
    }
    if ((self->unkE0 != 0) && (gDLL_28_ScreenFade->vtbl->is_complete() != 0)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        gDLL_28_ScreenFade->vtbl->fade_reversed(30, SCREEN_FADE_BLACK);
        self->unkE0 = 0;
    }
    animObjData->unk62 = 0;
    return 0;
}
