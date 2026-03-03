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
/*01*/ u8 stepAside;            //GuardClaw moves out of the way when this is set
/*02*/ u8 objectSeqIndex;       //index to play when chatting to GuardClaw
/*04*/ Object *guardClaw;       //the GuardClaw object this "CCguardgiving" object is controlling
} CCguardgiving_Data;

typedef enum {
    STATE_Waiting_for_Bribe,
    STATE_Bribe_Attempt,
    STATE_Waiting_to_Sell_Map,
    STATE_Map_Sale_Attempt,
    STATE_Finished
} CCguardgiving_State;

typedef enum {
    SeqIndex_Chat_Bribe_Wanted_1 = 0,          //0x21E "Nobody enter Cape Claw! Unless maybe girl have something to give?"
    SeqIndex_Chat_Bribe_Wanted_2 = 1,          //0x222 "Me like shiny things! You not get past."
    SeqIndex_Bribed_with_Gold = 2,             //0x36F *takes gold* "You pass, but don't tell boss!" (shoves player forward with staff)
    SeqIndex_Chat_Map = 3,                     //0x21F "I'll sell you a map for 10 scarabs!"
    SeqIndex_Selling_Map = 4,                  //0x36E *takes Scarabs (shown as gold) and gives map*
    SeqIndex_Chat_End = 5,                     //0x3C5 "You pass, but don't tell boss!" (without shoving player forward)
    SeqIndex_Bribe_with_Scarabs_Refused = 6    //0x40D "I want more than just Scarabs!" (no MP3, sadly)
} CCguardgiving_SequenceIndices;

// offset: 0x0 | ctor
void CCguardgiving_ctor(void *self) { }

// offset: 0xC | dtor
void CCguardgiving_dtor(void *self) { }

// offset: 0x18 | func: 0 | export: 0
void CCguardgiving_setup(Object *self, ObjSetup *setup, s32 arg2) {
    CCguardgiving_Data *objdata = self->data;

    objdata->guardClaw = NULL;
    if (main_get_bits(BIT_Map_CC)) {
        objdata->state = STATE_Finished;
        objdata->stepAside = TRUE;
    } else if (main_get_bits(BIT_CC_Bribed_GuardClaw)) {
        objdata->state = STATE_Waiting_to_Sell_Map;
        objdata->stepAside = TRUE;
    } else {
        objdata->state = STATE_Waiting_for_Bribe;
        objdata->stepAside = FALSE;
    }
    objdata->objectSeqIndex = SeqIndex_Chat_Bribe_Wanted_1;
}

// offset: 0xCC | func: 1 | export: 1
void CCguardgiving_control(Object *self) {
    f32 distance = 100.0f;
    s32 count;
    CCguardgiving_Data *objdata = self->data;
    Object *player;
    Object *guardClaw;

    //Find the GuardClaw object
    if (!objdata->guardClaw) {
        guardClaw = obj_get_nearest_type_to(OBJTYPE_4, self, &distance);
        objdata->guardClaw = guardClaw;
        if (guardClaw == NULL)
            return;
        func_80023BF8(self, 0, 0, 0, 0, 2);
    }

    //Copy GuardClaw's transform
    self->srt.transl.x = objdata->guardClaw->srt.transl.x;
    self->srt.transl.y = objdata->guardClaw->srt.transl.y;
    self->srt.transl.z = objdata->guardClaw->srt.transl.z;
    self->srt.yaw = objdata->guardClaw->srt.yaw;
    self->srt.roll = objdata->guardClaw->srt.roll;
    self->srt.pitch = objdata->guardClaw->srt.pitch;

    //Step aside if already bribed (set by setup function based on gamebits)
    if (objdata->stepAside) {
        gDLL_3_Animation->vtbl->func20(self, 0x20D);
        gDLL_3_Animation->vtbl->func17(2, self, 2);
        objdata->stepAside = FALSE;
        return;
    }

    objdata->guardClaw->unkAF |= 0x20;

    switch (objdata->state) {
    case STATE_Waiting_for_Bribe:
        if (func_80032538(self)) {
            //When talked to, play one of two different chat sequences
            gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
            if (objdata->objectSeqIndex == SeqIndex_Chat_Bribe_Wanted_1) {
                objdata->objectSeqIndex = SeqIndex_Chat_Bribe_Wanted_2;
            } else {
                objdata->objectSeqIndex = SeqIndex_Chat_Bribe_Wanted_1;
            }
        } else if (gDLL_1_UI->vtbl->func_F24() == PAGE_Inventory) {
            //Go to bribe attempt state if the object inventory is open
            objdata->state = STATE_Bribe_Attempt;
            func_80023BF8(self, 0, 0, 0, 0, 3);
        }
        break;
    case STATE_Bribe_Attempt:
        if (gDLL_1_UI->vtbl->func_DF4(BIT_Gold_Nugget_CC)) {
            //Accepting gold nugget
            gDLL_3_Animation->vtbl->func17(SeqIndex_Bribed_with_Gold, self, -1);
            objdata->state = STATE_Waiting_to_Sell_Map;
            func_80023BF8(self, 0, 0, 0, 0, 2);
            main_set_bits(BIT_CC_Bribed_GuardClaw, 1);
        } else if (gDLL_1_UI->vtbl->func_DF4(BIT_Scarab_Count_Krystal)) {
            //Refusing scarabs
            gDLL_3_Animation->vtbl->func17(SeqIndex_Bribe_with_Scarabs_Refused, self, -1);
        } else if (gDLL_1_UI->vtbl->func_F24() != PAGE_Inventory) {
            //If the object inventory closes, go back to waiting
            objdata->state = STATE_Waiting_for_Bribe;
            func_80023BF8(self, 0, 0, 0, 0, 2);
        }
        break;
    case STATE_Waiting_to_Sell_Map:
        if (func_80032538(self)) {
            //Play map sale chat sequence when talked to
            gDLL_3_Animation->vtbl->func17(SeqIndex_Chat_Map, self, -1);
        } else if (gDLL_1_UI->vtbl->func_F24() == PAGE_Inventory) {
            //Advance state when object inventory open
            objdata->state = STATE_Map_Sale_Attempt;
            func_80023BF8(self, 0, 0, 0, 0, 3);
        }
        break;
    case STATE_Map_Sale_Attempt:
        if (gDLL_1_UI->vtbl->func_DF4(BIT_Scarab_Count_Krystal)) {
            //Give map when scarabs selected in inventory
            player = get_player();
            count = ((DLL_210_Player*)player->dll)->vtbl->get_scarabs(player);
            if (count >= 10) {
                ((DLL_210_Player*)player->dll)->vtbl->set_scarabs(player, count - 10);
                gDLL_3_Animation->vtbl->func17(SeqIndex_Selling_Map, self, -1);
                objdata->state = STATE_Finished;
                func_80023BF8(self, 0, 0, 0, 0, 2);
                main_set_bits(BIT_Map_CC, 1);
            }
        } else if (gDLL_1_UI->vtbl->func_F24() != PAGE_Inventory) {
            //If the object inventory closes, go back to waiting
            objdata->state = STATE_Waiting_to_Sell_Map;
            func_80023BF8(self, 0, 0, 0, 0, 2);
        }
        break;
    case STATE_Finished:
        //Play "restating" sequence when talked to
        if (func_80032538(self)) {
            gDLL_3_Animation->vtbl->func17(SeqIndex_Chat_End, self, -1);
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
