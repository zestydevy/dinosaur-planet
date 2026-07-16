#include "PR/gbi.h"
#include "PR/ultratypes.h"

#include "dlls/objects/223_cannon_claw.h"
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

static void cannon_claw_die(Object* self);

// offset: 0x0 | ctor
void cannon_claw_ctor(void* dll) { }

// offset: 0xC | dtor
void cannon_claw_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void cannon_claw_setup(Object* self, ObjSetup* setup, s32 reset) {
    self->srt.yaw = -M_180_DEGREES;
    self->srt.transl.y = setup->y + 2.0f;  
}

// offset: 0x4C | func: 1 | export: 1
void cannon_claw_control(Object* self) {
    DIMCannon_Setup* cannonSetup;
    Object* sidekick;

    sidekick = get_sidekick();
    
    if (self->unkDC) {
        return;
    }

    //Handle animation
    {
        if (self->curModAnimId != 0x208) {
            func_80023D30(self, 0x208, 0, 0);
        }
        func_80024108(self, 0.005f, gUpdateRateF, NULL);
    }

    //Handle damage
    if (func_80025F40(self, NULL, 0, 0)) {
        if (self->parent) {
            cannonSetup = (DIMCannon_Setup*)self->parent->setup;
            main_set_bits(cannonSetup->gamebit, TRUE);

            //@bug: missing null check for the sidekick
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, NULL);
        }
        
        cannon_claw_die(self);
        return;
    }

    //Check if cannon's gamebit is set
    if (self->parent) {
        cannonSetup = (DIMCannon_Setup*)self->parent->setup;
        if (main_get_bits(cannonSetup->gamebit)) {
            cannon_claw_die(self);
        }
    }
}

// offset: 0x1B4 | func: 2
static void cannon_claw_die(Object* self) {
    //Stop being drawn
    self->unkDC = 1;

    //Remove targetting and collision
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    func_800267A4(self);
}

// offset: 0x200 | func: 3 | export: 2
void cannon_claw_update(Object* self) { }

// offset: 0x20C | func: 4 | export: 3
void cannon_claw_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility && self->unkDC == 0) {
		draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
	}
}

// offset: 0x26C | func: 5 | export: 4
void cannon_claw_free(Object* self, s32 onlySelf) { }

// offset: 0x27C | func: 6 | export: 5
u32 cannon_claw_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x28C | func: 7 | export: 6
u32 cannon_claw_get_data_size(Object* self, u32 offsetAddr) {
    return 0;
}
