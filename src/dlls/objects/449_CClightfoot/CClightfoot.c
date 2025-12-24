#include "PR/ultratypes.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "segment_334F0.h"
#include "dll.h"
#include "functions.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1A-0x18];
/*1A*/ u8 yaw;
} CClightfoot_Setup;

typedef struct {
/*00*/ u8 spokeToPlayer;
/*01*/ u8 _unk1[0x4-0x1];
/*04*/ HeadAnimation unk4;
} CClightfoot_Data;

// @bug: no arg3?
static int CClightfoot_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData);

// offset: 0x0 | ctor
void CClightfoot_ctor(void *self) { }

// offset: 0xC | dtor
void CClightfoot_dtor(void *self) { }

// offset: 0x18 | func: 0 | export: 0
void CClightfoot_setup(Object *self, CClightfoot_Setup *setup, s32 arg2) {
    CClightfoot_Data *objdata;

    objdata = self->data;
    self->animCallback = (AnimationCallback)CClightfoot_anim_callback; // NOLINT
    objdata->spokeToPlayer = main_get_bits(BIT_Play_Seq_022F_CC_Lightfoot_Gives_Spellpage);
    self->srt.yaw = setup->yaw << 8;
    obj_add_object_type(self, OBJTYPE_40);
}

// offset: 0xAC | func: 1 | export: 1
void CClightfoot_control(Object *self) {
    CClightfoot_Data *objdata;
    Object *player;

    objdata = self->data;
    player = get_player();
    if (!objdata->spokeToPlayer) {
        objdata->unk4.headAimX = player->srt.transl.x;
        objdata->unk4.headAimY = player->srt.transl.y;
        objdata->unk4.headAimZ = player->srt.transl.z;
        objdata->unk4.aimIsActive = 1;
        func_800328F0(self, &objdata->unk4, 0.0f);
        func_80032A08(self, &objdata->unk4);
        if (self->curModAnimId != 10) {
            func_80023D30(self, 10, 0.0f, 0);
        }
        func_80024108(self, 0.005f, gUpdateRateF, NULL);
        if (main_get_bits(BIT_CC_Shot_Down_Lightfoot_Cage)) {
            self->unkAF &= ~0x8;
            if (self->unkAF & 1) {
                main_set_bits(BIT_Play_Seq_022F_CC_Lightfoot_Gives_Spellpage, 1);
                objdata->spokeToPlayer = TRUE;
            }
        }
    }
}

// offset: 0x1F4 | func: 2 | export: 2
void CClightfoot_update(Object *self) { }

// offset: 0x200 | func: 3 | export: 3
void CClightfoot_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x254 | func: 4 | export: 4
void CClightfoot_free(Object *self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_40);
}

// offset: 0x294 | func: 5 | export: 5
u32 CClightfoot_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2A4 | func: 6 | export: 6
u32 CClightfoot_get_data_size(Object *self, u32 arg1) {
    return sizeof(CClightfoot_Data);
}

// offset: 0x2B8 | func: 7
static int CClightfoot_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData) {
    CClightfoot_Data *objdata = self->data;
    if (self->unkAF & 1) {
        gDLL_3_Animation->vtbl->func18(animObjData->unk63);
        main_set_bits(BIT_Play_Seq_022F_CC_Lightfoot_Gives_Spellpage, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        objdata->spokeToPlayer = TRUE;
    } else {
        self->unkAF &= ~1;
    }
    return 0;
}
