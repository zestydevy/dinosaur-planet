#include "common.h"
#include "dlls/engine/53_movelib.h"
#include "game/objects/interaction_arrow.h"
#include "sys/newshadows.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} WCKingEarthWalker_Setup;

typedef struct {
    MoveLibData moveData;
    HeadAnimation headAnim;
    u8 state;
    u8 flags;
} WCKingEarthWalker_Data;

typedef enum {
    WCKingEarthWalker_STATE_0_Initial,
    WCKingEarthWalker_STATE_1_Met_Sabre,
    WCKingEarthWalker_STATE_2_SpellStone_Retrieved
} WCKingEarthWalker_States;

typedef enum {
    WCKingEarthWalker_FLAG_1_Waiting_for_Sequence = 1
} WCKingEarthWalker_Flags;

static void WCKingEarthWalker_updateShadow(Object* self);
static int WCKingEarthWalker_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WCKingEarthWalker_ctor(void* dll) { }

// offset: 0xC | dtor
void WCKingEarthWalker_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCKingEarthWalker_obj_Setup(Object* self, WCKingEarthWalker_Setup* objSetup, s32 reset) {
    WCKingEarthWalker_Data* objData = self->data;
/*0x0*/ s16 data_0[] = { 0, 8} ;
/*0x4*/ s16 data_4[] = { 10, 35 };

    self->animCallback = WCKingEarthWalker_animCallback;
    
    mainCreateTempDLL(DLL_ID_MOVELIB);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objData->moveData, -0x1FFF, 0x31C6, 2);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objData->moveData, 0, data_0, 2);
    
    objData->moveData.unk4A9 |= 2;
    
    self->srt.yaw = objSetup->yaw << 8;
}

// offset: 0x114 | func: 1 | export: 1
void WCKingEarthWalker_obj_Control(Object* self) {
    WCKingEarthWalker_Data* objData;

    objData = self->data;
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &objData->moveData);
    objExprEyeIdle(self, &objData->headAnim);
    
    //Return early if a gamebit-driven sequence has yet to play
    if ((objData->flags & WCKingEarthWalker_FLAG_1_Waiting_for_Sequence)) {
        return;
    }
    
    //State Machine (unfinished)
    switch (objData->state) {
    case WCKingEarthWalker_STATE_1_Met_Sabre:
    case WCKingEarthWalker_STATE_2_SpellStone_Retrieved:    //Presumably!
        break;
    case WCKingEarthWalker_STATE_0_Initial:
        //Play King EarthWalker's initial cutscene when interacting with him
        if (self->unkAF & ARROW_FLAG_1_Interacted) {
            joyDisableButtons(0, A_BUTTON);
            mainSetBits(BIT_WC_Met_King_EarthWalker, TRUE);
            objData->state = WCKingEarthWalker_STATE_1_Met_Sabre;
            objData->flags |= WCKingEarthWalker_FLAG_1_Waiting_for_Sequence;
        }
        break;
    }
    
    //Update animation and shadow
    objAnimAdvance(self, 0.005f, gUpdateRateF, NULL);
    WCKingEarthWalker_updateShadow(self);
}

// offset: 0x254 | func: 2 | export: 2
void WCKingEarthWalker_obj_Update(Object* self) { }

// offset: 0x260 | func: 3 | export: 3
void WCKingEarthWalker_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    WCKingEarthWalker_Data* objData = self->data;
    
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objData->moveData, 0);
    }
}

// offset: 0x2E4 | func: 4 | export: 4
void WCKingEarthWalker_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x2F4 | func: 5 | export: 5
u32 WCKingEarthWalker_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x304 | func: 6 | export: 6
u32 WCKingEarthWalker_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCKingEarthWalker_Data);
}

// offset: 0x318 | func: 7
void WCKingEarthWalker_updateShadow(Object* self) {
    SRT xform;
    MtxF mtx;
    Vec3f v;

    xform.transl.x = self->srt.transl.x;
    xform.transl.y = self->srt.transl.y;
    xform.transl.z = self->srt.transl.z;
    xform.yaw = self->srt.yaw;
    xform.pitch = self->srt.pitch;
    xform.roll = self->srt.roll;
    xform.scale = 1.0f;
    mathYprXyzMtx(&mtx, &xform);
    mathMtxXFMF(&mtx, 0.0f, 0.0f, 8.0f, &v.z, &v.y, &v.x);
    shadowsSetCustomObjPos(self, v.z, v.y, v.x);
}

// offset: 0x3F4 | func: 8
int WCKingEarthWalker_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    WCKingEarthWalker_Data* objData = self->data;
    
    objData->flags &= ~WCKingEarthWalker_FLAG_1_Waiting_for_Sequence;
    
    return 0;
}

