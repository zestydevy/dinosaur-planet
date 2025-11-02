#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "sys/objects.h"
#include "functions.h"
#include "game/gamebits.h"

typedef struct {
/*00*/ u8 state;
/*01*/ u8 unk1;
/*02*/ u8 objectSeqIndex;
/*04*/ Object *positionObj;
} CCguardgiving_Data;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4
} CCguardgiving_State;

// offset: 0x0 | ctor
void CCguardgiving_ctor(void *self) { }

// offset: 0xC | dtor
void CCguardgiving_dtor(void *self) { }

// offset: 0x18 | func: 0 | export: 0
void CCguardgiving_setup(Object *self, ObjSetup *setup, s32 arg2) {
    CCguardgiving_Data *objdata = self->data;

    objdata->positionObj = NULL;
    if (main_get_bits(BIT_82F)) {
        objdata->state = STATE_4;
        objdata->unk1 = 1;
    } else if (main_get_bits(BIT_CC_Bribed_GuardClaw)) {
        objdata->state = STATE_2;
        objdata->unk1 = 1;
    } else {
        objdata->state = STATE_0;
        objdata->unk1 = 0;
    }
    objdata->objectSeqIndex = 0;
}

// offset: 0xCC | func: 1 | export: 1
void CCguardgiving_control(Object *self) {
    f32 distance = 100.0f;
    s32 count;
    CCguardgiving_Data *objdata = self->data;
    Object *player;
    Object *obj;

    if (!objdata->positionObj) {
        obj = obj_get_nearest_type_to(OBJTYPE_4, self, &distance);
        objdata->positionObj = obj;
        if (obj == NULL)
            return;
        func_80023BF8(self, 0, 0, 0, 0, 2);
    }

    self->srt.transl.x = objdata->positionObj->srt.transl.x;
    self->srt.transl.y = objdata->positionObj->srt.transl.y;
    self->srt.transl.z = objdata->positionObj->srt.transl.z;
    self->srt.yaw = objdata->positionObj->srt.yaw;
    self->srt.roll = objdata->positionObj->srt.roll;
    self->srt.pitch = objdata->positionObj->srt.pitch;

    if (objdata->unk1) {
        gDLL_3_Animation->vtbl->func20(self, 0x20D);
        gDLL_3_Animation->vtbl->func17(2, self, 2);
        objdata->unk1 = 0;
        return;
    }

    objdata->positionObj->unkAF |= 0x20;

    switch (objdata->state) {
    case STATE_0:
        if (func_80032538(self)) {
            gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
            if (objdata->objectSeqIndex == 0) {
                objdata->objectSeqIndex = 1;
            } else {
                objdata->objectSeqIndex = 0;
            }
        } else if (gDLL_1_UI->vtbl->func_F24() == 3) {
            objdata->state = STATE_1;
            func_80023BF8(self, 0, 0, 0, 0, 3);
        }
        break;
    case STATE_1:
        if (gDLL_1_UI->vtbl->func_DF4(BIT_Gold_Nugget_CC)) {
            gDLL_3_Animation->vtbl->func17(2, self, -1);
            objdata->state = STATE_2;
            func_80023BF8(self, 0, 0, 0, 0, 2);
            main_set_bits(BIT_CC_Bribed_GuardClaw, 1);
        } else if (gDLL_1_UI->vtbl->func_DF4(BIT_3E4)) {
            gDLL_3_Animation->vtbl->func17(6, self, -1);
        } else if (gDLL_1_UI->vtbl->func_F24() != 3) {
            objdata->state = STATE_0;
            func_80023BF8(self, 0, 0, 0, 0, 2);
        }
        break;
    case STATE_2:
        if (func_80032538(self)) {
            gDLL_3_Animation->vtbl->func17(3, self, -1);
        } else if (gDLL_1_UI->vtbl->func_F24() == 3) {
            objdata->state = STATE_3;
            func_80023BF8(self, 0, 0, 0, 0, 3);
        }
        break;
    case STATE_3:
        if (gDLL_1_UI->vtbl->func_DF4(BIT_3E4)) {
            player = get_player();
            count = ((DLL_210_Player*)player->dll)->vtbl->get_scarab_count(player);
            if (count >= 10) {
                ((DLL_210_Player*)player->dll)->vtbl->set_scarab_count(player, count - 10);
                gDLL_3_Animation->vtbl->func17(4, self, -1);
                objdata->state = STATE_4;
                func_80023BF8(self, 0, 0, 0, 0, 2);
                main_set_bits(BIT_82F, 1);
            }
        } else if (gDLL_1_UI->vtbl->func_F24() != 3) {
            objdata->state = STATE_2;
            func_80023BF8(self, 0, 0, 0, 0, 2);
        }
        break;
    case STATE_4:
        if (func_80032538(self)) {
            gDLL_3_Animation->vtbl->func17(5, self, -1);
        }
        break;
    }
}

// offset: 0x66C | func: 2 | export: 2
void CCguardgiving_update(Object *self) {
    if (self->def->flags & 1 && self->unk74) {
        func_80036438(self);
    }
}

// offset: 0x6C4 | func: 3 | export: 3
void CCguardgiving_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x6DC | func: 4 | export: 4
void CCguardgiving_free(Object *self, s32 arg1) { }

// offset: 0x6EC | func: 5 | export: 5
u32 CCguardgiving_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6FC | func: 6 | export: 6
u32 CCguardgiving_get_data_size(Object *self, u32 arg1) {
    return sizeof(CCguardgiving_Data);
}
