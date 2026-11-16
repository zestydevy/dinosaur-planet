#include "dll.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/gfx/animseq.h"
#include "sys/objprint.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    u16 unk1A;
    u16 unk1C;
    s16 gamebitCollected;
    s16 gamebitPlaySeq;
} WL_podiumgem_Setup;

typedef struct {
    s16 gamebitCollected;
    s16 gamebitPlaySeq;
    u8 dislodged;
} WL_podiumgem_Data;

static int WL_podiumgem_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WL_podiumgem_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_podiumgem_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_podiumgem_obj_Setup(Object* self, WL_podiumgem_Setup* objSetup, s32 reset) {
    WL_podiumgem_Data* objData = self->data;

    objInitMesgQueue(self, 2);

    self->animCallback = WL_podiumgem_animCallback;
    self->srt.yaw = objSetup->yaw << 8;

    if (mainGetBits(objSetup->gamebitCollected)) {
        self->unkDC = TRUE;
        self->unkE0 = TRUE;
    }

    objData->gamebitCollected = objSetup->gamebitCollected;
    objData->gamebitPlaySeq = objSetup->gamebitPlaySeq;
    objData->dislodged = FALSE;

    //Start out on the ground if the dislodging sequence played previously
    if (mainGetBits(objData->gamebitPlaySeq)) {
        objData->dislodged = TRUE;
        self->srt.transl.x = 12516.0f;
        self->srt.transl.y = -103.0f;
        self->srt.transl.z = 1029.0f;
    }
}

// offset: 0x10C | func: 1 | export: 1
void WL_podiumgem_obj_Control(Object* self) {
    WL_podiumgem_Data* objData;
    Object* player;
    u32 msgVal;

    player = objGetPlayer();
    objData = self->data;

    //Fall from the wall when the Projectile Switch's gamebit is set
    if ((objData->gamebitPlaySeq != NO_GAMEBIT) && (objData->dislodged == FALSE) && mainGetBits(objData->gamebitPlaySeq)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objData->dislodged = TRUE;
    }

    //Get scooped up when the player interacts, and play the item collection sequence
    if ((self->unkDC == FALSE) && (self->unkAF & ARROW_FLAG_1_Interacted)) {
        msgVal = 0x20000;
        gDLL_3_Animation->vtbl->set_variable_obj(OBJ_WL_AnimPodiumge, NULL, 0);
        objSendMesg(player, 0x7000A, self, (void*)msgVal);

        if (objData && objData && objData) {} //fake
    }

    //Set gamebit after the item collection sequence 
    while (objRecvMesg(self, &msgVal, NULL, NULL)) {
        if (msgVal == 0x7000B) {
            mainSetBits(objData->gamebitCollected, TRUE);
        }
    }
}

// offset: 0x2A8 | func: 2 | export: 2
void WL_podiumgem_obj_Update(Object* self) { }

// offset: 0x2B4 | func: 3 | export: 3
void WL_podiumgem_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility && (self->unkE0 == FALSE)) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x314 | func: 4 | export: 4
void WL_podiumgem_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x324 | func: 5 | export: 5
u32 WL_podiumgem_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x334 | func: 6 | export: 6
u32 WL_podiumgem_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WL_podiumgem_Data);
}

// offset: 0x348 | func: 7
int WL_podiumgem_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    animData->unk62 = 0;

    return 0;
}
