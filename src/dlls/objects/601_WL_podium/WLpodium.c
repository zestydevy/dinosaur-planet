#include "dll.h"
#include "dlls/objects/607_WL_LevelControl.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 gamebitCrystal;
    s16 gamebitPlaced;
} WL_podium_Setup;

typedef struct {
    u32 soundHandle;
    s16 gamebitCrystal;
    s16 gamebitPlaced;
    s16 unk8;
    s16 unkA;
} WL_podium_Data;

static int WL_podium_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WL_podium_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_podium_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_podium_obj_Setup(Object* self, WL_podium_Setup* objSetup, s32 reset) {
    WL_podium_Data* objData = self->data;

    self->animCallback = WL_podium_animCallback;
    self->srt.yaw = objSetup->yaw << 8;
    self->unkDC = 0;

    objData->gamebitCrystal = objSetup->gamebitCrystal;
    objData->gamebitPlaced = objSetup->gamebitPlaced;
    objData->unk8 = objSetup->unk19;

    if ((objData->gamebitPlaced != NO_GAMEBIT) && mainGetBits(objData->gamebitPlaced)) {
        self->srt.transl.y = objSetup->base.y + 25.0f;
        objSetModel(self, 1);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
}

// offset: 0xFC | func: 1 | export: 1
void WL_podium_obj_Control(Object* self) {
    WL_podium_Data* objData = self->data;

    //Show Z-Lock interaction tutorial box when the player approaches
    if ((self->unkAF & ARROW_FLAG_4_Highlighted) && (mainGetBits(BIT_Shown_ZLock_Interact_Message) == FALSE)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
        mainSetBits(BIT_Shown_ZLock_Interact_Message, TRUE);
        return;
    }

    //LockIcon greyed out until the Warp Crystal is collected
    if (mainGetBits(objData->gamebitCrystal)) {
        self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
    } else {
        self->unkAF |= ARROW_FLAG_10_Greyed_Out;
    }

    //Check if the player used the Warp Crystal
    if ((self->unkAF & ARROW_FLAG_4_Highlighted) && (gDLL_1_cmdmenu->vtbl->was_this_item_used(objData->gamebitCrystal)) && (self->unkDC == 0)) {
        if (gDLL_29_Gplay->vtbl->get_act(self->mapID) == WM_ACT3_Spirit2_Sabre_DB) {
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        } else {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        }
        self->unkDC = 1;
    }

    //Change model when the Warp Crystal's placed
    if ((objData->gamebitPlaced != NO_GAMEBIT) && mainGetBits(objData->gamebitPlaced) && (self->modelInstIdx == 0)) {
        objSetModel(self, 1);
    }
}

// offset: 0x2EC | func: 2 | export: 2
void WL_podium_obj_Update(Object* self) { }

// offset: 0x2F8 | func: 3 | export: 3
void WL_podium_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x34C | func: 4 | export: 4
void WL_podium_obj_Free(Object* self, s32 onlySelf) {
    WL_podium_Data* objData = self->data;

    if (objData->soundHandle) {
        gDLL_6_AMSFX->vtbl->Stop(objData->soundHandle);
    }
}

// offset: 0x3A8 | func: 5 | export: 5
u32 WL_podium_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x3B8 | func: 6 | export: 6
u32 WL_podium_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WL_podium_Data);
}

// offset: 0x3CC | func: 7
int WL_podium_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    WL_podium_Data* objData;
    s32 i;

    objData = self->data;
    animData->unk62 = 0;

    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i]) {
            switch (animData->messages[i]) {
            case 1:
                if ((objData->gamebitPlaced != NO_GAMEBIT) && (mainGetBits(objData->gamebitPlaced) == FALSE)) {
                    mainSetBits(objData->gamebitPlaced, TRUE);
                    self->unkAF |= ARROW_FLAG_8_No_Targetting;
                }
                break;
            }
            animData->messages[i] = 0;
        }
    }

    return 0;
}
