#include "dll.h"
#include "game/objects/setups/39_Warp.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ s16 warpDelayTimer;
/*02*/ s16 gamebit;
/*04*/ s16 objectSeqIndex;
/*08*/ f32 range;
/*0C*/ u8 objSeqPlayed;
} WarpPoint_Data;

typedef enum {
    WarpPoint_OBJSEQ_0, //0x81 (outbound) (FADE NOT CHECKED) Warp immediately via objSeq message, has unseen player anims too (curling into ball?)
    WarpPoint_OBJSEQ_1, //0x82 (inbound) (no fade) Player curled and rolling in midair, uncurls and lands
    WarpPoint_OBJSEQ_2, //0x15E (inbound) (FADE NOT CHECKED) Player spat out during Galadon battle #1
    WarpPoint_OBJSEQ_3, //0x196 (inbound) (FADE NOT CHECKED) Player landing in Galadon's stomach
    WarpPoint_OBJSEQ_4, //0x15F (inbound) (FADE NOT CHECKED) Player spat out during Galadon battle #2
    WarpPoint_OBJSEQ_5, //0x1C3 (inbound) (FADE NOT CHECKED) Rocky swapping to Sabre (unused early draft?)
    WarpPoint_OBJSEQ_6, //0x2CE (inbound) (fade in) Sabre and Tricky thrown out of DarkIce Mines
    WarpPoint_OBJSEQ_7, //0x382 (inbound) (no fade) Sabre falling through vertical chute and crashing to ground in Lower Dragon Rock
    WarpPoint_OBJSEQ_8, //0x429 (inbound) (fade in) Sabre and Tricky walking towards camera 
    WarpPoint_OBJSEQ_9  //0x447 (inbound) (fade in) Player walks in towards camera, camera cuts behind them
} WarpPoint_ObjSeqs; //NOTE: SC_WarpPoint, WM_WarpPoint, and RestartPoint have different objSeqs

typedef enum {
    WarpPoint_MODE_0, //Play an arrival objSeq (fade in); warp when nearby (fade out) (ignores delay timer)
    WarpPoint_MODE_1, //Play arrival objSeq1 (uncurling from ball) (fade in); warp via objSeq0 (no fade out) (use delay timer)
    WarpPoint_MODE_2, //Play an arrival objSeq if a gamebit it set (no fade in); warp and unset gamebit if gamebit set (no fade out) (checks delay timer, but ignores it in practice by setting it to 0)
    WarpPoint_MODE_3, //(Inbound only) Play an arrival objSeq if a gamebit is set (no fade in) and unset the gamebit; no warp away
    WarpPoint_MODE_4  //Play an arrival objSeq if a warpID is specified (fade in); warp if a gamebit is set, unsetting gamebit (fade out) (uses delay timer)
} WarpPoint_Modes;

static int WarpPoint_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void WarpPoint_ctor(void* dll) { }

// offset: 0xC | dtor
void WarpPoint_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WarpPoint_obj_Setup(Object* self, Warp_Setup* setup, s32 reset) {
    WarpPoint_Data* objdata = self->data;

    self->animCallback = WarpPoint_animCallback;
    self->srt.yaw = setup->yaw << 8;

    objdata->warpDelayTimer = 30;
    objdata->range = setup->quarterRange * 4;
    objdata->gamebit = setup->gamebit;
    objdata->objectSeqIndex = setup->objectSeqIndex;

    if (setup->isInboundWarp) {
        objdata->objSeqPlayed = FALSE;
    } else {
        objdata->objSeqPlayed = TRUE;
    }

    if (setup->mode == WarpPoint_MODE_2) {
        objdata->warpDelayTimer = 0;
    }
}

// offset: 0xAC | func: 1 | export: 1
void WarpPoint_obj_Control(Object* self) {
    Object* player;
    Warp_Setup* setup;
    WarpPoint_Data* objdata;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;

    setup = (Warp_Setup*)self->setup;
    objdata = self->data;
    player = objGetPlayer();

    //Don't allow warping until a timer has run out (for modes 1, 4)
    objdata->warpDelayTimer -= gUpdateRate;
    if (objdata->warpDelayTimer < 0) {
        objdata->warpDelayTimer = 0;
    }

    switch (setup->mode) {
    case WarpPoint_MODE_0:
        //If this is an inbound Warp Point, play the specified arrival objSeq when the player arrives nearby (variable range)
        if (D_800B4A5E > NO_WARP_ID) {
            dx = player->srt.transl.x - self->srt.transl.x;
            dy = player->srt.transl.y - self->srt.transl.y;
            dz = player->srt.transl.z - self->srt.transl.z;
            dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
            if ((objdata->objSeqPlayed == FALSE) && setup->isInboundWarp && (dist < objdata->range) && (player->parent == self->parent)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objdata->objectSeqIndex, self, -1);
                D_80092A78 = 2; //Fade in from black
                objdata->objSeqPlayed = TRUE;
            }
        }

        //If a warpID is provided, fade to black and warp the player when they're nearby (variable range)
        if ((setup->warpID > NO_WARP_ID) && (vec3Distance(&self->globalPosition, &player->globalPosition) < objdata->range)) {
            mapWarpPlayer(setup->warpID, TRUE);
        }
        break;
    case WarpPoint_MODE_1:
        dx = player->srt.transl.x - self->srt.transl.x;
        dy = player->srt.transl.y - self->srt.transl.y;
        dz = player->srt.transl.z - self->srt.transl.z;
        dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));

        //If this is an inbound Warp Point, play objSeq1 (player uncurling from ball) when the player arrives nearby (fixed range of 100)
        if ((D_800B4A5E > NO_WARP_ID) && setup->isInboundWarp && (dist < 100.0f) && (player->parent == self->parent)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(WarpPoint_OBJSEQ_1, self, -1);
            D_80092A78 = 2; //Fade in from black
        }

        //If a warpID is provided, play objSeq0 and warp via its anim message when the player is nearby (variable range/4) (waits for timer to expire)
        if ((objdata->warpDelayTimer == 0) && (dist < setup->quarterRange)) {
            if ((setup->warpID > NO_WARP_ID) && (setup->warpID > NO_WARP_ID)) { //@bug: accidentally pasted condition twice?
                gDLL_3_Animation->vtbl->start_obj_sequence(WarpPoint_OBJSEQ_0, self, -1);
            }
        }
        break;
    case WarpPoint_MODE_2:
        if (objdata->range != 0.0f) {
            dx = player->globalPosition.x - self->globalPosition.x;
            dy = player->globalPosition.y - self->globalPosition.y;
            dz = player->globalPosition.z - self->globalPosition.z;
            dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        } else {
            dist = objdata->range;
        }

        //If this is an inbound Warp Point, play the specified arrival objSeq when the player's nearby and a specified gamebit is set (variable range)
        if (mainGetBits(objdata->gamebit) && (objdata->objSeqPlayed == FALSE) && setup->isInboundWarp && (dist <= objdata->range) && (player->parent == self->parent)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objdata->objectSeqIndex, self, -1);
            objdata->objSeqPlayed = TRUE;
        } else if (objdata->objSeqPlayed == TRUE) {
        //If a warpID is provided and the arrival sequence played, warp the player immediately (without fade) when they're nearby and unset the gamebit (variable range)
        //(Waits for timer to expire, but obj_Setup has the timer expire immediately when using this mode!)
            if ((mainGetBits(objdata->gamebit)) && (objdata->warpDelayTimer == 0) && (dist <= objdata->range) && (setup->warpID > NO_WARP_ID)) {
                mainSetBits(objdata->gamebit, FALSE);
                mapWarpPlayer(setup->warpID, FALSE);
            }
        }
        break;
    case WarpPoint_MODE_3:
        dx = player->srt.transl.x - self->srt.transl.x;
        dy = player->srt.transl.y - self->srt.transl.y;
        dz = player->srt.transl.z - self->srt.transl.z;
        dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));

        //If this is an inbound Warp Point, play the specified arrival objSeq when the player's nearby and a specified gamebit is set (variable range)
        if (mainGetBits(objdata->gamebit) && (objdata->objSeqPlayed == FALSE) && setup->isInboundWarp && (dist < objdata->range) && (player->parent == self->parent)) {
            mainSetBits(objdata->gamebit, FALSE);
            gDLL_3_Animation->vtbl->start_obj_sequence(objdata->objectSeqIndex, self, -1);
            objdata->objSeqPlayed = TRUE;
        }

        //NOTE: no outbound warp in this mode (unless objectSeqIndex is 0, activating the animCallback function's warp)
        break;
    case WarpPoint_MODE_4:
        if (objdata->range != 0.0f) {
            dx = player->globalPosition.x - self->globalPosition.x;
            dy = player->globalPosition.y - self->globalPosition.y;
            dz = player->globalPosition.z - self->globalPosition.z;
            dist = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
        } else {
            dist = objdata->range;
        }

        //If this is an inbound Warp Point, play the specified arrival objSeq when the player arrives nearby (variable range)
        if ((D_800B4A5E > NO_WARP_ID) && (objdata->objSeqPlayed == FALSE) && setup->isInboundWarp && (dist < objdata->range) && (player->parent == self->parent)) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objdata->objectSeqIndex, self, -1);
            D_80092A78 = 2; //Fade in from black
            objdata->objSeqPlayed = TRUE;
        }

        //If a specified gamebit is set and the timer's expired and warpID is provided, unset the gamebit and fade out and warp the player when they're nearby (variable range)
        if (mainGetBits(objdata->gamebit) && (objdata->warpDelayTimer == 0) && (dist <= objdata->range) && (setup->warpID > NO_WARP_ID)) {
            mainSetBits(objdata->gamebit, FALSE);
            mapWarpPlayer(setup->warpID, TRUE);
        }
        break;
    }
}

// offset: 0x83C | func: 2 | export: 2
void WarpPoint_obj_Update(Object* self) { }

// offset: 0x848 | func: 3 | export: 3
void WarpPoint_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        if (self) { }
    }
}

// offset: 0x868 | func: 4 | export: 4
void WarpPoint_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x878 | func: 5 | export: 5
u32 WarpPoint_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x888 | func: 6 | export: 6
u32 WarpPoint_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WarpPoint_Data);
}

// offset: 0x89C | func: 7
int WarpPoint_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 prevCallbackValue) {
    Warp_Setup* setup = (Warp_Setup*)self->setup;

    //Warp the player (with a fade-to-black) via animMessage1
    if ((setup->mode != WarpPoint_MODE_2) && (animObjData->lastMessage == 1)) {
        if (setup->warpID >= 0) {
            mapWarpPlayer(setup->warpID, TRUE);
            animObjData->lastMessage = 0;
        }
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "\tbit %d  set %d %d %d %d %d %f %f %x\n";
/*0x28*/ static const char str_28[] = "\tdoseq warp %x %d\n";
/*0x3C*/ static const char str_3C[] = "\tdowarp warp %x %d\n";
