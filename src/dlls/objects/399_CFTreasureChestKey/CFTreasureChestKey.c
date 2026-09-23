#include "dlls/objects/217_GuardClaw.h"
#include "game/objects/interaction_arrow.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1A - 0x18];
/*1A*/ u8 scale; // scale * 64
/*1B*/ u8 yaw8;
/*1C*/ s16 itemGamebit; // treasure key inventory item bit
/*1E*/ u8 _unk1E[0x22 - 0x1E];
/*22*/ u8 pitch8;
/*23*/ u8 roll8;
/*24*/ s16 guardClawDefeatedGamebit;
} CFTreasureChestKey_Setup;

typedef struct {
    u8 guardClawAlive : 1;
    u8 freeKey : 1;
    u8 keyGiven : 1;
    u8 guardClawDisabled : 1;
} CFTreasureChestKey_Data;

static void CFTreasureChestKey_guardClawPrintCallback(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, Object* self, Object* guardClaw);

// offset: 0x0 | ctor
void CFTreasureChestKey_ctor(void* dll) { }

// offset: 0xC | dtor
void CFTreasureChestKey_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFTreasureChestKey_obj_Setup(Object* self, CFTreasureChestKey_Setup* setup, s32 reset) {
    CFTreasureChestKey_Data* objdata = self->data;
    
    objAddObjectType(self, OBJTYPE_32);
    self->unkDC = 0;
    self->objhitInfo->unk58 &= ~1;
    self->srt.yaw = setup->yaw8 << 8;
    self->srt.pitch = setup->pitch8 << 8;
    self->srt.roll = setup->roll8 << 8;
    objdata->guardClawAlive = TRUE;
    objdata->freeKey = objdata->keyGiven = FALSE;
    self->srt.scale = (f32) setup->scale * (1.0f / 64.0f);
    self->srt.scale *= self->def->scale;
    if (mainGetBits(setup->guardClawDefeatedGamebit)) {
        objdata->freeKey = TRUE;
    }
    if (mainGetBits(setup->itemGamebit)) {
        objDisable(self);
        objdata->keyGiven = TRUE;
    }
}

// offset: 0x19C | func: 1 | export: 1
void CFTreasureChestKey_obj_Control(Object* self) {
    CFTreasureChestKey_Setup* setup = (CFTreasureChestKey_Setup*)self->setup;
    CFTreasureChestKey_Data* objdata = self->data;
    Object* player = objGetPlayer();
    s32 _pad;
    Object* guardClaw;
    f32 dist = 2000.0f;
    
    if (self->unkDC >= 2) {
        if (self->unkDC == 2) {
            self->unkDC = 0;
        } else {
            self->unkDC--;
        }
    }
    if (objdata->freeKey) {
        if (!objdata->guardClawDisabled) {
            // Disable the GuardClaw as soon as they're done fading out from the cutscene
            guardClaw = objGetNearestTypeTo(OBJTYPE_Baddie, self, &dist);
            if ((guardClaw != NULL) && (guardClaw->opacity == 0)) {
                // @bug: need to confirm this pointer is actually a GuardClaw! not all baddie DLLs implement this
                ((DLL_217_GuardClaw*)guardClaw->dll)->vtbl->SetItem(guardClaw, NULL, NULL);
                objDisable(guardClaw);
                guardClaw->srt.flags |= OBJFLAG_INVISIBLE;
                guardClaw->stateFlags |= OBJSTATE_UPDATE_DISABLED;
                guardClaw->stateFlags |= OBJSTATE_CONTROL_DISABLED;
                func_800267A4(guardClaw);
                objFreeObjectType(guardClaw, OBJTYPE_Baddie);
                objdata->guardClawDisabled = TRUE;
            }
        }
        self->objhitInfo->unk58 |= 1;
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
        if (self->unkAF & ARROW_FLAG_1_Interacted) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            // @bug: need to disable A-button after iteract
            // Give player the key
            objdata->keyGiven = TRUE;
            mainSetBits(setup->itemGamebit, 1);
            STUBBED_PRINTF(" Bit %i Set\n", setup->itemGamebit);
            objSendMesg(player, 0x7000A, self, (void* )0x10000);
        }
    }
    if (!objdata->freeKey && !objdata->guardClawAlive) {
        // time to crush GuardClaw
        STUBBED_PRINTF(" Should be Starting Sequence ");
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objdata->freeKey = TRUE;
    }
}

// offset: 0x3C0 | func: 2 | export: 2
void CFTreasureChestKey_obj_Update(Object* self) { }

// offset: 0x3CC | func: 3 | export: 3
void CFTreasureChestKey_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFTreasureChestKey_Data* objdata;
    CFTreasureChestKey_Setup* setup;
    Object* guardClaw;
    f32 dist;

    setup = (CFTreasureChestKey_Setup*)self->setup;
    objdata = self->data;
    dist = 2000.0f;
    if (!mainGetBits(setup->guardClawDefeatedGamebit)) {
        // GuardClaw not defeated yet, attach to their model
        guardClaw = objGetNearestTypeTo(OBJTYPE_Baddie, self, &dist);
        // @bug: we need to confirm this is actually a GuardClaw pointer????? not all baddie DLLs implement this!
        if (guardClaw != NULL) {
            ((DLL_217_GuardClaw*)guardClaw->dll)->vtbl->SetItem(guardClaw, self, CFTreasureChestKey_guardClawPrintCallback);
            return;
        }
    } else {
        objdata->guardClawAlive = FALSE;
    }
    if ((visibility != 0) && !objdata->keyGiven) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4E0 | func: 4
static void CFTreasureChestKey_guardClawPrintCallback(Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, Object* self, Object* guardClaw) {
    objGetAttachPointWorldSpace(guardClaw, 2, &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z, 0);
    self->srt.yaw = guardClaw->srt.yaw + 0x4000;
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x584 | func: 5 | export: 4
void CFTreasureChestKey_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_32);
}

// offset: 0x5C4 | func: 6 | export: 5
u32 CFTreasureChestKey_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x5D4 | func: 7 | export: 6
u32 CFTreasureChestKey_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFTreasureChestKey_Data);
}
