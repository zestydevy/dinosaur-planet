#include "PR/gbi.h"
#include "PR/ultratypes.h"

#include "dlls/objects/223_CannonClaw.h"
#include "dlls/objects/537_DIMcannon.h"
#include "dlls/objects/common/sidekick.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "types.h"

static void CannonClaw_die(Object* self);

// offset: 0x0 | ctor
void CannonClaw_ctor(void* dll) { }

// offset: 0xC | dtor
void CannonClaw_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CannonClaw_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    self->srt.yaw = -M_180_DEGREES;
    self->srt.transl.y = setup->y + 2.0f;  
}

// offset: 0x4C | func: 1 | export: 1
void CannonClaw_obj_Control(Object* self) {
    DIMCannon_Setup* cannonSetup;
    Object* sidekick;

    sidekick = objGetSidekick();
    
    //Do nothing when not being drawn
    if (self->unkDC) {
        return;
    }

    //Handle animation
    {
        if (self->curModAnimId != 0x208) {
            objAnimSet(self, 0x208, 0, 0);
        }
        objAnimAdvance(self, 0.005f, gUpdateRateF, NULL);
    }

    //Handle damage
    if (func_80025F40(self, NULL, 0, 0)) {
        if (self->parent) {
            cannonSetup = (DIMCannon_Setup*)self->parent->setup;
            mainSetBits(cannonSetup->gamebitCannonClawDead, TRUE);

            //@bug: missing null check for the sidekick
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, NULL);
        }
        
        CannonClaw_die(self);
        return;
    }

    //Check if cannon's gamebit is set
    if (self->parent) {
        cannonSetup = (DIMCannon_Setup*)self->parent->setup;
        if (mainGetBits(cannonSetup->gamebitCannonClawDead)) {
            CannonClaw_die(self);
        }
    }
}

// offset: 0x1B4 | func: 2
static void CannonClaw_die(Object* self) {
    //Stop being drawn
    self->unkDC = 1;

    //Remove targetting and collision
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    func_800267A4(self);
}

// offset: 0x200 | func: 3 | export: 2
void CannonClaw_obj_Update(Object* self) { }

// offset: 0x20C | func: 4 | export: 3
void CannonClaw_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility && self->unkDC == 0) {
		objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
	}
}

// offset: 0x26C | func: 5 | export: 4
void CannonClaw_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x27C | func: 6 | export: 5
u32 CannonClaw_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x28C | func: 7 | export: 6
u32 CannonClaw_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
