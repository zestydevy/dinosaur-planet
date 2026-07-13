#include "PR/os.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/joypad.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

// Note: Object.unkE0 determines whether the object is being held by the player.
//       This field is set by the player DLL partway through the pickup animation.

static s32 pickup_should_pickup(Object* obj, Object* player, Pickup* pickup);

// offset: 0x0 | ctor
void pickup_ctor(void *dll) { }

// offset: 0xC | dtor
void pickup_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void pickup_setup(Object* obj, Pickup* pickup, s16 arg2) {
    objAddObjectType(obj, OBJTYPE_Pickup);
    pickup->unk2 = arg2;
}

// offset: 0x64 | func: 1 | export: 1
// @bug: Missing 2nd param
#ifndef AVOID_UB
s32 pickup_control(Object* obj) {
#else
s32 pickup_control(Object* obj, UNUSED Pickup* _pickup) {
#endif
    Object* player;
    Pickup* pickup;
    Object* var_t2;
    s32 temp_v0;
    s32 i;
    Func_80057F1C_Struct** sp58;
    f32 var_fv0;

    pickup = obj->data;
    pickup->unk8 = 0;
    pickup->flags &= ~PICKUPFLAG_PickedUpThisTick;
    player = objGetPlayer();
    if (pickup->state == PICKUP_NotHeld) {
        // Not currently held
        pickup->state = pickup_should_pickup(obj, player, pickup);
        if (pickup->state != PICKUP_NotHeld) {
            pickup->flags |= PICKUPFLAG_PickedUpThisTick;
            pickup->unk6 = 1;
        }
        if (obj->unkE0 == 0) {
            func_800267C4(obj);
            obj->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            // Do gravity
            if (!(pickup->flags & PICKUPFLAG_NoGravity)) {
                obj->velocity.y -= 0.1f * gUpdateRateF;
                obj->srt.transl.y += obj->velocity.y * gUpdateRateF;
            }
            // Do track collision
            temp_v0 = func_80057F1C(obj, obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, &sp58, 0, 1);
            var_t2 = NULL;
            for (i = 0; i < temp_v0; i++) {
                if (sp58[i]->unk14 != 0xE) {
                    if ((obj->srt.transl.y < sp58[i]->unk0[0]) && ((sp58[i]->unk0[0] - 40.0f) < obj->srt.transl.y)) {
                        var_t2 = sp58[i]->unk10;
                        obj->srt.transl.y = sp58[i]->unk0[0];
                        obj->velocity.y = 0.0f;
                        break;
                    }
                }
            }
            for (i = 0; i < temp_v0; i++) {
                var_fv0 = obj->srt.transl.y - sp58[i]->unk0[0];
                if (var_fv0 < 0.0f) {
                    var_fv0 = -var_fv0;
                }
                if (var_fv0 < 5.0f) {
                    if (pickup->unk8 < sp58[i]->unk14) {
                        pickup->unk8 = sp58[i]->unk14;
                    }
                }
            }
            // Tell stuff like pressure plates that we're on top of them?
            if (var_t2 != NULL) {
                var_t2->polyhits->unk100[var_t2->polyhits->unk10F] = obj;
                var_t2->polyhits->unk10F += 1;
            }
        }
    } else {
        // Currently held
        func_8002681C(obj);
        obj->unkAF |= ARROW_FLAG_8_No_Targetting;
        if (joyGetPressed(0) & A_BUTTON) {
            joyDisableButtons(0, A_BUTTON);
            if ((pickup->flags & PICKUPFLAG_DropDisabled) || (((DLL_210_Player*)player->dll)->vtbl->func49(player) == 0)) {
                gDLL_6_AMSFX->vtbl->play(player, SOUND_912_Object_Refused, MAX_VOLUME, NULL, NULL, 0, NULL);
            } else {
                pickup->unk6 = 0;
            }
        }
        if (obj->unkE0 == 1) {
            // Player set field to indicate that we are now being held
            pickup->state = PICKUP_Held;
        }
        if ((pickup->state == PICKUP_Held) && (obj->unkE0 == 0)) {
            // Player signalled that we have been dropped
            pickup->state = PICKUP_NotHeld;
            pickup->unk6 = 0;
            if (!(pickup->flags & PICKUPFLAG_DontSave)) {
                map_save_object(obj->setup, obj->mapID, obj->srt.transl.x, obj->srt.transl.y + 10.0f, obj->srt.transl.z);
            }
        }
        if (pickup->unk6 != 0) {
            // Send message to player to pickup this object
            objSendMesg(player, 0x100008, obj, (void* ) ((pickup->unk2 << 0x10) | (pickup->unk0 & 0xFFFF)));
        }
    }
    return pickup->state;
}

// offset: 0x5DC | func: 2 | export: 2
s32 pickup_should_print(Object* obj, s32 visibility) {
    // Fade out shadow when in a seq
    if (obj->def->shadowType == OBJ_SHADOW_GEOM) {
        if (obj->seqSlot == SEQSLOT_NONE) {
            obj->shadow->flags &= ~OBJ_SHADOW_FLAG_FADE_OUT;
        } else {
            obj->shadow->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
        }
    }

    if (obj->unkE0) {
        if (visibility != -1) {
            return FALSE;
        }
    } else if (visibility == 0) {
        return FALSE;
    }
    return TRUE;
}

// offset: 0x664 | func: 3 | export: 3
void pickup_free(Object* obj) {
    objFreeObjectType(obj, OBJTYPE_Pickup);
}

// offset: 0x6A0 | func: 4 | export: 4
s32 pickup_get_state(Pickup* pickup) {
    return pickup->state;
}

// offset: 0x6AC | func: 5 | export: 5
s32 pickup_was_just_picked_up(Pickup* pickup) {
    return pickup->flags & PICKUPFLAG_PickedUpThisTick;
}

// offset: 0x6C0 | func: 6 | export: 6
s32 pickup_func_6C0(Pickup* pickup) {
    return pickup->unk8;
}

// offset: 0x6CC | func: 7 | export: 7
void pickup_set_gravity(Pickup* pickup, u8 enabled) {
    if (enabled) {
        pickup->flags &= ~PICKUPFLAG_NoGravity;
    } else {
        pickup->flags |= PICKUPFLAG_NoGravity;
    }
}

// offset: 0x704 | func: 8 | export: 8
void pickup_set_drop_disabled(Pickup* pickup, u8 disabled) {
    if (disabled) {
        pickup->flags |= PICKUPFLAG_DropDisabled;
    } else {
        pickup->flags &= ~PICKUPFLAG_DropDisabled;
    }
}

// offset: 0x73C | func: 9 | export: 9
s32 pickup_get_drop_disabled(Pickup* pickup) {
    if (pickup->flags & PICKUPFLAG_DropDisabled) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x760 | func: 10 | export: 10
void pickup_set_dont_save(Pickup *pickup, u8 dontSave) {
    if (dontSave) {
        pickup->flags |= PICKUPFLAG_DontSave;
    } else {
        pickup->flags &= ~PICKUPFLAG_DontSave;
    }
}

// offset: 0x798 | func: 11 | export: 11
void pickup_drop(Object* obj, Pickup* pickup) {
    Object* player;
    Object* heldObj;

    player = objGetPlayer();
    pickup->state = PICKUP_NotHeld;

    // Get held object
    ((DLL_210_Player*)player->dll)->vtbl->func10(player, &heldObj);
    // If we are the held object, drop
    if (heldObj == obj) {
        ((DLL_210_Player*)player->dll)->vtbl->func11(player, NULL);
    }
}

// offset: 0x838 | func: 12
static s32 pickup_should_pickup(Object* obj, Object* player, Pickup* pickup) {
    s32 shouldPickup = FALSE;

    if (((obj->unk78[obj->unkD4].flags & 0xF) == 6) 
            && (obj->unkAF & ARROW_FLAG_1_Interacted) 
            && (obj->unkE0 == 0)) {
        pickup->unk0 = 0;
        joyDisableButtons(0, A_BUTTON);
        shouldPickup = TRUE;
    }

    return shouldPickup;
}
