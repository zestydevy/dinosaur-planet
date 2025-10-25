#include "PR/gbi.h"
#include "PR/ultratypes.h"

#include "dlls/objects/223_cannon_claw.h"
#include "dlls/objects/common/sidekick.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "functions.h"
#include "types.h"

static void cannon_claw_func_1B4(Object *self);

void cannon_claw_ctor(void *dll) { }

void cannon_claw_dtor(void *dll) { }

void cannon_claw_setup(Object *self, ObjSetup *setup, s32 a2) {
    self->srt.yaw = -32768;
    self->srt.transl.y = setup->y + 2.0f;  
}

void cannon_claw_control(Object *self) {
    s32 temp;
    Object *sidekick;

    sidekick = get_sidekick();
    
    if (self->unkDC != 0) {
        return;
    }

    if (self->curModAnimId != 0x208) {
        func_80023D30(self, 0x208, 0, 0);
    }

    func_80024108(self, 0.0049999998f, delayFloat, NULL);
    temp = func_80025F40(self, NULL, 0, 0);
    
    if (temp != 0) {
        if (self->parent != NULL) {
            CannonClaw_Setup *setup = (CannonClaw_Setup*)self->parent->setup;
            main_set_bits(setup->gamebit, 1);
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, 0);
        }
        
        cannon_claw_func_1B4(self);
    } else {
        if (self->parent != NULL) {
            CannonClaw_Setup *setup = (CannonClaw_Setup*)self->parent->setup;
            if (main_get_bits(setup->gamebit) != 0) {
                cannon_claw_func_1B4(self);
            }
        }
    }
}

static void cannon_claw_func_1B4(Object *self) {
    self->unkDC = 1;
    self->unkAF = self->unkAF | 8;    
    func_800267A4(self);
}

void cannon_claw_update(Object *self) { }

void cannon_claw_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0 && self->unkDC == 0) {
		draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
	}
}

void cannon_claw_free(Object *self, s32 a1) { }

u32 cannon_claw_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

u32 cannon_claw_get_data_size(Object *self, u32 a1) {
    return 0;
}
