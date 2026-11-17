#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/53_movelib.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/main.h"
#include "sys/newshadows.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
/*000*/ ObjFSA_Data fsa;
/*34C*/ MoveLibData moveData;
/*804*/ HeadAnimation unk804;
/*828*/ f32 playerDist;
} WCqueenearthwalker_Data;

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[2];
/*0x8*/ static ObjFSA_StateCallback sLogicStateCallbacks[3];

static void WCqueenearthwalker_updateShadow(Object* self);
static s32 WCqueenearthwalker_animState0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCqueenearthwalker_animState1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCqueenearthwalker_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCqueenearthwalker_logicState1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 WCqueenearthwalker_logicState2(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void WCqueenearthwalker_setupFSACallbacks(void) {
    sAnimStateCallbacks[0] = WCqueenearthwalker_animState0;
    sAnimStateCallbacks[1] = WCqueenearthwalker_animState1;
    sLogicStateCallbacks[0] = WCqueenearthwalker_logicState0;
    sLogicStateCallbacks[1] = WCqueenearthwalker_logicState1;
    sLogicStateCallbacks[2] = WCqueenearthwalker_logicState2;
}

// offset: 0x60 | ctor
void WCqueenearthwalker_ctor(void* dll) {
    WCqueenearthwalker_setupFSACallbacks();
}

// offset: 0xA0 | dtor
void WCqueenearthwalker_dtor(void* dll) { }

// offset: 0xAC | func: 1 | export: 0
void WCqueenearthwalker_obj_Setup(Object* self, Generic_Setup* setup, s32 reset) {
    WCqueenearthwalker_Data* objdata = self->data;
    s16 sp30[] = {0, 8};
    s16 sp2C[] = {10, 35};

    mainCreateTempDLL(DLL_ID_MOVELIB);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->moveData, -0x1FFF, 0x31C6, 2);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->moveData, 0, sp30, 2);
    objdata->moveData.unk4A9 |= 2;
}

// offset: 0x188 | func: 2 | export: 1
void WCqueenearthwalker_obj_Control(Object* self) {
    WCqueenearthwalker_Data* objdata = self->data;
    
    objdata->playerDist = vec3DistanceXZ(&self->globalPosition, &objGetPlayer()->globalPosition);
    gDLL_18_objfsa->vtbl->tick(self, &objdata->fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &objdata->moveData);
    objExprEyeIdle(self, &objdata->unk804);
    WCqueenearthwalker_updateShadow(self);
}

// offset: 0x290 | func: 3 | export: 2
void WCqueenearthwalker_obj_Update(Object* self) { }

// offset: 0x29C | func: 4 | export: 3
void WCqueenearthwalker_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    WCqueenearthwalker_Data* objdata = self->data;
    
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->moveData, 0);
    }
}

// offset: 0x324 | func: 5 | export: 4
void WCqueenearthwalker_obj_Free(Object* self, s32 onlySelf) {
    // @bug: missing mainRemoveTempDLL(DLL_ID_MOVELIB);
}

// offset: 0x334 | func: 6 | export: 5
u32 WCqueenearthwalker_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x344 | func: 7 | export: 6
u32 WCqueenearthwalker_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCqueenearthwalker_Data);
}

// offset: 0x358 | func: 8
static void WCqueenearthwalker_updateShadow(Object* self) {
    SRT srt;
    MtxF mtx;
    Vec3f pos;

    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    mathYprXyzMtx(&mtx, &srt);
    mathMtxXFMF(&mtx, 0.0f, 0.0f, 8.0f, &pos.z, &pos.y, &pos.x);
    shadowsSetCustomObjPos(self, pos.z, pos.y, pos.x);
}

// offset: 0x434 | func: 9
static s32 WCqueenearthwalker_animState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x44C | func: 10
static s32 WCqueenearthwalker_animState1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        objAnimSet(self, 0x1C, 0.0f, 0);
        fsa->animTickDelta = 0.005f;
    }
    return 0;
}

// offset: 0x4B8 | func: 11
static s32 WCqueenearthwalker_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return FSA_NEXTSTATE_SYNC(1);
}

// offset: 0x4D0 | func: 12
static s32 WCqueenearthwalker_logicState1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WCqueenearthwalker_Data* objdata = self->data;
    
    if (fsa->enteredLogicState != 0) {
        objdata->moveData.unk4A9 |= 1;
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
    }
    if (objdata->playerDist < 100.0f) {
        return FSA_NEXTSTATE_SYNC(2);
    }
    return 0;
}

// offset: 0x564 | func: 13
static s32 WCqueenearthwalker_logicState2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    WCqueenearthwalker_Data* objdata = self->data;

    if (fsa->enteredLogicState != 0) {
        objdata->moveData.unk4A9 &= ~1;
    }
    return 0;
}
