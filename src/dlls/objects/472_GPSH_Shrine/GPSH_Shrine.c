#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/dll.h"
#include "sys/envfx.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/map.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 _unk18;
    s16 testStartRadius;
} GPSH_Shrine_Setup;

typedef struct {
    s16 testStartRadius;
    s16 musicPlayTimer;
    s16 volumeA;
    s16 volumeASpeed;
    s16 volumeB;
    s16 volumeBSpeed;
    s16 modGfxCircle;
    s32 time;
    u8 itemsPlaced;
    u8 state;
    u8 seqValue;
    u8 _unk17;
    u8 musicStarted;
    u8 unk19;
} GPSH_Shrine_Data;

typedef enum {
    GPSH_Shrine_STATE_Waiting,
    GPSH_Shrine_STATE_Test_Start,
    GPSH_Shrine_STATE_Test_of_Knowledge,
    GPSH_Shrine_STATE_3, //State never seems to get set to 3, but it is referenced?
    GPSH_Shrine_STATE_Test_Successful,
    GPSH_Shrine_STATE_Warp_Away,
    GPSH_Shrine_STATE_Finished,
    GPSH_Shrine_STATE_Test_Failure
} GPSH_Shrine_States;

/*0x0*/ static Texture* _data_0 = NULL;

/*0x0*/ static u8 _bss_0[0x8];
/*0x8*/ static f32 _bss_8;
/*0xC*/ static u8 _bss_C[0x10];

static int GPSH_Shrine_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void GPSH_Shrine_handleMessages(Object* self);

// offset: 0x0 | ctor
void GPSH_Shrine_ctor(void* dll) { }

// offset: 0xC | dtor
void GPSH_Shrine_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void GPSH_Shrine_obj_Setup(Object* self, GPSH_Shrine_Setup* setup, s32 reset) {
    GPSH_Shrine_Data* objdata;
    DLL_IModgfx* modgfxDLL;

    objdata = self->data;
    self->srt.yaw = 0;

    objdata->testStartRadius = 10;
    if (setup->testStartRadius > 0) {
        objdata->testStartRadius = setup->testStartRadius >> 8;
    }

    objdata->state = GPSH_Shrine_STATE_Waiting;
    objdata->seqValue = 0;
    objdata->musicPlayTimer = 0;
    self->animCallback = GPSH_Shrine_animCallback;
    objInitMesgQueue(self, 4);

    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
    mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
    mainSetBits(BIT_DB_Entered_Shrine_1, 1);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    mainSetBits(BIT_Test_of_Fear_Particles, 0);

    mainSetBits(BIT_GPSH_Placed_SW_Scene_Root, 0);
    mainSetBits(BIT_GPSH_Placed_CRF_Scene_Gem, 0);
    mainSetBits(BIT_GPSH_Placed_MMP_Scene_Barrel, 0);
    mainSetBits(BIT_GPSH_Placed_DIM_Scene_Horn, 0);
    mainSetBits(BIT_GPSH_Placed_SC_Scene_Nugget, 0);
    mainSetBits(BIT_GPSH_Placed_SB_Scene_Egg, 0);

    objdata->time = 0;
    objdata->volumeA = 0xC;
    objdata->volumeB = 0x1E;
    objdata->musicPlayTimer = 200;
    gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    objdata->volumeASpeed = 0;
    objdata->volumeBSpeed = 0;
    objdata->musicStarted = FALSE;
    objdata->unk19 = FALSE;

    //Create glowing circle around test's startpoint
    modgfxDLL = dllLoad(DLL_ID_122, 1);
    objdata->modGfxCircle = modgfxDLL->vtbl->Spawn(self, 2, 0, 0x402, -1, 0);
    dllFree(modgfxDLL);

    _bss_8 = 0.00001f;

    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
}

// offset: 0x290 | func: 1 | export: 1
void GPSH_Shrine_obj_Control(Object* self) {
    GPSH_Shrine_Data* objdata;
    Object* player;
    Object** pickupItems;
    DLL_IModgfx* modgfxDLL;
    Object* door;
    f32 distance;
    f32 dz;
    s16 vol;
    s32 count;

    objdata = self->data;
    player = objGetPlayer();
    count = 0;
    distance = 1000.0f;
    dz = 0.0f;
    if (player == NULL) {
        return;
    }
    
    //Shrink the player as they approach the scene dioramas
    if (mainGetBits(BIT_GPSH_Shrink_Player_Right_Dioramas)) {
        if (player->srt.scale >= 0.0265f) {
            player->srt.scale = 0.0495f - ((player->srt.transl.x + -16386.0f) * 0.0005f);
            diPrintf("scale %f %f  %f\n", &player->srt.scale, &dz, &player->srt.transl.x);
            if (player->srt.scale < 0.0265f) {
                player->srt.scale = 0.0265f;
            }
        }
    } else if (mainGetBits(BIT_GPSH_Shrink_Player_Left_Dioramas)) {
        if (player->srt.scale >= 0.0265f) {
            player->srt.scale = ((player->srt.transl.x + -15613.0f) * 0.0005f) + 0.0495f;
            diPrintf("scale %f %f  %f\n", &player->srt.scale, &dz, &player->srt.transl.x);
            if (player->srt.scale < 0.0265f) {
                player->srt.scale = 0.0265f;
            }
        }
    } else if (player->srt.scale != 0.0495f) {
        player->srt.scale = 0.0495f;
    }

    GPSH_Shrine_handleMessages(self);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);

    if (objdata->volumeASpeed) {
        objdata->volumeA += objdata->volumeASpeed;
        if (objdata->volumeA <= 0xC) {
            objdata->volumeA = 0xC;
            objdata->volumeASpeed = 0;
        } else if (objdata->volumeA >= 0x46) {
            objdata->volumeA = 0x46;
            objdata->volumeASpeed = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objdata->volumeA);
    }

    if (objdata->volumeBSpeed != 0) {
        objdata->volumeB += objdata->volumeBSpeed;
        if ((objdata->volumeB < 2) && (objdata->volumeBSpeed <= 0)) {
            objdata->volumeB = 1;
            objdata->volumeBSpeed = 0;
        }else if ((objdata->volumeB >= 0x46) && (objdata->volumeBSpeed >= 0)) {
            objdata->volumeB = 0x46;
            objdata->volumeBSpeed = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->volumeB);
    }

    if (objdata->musicPlayTimer > 0) {
        objdata->musicPlayTimer -= gUpdateRate;
        if (objdata->musicPlayTimer <= 0) {
            objdata->musicPlayTimer = 0;
            if (objdata->musicStarted == FALSE) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objdata->volumeB, 0);
                objdata->musicStarted = TRUE;
            }
        }

        if ((objdata->state == GPSH_Shrine_STATE_Test_of_Knowledge) && (objdata->musicPlayTimer <= 40) && (objdata->unk19 == FALSE)) {
            objdata->unk19 = TRUE;
        }
    } else {
        door = objGetNearestTypeTo(OBJTYPE_Door, player, &distance);
        if ((door != NULL) && (distance < 300.0f) && (distance > 100.0f)) {
            dz = door->srt.transl.z - player->srt.transl.z;
            if (dz <= 0.0f) {
                if (dz < 0.0f) {
                    dz *= -1.0f;
                }
                if (objdata->volumeB != 0x1E) {
                    objdata->volumeB = 0x1E;
                }
                vol = ((f32) objdata->volumeB * ((dz - 100.0f) / 200.0f));
                if (vol <= 0) {
                    vol = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, vol);
                vol = ((f32) objdata->volumeA * ((200.0f - (dz - 100.0f)) / 200.0f));
                if (vol <= 0) {
                    vol = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, vol);
            }
        }

        switch (objdata->state) {
        case GPSH_Shrine_STATE_Waiting:
            if (vec3Distance(&self->globalPosition, &player->globalPosition) < objdata->testStartRadius) {
                objdata->state = GPSH_Shrine_STATE_Test_Start;
                mainSetBits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);

                modgfxDLL = dllLoad(DLL_ID_147, 1);
                modgfxDLL->vtbl->Spawn(self, 2, 0, 1, -1, 0);
                dllFree(modgfxDLL);

                modgfxDLL = dllLoad(DLL_ID_148, 1);
                modgfxDLL->vtbl->Spawn(self, 0, 0, 1, -1, 0);
                dllFree(modgfxDLL);

                mainSetBits(BIT_DB_Entered_Shrine_1, 0);
                dll_modgfx->Func7(&objdata->modGfxCircle);
                objdata->modGfxCircle = -1;

                mainSetBits(BIT_5AF, 0);
            }
            break;
        case GPSH_Shrine_STATE_Test_Start:
            if (objdata->seqValue == 1) {
                mainSetBits(BIT_148, 1);
                objdata->state = GPSH_Shrine_STATE_Test_of_Knowledge;
                objdata->musicPlayTimer = 80;
                objdata->time = 15000;
                return;
            }
            break;
        case GPSH_Shrine_STATE_Test_of_Knowledge:
            objdata->time -= gUpdateRate;
            diPrintf("\ntime %d\n", objdata->time);
            objdata->itemsPlaced = 0;
            if (mainGetBits(BIT_GPSH_Placed_SW_Scene_Root)) {
                objdata->itemsPlaced++;
            }
            if (mainGetBits(BIT_GPSH_Placed_SB_Scene_Egg)) {
                objdata->itemsPlaced++;
            }
            if (mainGetBits(BIT_GPSH_Placed_DIM_Scene_Horn)) {
                objdata->itemsPlaced++;;
            }
            if (mainGetBits(BIT_GPSH_Placed_MMP_Scene_Barrel)) {
                objdata->itemsPlaced++;
            }
            if (mainGetBits(BIT_GPSH_Placed_CRF_Scene_Gem)) {
                objdata->itemsPlaced++;
            }
            if (mainGetBits(BIT_GPSH_Placed_SC_Scene_Nugget)) {
                objdata->itemsPlaced++;
            }
            if (objdata->itemsPlaced == 6) {
                objdata->state = GPSH_Shrine_STATE_Test_Successful;
                objdata->musicPlayTimer = 120;
                return;
            }

            if (objdata->time <= 0) {
                objdata->state = GPSH_Shrine_STATE_Test_Failure;
                pickupItems = objGetAllOfType(OBJTYPE_Pickup, &count);
                while (count != 0) {
                    objFreeObject(pickupItems[count - 1]);
                    count--;
                    if ((!objdata) && (!objdata)){} // @fake
                }
                gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
            } else {
                objdata->itemsPlaced = 0;
                return;
            }
            break;
        case GPSH_Shrine_STATE_Test_Successful:
            if (mainGetBits(BIT_SP_Map_MMP)) {
                objdata->volumeB = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->volumeB, 0);
                objdata->volumeBSpeed = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                objdata->state = GPSH_Shrine_STATE_Finished;
            } else {
                mainSetBits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->volumeB, 0);
                objdata->volumeBSpeed = 1;
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                objdata->state = GPSH_Shrine_STATE_Warp_Away;
            }
            return;
        case GPSH_Shrine_STATE_Warp_Away:
            if (mainGetBits(BIT_Shrine_Do_Exit_Warp) == 0) {
                mainSetBits(BIT_Shrine_Do_Exit_Warp, 1);
            }
            mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
            mainSetBits(BIT_DB_Entered_Shrine_2, 0);
            objdata->state = GPSH_Shrine_STATE_Finished;
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_SP_Map_MMP, 1);
            gDLL_29_Gplay->vtbl->set_act(MAP_WARLOCK_MOUNTAIN, 8);
            break;
        case GPSH_Shrine_STATE_Test_Failure:
            objdata->state = GPSH_Shrine_STATE_Waiting;
            objdata->seqValue = 0;
            objdata->musicPlayTimer = 400;
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_DB_Entered_Shrine_2, 1);
            modgfxDLL = dllLoad(DLL_ID_122, 1);
            objdata->modGfxCircle = modgfxDLL->vtbl->Spawn(self, 2, 0, 0x402, -1, 0);
            dllFree(modgfxDLL);
            mainSetBits(BIT_GPSH_Placed_SW_Scene_Root, 0);
            mainSetBits(BIT_GPSH_Placed_CRF_Scene_Gem, 0);
            mainSetBits(BIT_GPSH_Placed_MMP_Scene_Barrel, 0);
            mainSetBits(BIT_GPSH_Placed_DIM_Scene_Horn, 0);
            mainSetBits(BIT_GPSH_Placed_SC_Scene_Nugget, 0);
            mainSetBits(BIT_GPSH_Placed_SB_Scene_Egg, 0);
            mainSetBits(BIT_GPSH_Placed_SB_Scene_Egg, 0); //@bug: duplicate
            mainSetBits(BIT_5AF, 1);
            mainSetBits(BIT_148, 0);
            break;
        }
    }
}

// offset: 0xF10 | func: 2 | export: 2
void GPSH_Shrine_obj_Update(Object* self) { }

// offset: 0xF1C | func: 3 | export: 3
void GPSH_Shrine_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF70 | func: 4 | export: 4
void GPSH_Shrine_obj_Free(Object* self, s32 onlySelf) {
    dll_modgfx->Func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0x1000 | func: 5 | export: 5
u32 GPSH_Shrine_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1010 | func: 6 | export: 6
u32 GPSH_Shrine_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(GPSH_Shrine_Data);
}

// offset: 0x1024 | func: 7
static int GPSH_Shrine_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    GPSH_Shrine_Data* objdata;
    Object* player;
    s32 i;

    objdata = self->data;
    player = objGetPlayer();
    animData->unk7A = -1;
    animData->unk62 = 0;

    if (objdata->volumeBSpeed != 0) {
        objdata->volumeB += objdata->volumeBSpeed;
        if ((objdata->volumeB < 2) && (objdata->volumeBSpeed <= 0)) {
            objdata->volumeB = 1;
            objdata->volumeBSpeed = 0;
        } else if ((objdata->volumeB >= 0x46) && (objdata->volumeBSpeed >= 0)) {
            objdata->volumeB = 0x46;
            objdata->volumeBSpeed = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->volumeB);
    }

    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i] != 0) {
            switch (animData->messages[i]) {
            case 1:
                envfxAction(self, self, 0xCD, 0);
                break;
            case 2:
                if (D_80092A7C[0] == -1) {
                    envfxAction(self, self, 0x14, 0);
                } else {
                    envfxAction(self, self, D_80092A7C[0], 0);
                }
                break;
            case 3:
                objdata->seqValue = 1;
                break;
            case 4:
                objdata->state = GPSH_Shrine_STATE_Test_Successful;
                objdata->musicPlayTimer = 90;
                break;
            case 5:
                objdata->seqValue = 2;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                objdata->state = GPSH_Shrine_STATE_Warp_Away;
                objdata->seqValue = 3;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 7:
                mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                break;
            case 8:
                mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
                objdata->volumeBSpeed = -3;
                break;
            case 10:
                mainSetBits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (_data_0 == 0) {
                    _data_0 = blockTexanimGetTex(1);
                }
                break;
            case 9:
                mainSetBits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                objdata->volumeB = 0x64;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x30, 0x50, (u8)objdata->volumeB, 0);
                break;
            case 12:
                mainSetBits(BIT_Test_of_Fear_Particles, 0);
                break;
            }
        }
        animData->messages[i] = 0;
    }

    if ((objdata->state == GPSH_Shrine_STATE_3) && (objdata->testStartRadius < vec3Distance(&self->globalPosition, &player->globalPosition))) {
        gDLL_3_Animation->vtbl->end_obj_sequence(animData->seqSlot);
    }

    return 0;
}

// offset: 0x13E0 | func: 8
static void GPSH_Shrine_handleMessages(Object* self) {
    Object* mesgSender;
    u32 mesgID;
    void* mesg;
    GPSH_Shrine_Data* objdata;

    objdata = self->data;
    mesg = NULL;
    while (objRecvMesg(self, &mesgID, &mesgSender, &mesg) != 0) {
        switch (mesgID) {
        case 0x30005:
            objdata->volumeASpeed = -3;
            break;
        case 0x30006:
            objdata->volumeASpeed = 0x10;
            break;
        }
    }
}
