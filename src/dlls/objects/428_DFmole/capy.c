#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/common/foodbag.h"
#include "dlls/objects/210_player.h"
#include "functions.h"
#include "dll.h"

// size:0x14
typedef struct {
/*00*/ u8 fed;
/*01*/ u8 ateMagicPlant;
/*02*/ u8 unk2;
/*04*/ u16 timer;
/*08*/ u32 soundHandle;
/*0C*/ f32 unkC;
/*10*/ f32 unk10;
} capy_data;

enum CapyAnimStates {
    CAPY_ASTATE_0_Standing = 0,
    CAPY_ASTATE_1_Burrowed = 1,
    CAPY_ASTATE_2_Unburrow = 2,
    CAPY_ASTATE_3_Burrow = 3,
    CAPY_ASTATE_4_Sniff = 4,
    CAPY_ASTATE_5_Eat = 5,
    CAPY_ASTATE_6_DoneEating = 6,
    CAPY_ASTATE_7_DigWall = 7,
    CAPY_ASTATE_8_Walking = 8
};

enum CapyLogicStates {
    CAPY_LSTATE_0_Init = 0,
    CAPY_LSTATE_1_DigWall = 1,
    CAPY_LSTATE_2_GoToDigSpot = 2,
    CAPY_LSTATE_3_Eating = 3,
    CAPY_LSTATE_4_Idle = 4,
    CAPY_LSTATE_5_Underground = 5
};

enum CapyModAnimIndices {
    CAPY_MODANIM_0_Burrow = 0,
    CAPY_MODANIM_1_Unburrow = 1,
    CAPY_MODANIM_2_DigWall = 2,
    CAPY_MODANIM_3_Standing = 3,
    CAPY_MODANIM_4_Walking = 4,
    CAPY_MODANIM_5_Sniffing = 5,
    CAPY_MODANIM_6_Eating = 6,
    CAPY_MODANIM_7_DoneEating = 7
};

/*0x0*/ static s32 _data_0[1] = {2};
/*0x4*/ static u16 sTunnelGamebits[] = { // gamebits
    BIT_CapyTunnel1, BIT_CapyTunnel2, BIT_CapyTunnel3, 0x0000, 0x0000, 0x0000
};

/*0x0*/ static  ObjFSA_StateCallback sAnimCallbacks[9];
/*0x28*/ static ObjFSA_StateCallback sLogicCallbacks[6];

static void capy_func_468(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void capy_func_644(Object *self, s32 arg1, Baddie *baddie, ObjFSA_Data *fsa);
static void capy_func_704(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static int capy_anim_callback(Object *self, Object *arg1, AnimObj_Data *arg2, s8 arg3);

static s32 capy_anim_state_0_standing(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_1_burrowed(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_2_unburrow(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_3_burrow(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_4_sniff(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_5_eat(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_6_done_eating(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_7_dig_wall(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_anim_state_8_walking(Object *self, ObjFSA_Data *fsa, f32 updateRate);

static s32 capy_logic_state_0_init(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_logic_state_1_dig_wall(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_logic_state_2_go_to_dig_spot(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_logic_state_3_eating(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_logic_state_4_idle(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 capy_logic_state_5_underground(Object *self, ObjFSA_Data *fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void capy_setup_fsa_callbacks(void) {
    sAnimCallbacks[0] = capy_anim_state_0_standing;
    sAnimCallbacks[1] = capy_anim_state_1_burrowed;
    sAnimCallbacks[2] = capy_anim_state_2_unburrow;
    sAnimCallbacks[3] = capy_anim_state_3_burrow;
    sAnimCallbacks[4] = capy_anim_state_4_sniff;
    sAnimCallbacks[5] = capy_anim_state_5_eat;
    sAnimCallbacks[6] = capy_anim_state_6_done_eating;
    sAnimCallbacks[7] = capy_anim_state_7_dig_wall;
    sAnimCallbacks[8] = capy_anim_state_8_walking;
    
    sLogicCallbacks[0] = capy_logic_state_0_init;
    sLogicCallbacks[1] = capy_logic_state_1_dig_wall;
    sLogicCallbacks[2] = capy_logic_state_2_go_to_dig_spot;
    sLogicCallbacks[3] = capy_logic_state_3_eating;
    sLogicCallbacks[4] = capy_logic_state_4_idle;
    sLogicCallbacks[5] = capy_logic_state_5_underground;
}

// offset: 0xD8 | ctor
void capy_ctor(void *dll) {
    capy_setup_fsa_callbacks();
}

// offset: 0x118 | dtor
void capy_dtor(void *dll) { }

// offset: 0x124 | func: 1 | export: 0
void capy_setup(Object *self, Baddie_Setup *setup, s32 arg2) {
    Baddie* baddie = self->data;
    u8 var_v0;
    
    var_v0 = 0x16;
    
    if (arg2 != 0) {
        var_v0 = 0x17;
    }
    if (!(setup->unk2B & 1)) {
        var_v0 |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 9, 6, 0x102, var_v0, 20.0f);
    self->animCallback = capy_anim_callback;
    baddie->fsa.animState = CAPY_ASTATE_1_Burrowed;
    baddie->fsa.logicState = CAPY_LSTATE_0_Init;
    baddie->fsa.unk33A = 0;
}

// offset: 0x1DC | func: 2 | export: 1
void capy_control(Object *self) {
    Baddie *baddie = self->data;
    
    gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, sAnimCallbacks, sLogicCallbacks);
    capy_func_468(self, baddie, &baddie->fsa);
    if ((baddie->fsa.target != NULL) || (baddie->fsa.hitpoints == 0)) { 
        capy_func_644(self, 0, baddie, &baddie->fsa);
        return;
    }
    capy_func_704(self, baddie, &baddie->fsa);
}

// offset: 0x2D4 | func: 3 | export: 2
void capy_update(Object *self) {
    gDLL_18_objfsa->vtbl->func2(self, self->data, sAnimCallbacks);
}

// offset: 0x324 | func: 4 | export: 3
void capy_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x384 | func: 5 | export: 4
void capy_free(Object *self, s32 a1) {
    Baddie *baddie = self->data;

    obj_free_object_type(self, 4);
    if (self->linkedObject != NULL) {
        obj_destroy_object(self->linkedObject);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 1);
}

// offset: 0x428 | func: 6 | export: 5
u32 capy_get_model_flags(Object *self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x438 | func: 7 | export: 6
u32 capy_get_data_size(Object *self, u32 a1) {
    return sizeof(Baddie) + sizeof(capy_data);
}

// offset: 0x44C | func: 8
static int capy_anim_callback(Object *self, Object *arg1, AnimObj_Data *arg2, s8 arg3) {
    return 0; 
}

// offset: 0x468 | func: 9
static void capy_func_468(Object *self, Baddie *baddie, ObjFSA_Data *fsa) {
    Vec3f vec;
    Object* player;
    Object* dinoEgg;
    Object* foodbag; 

    dinoEgg = NULL;

    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }
    if (fsa->target != NULL) {
        vec.f[0] = fsa->target->positionMirror.f[0] - self->positionMirror.f[0];
        vec.f[1] = fsa->target->positionMirror.f[1] - self->positionMirror.f[1];
        vec.f[2] = fsa->target->positionMirror.f[2] - self->positionMirror.f[2];
        fsa->targetDist = sqrtf(SQ(vec.f[0]) + SQ(vec.f[1]) + SQ(vec.f[2]));
    }
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);
    player = get_player();
    if ((player == fsa->target) || ((fsa->target != NULL) && (fsa->target->id == OBJ_foodbagNewMeat))) {
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        if (foodbag != NULL) {
            dinoEgg = ((DLL_IFoodbag*)foodbag->dll)->vtbl->get_nearest_placed_food_of_type(foodbag, self, FOOD_Dino_Egg);
        }
        if ((dinoEgg != NULL) && (vec3_distance(&self->positionMirror, &dinoEgg->positionMirror) < 150.0f)) {
            fsa->target = dinoEgg;
            return;
        }
        fsa->target = NULL;
    }
}

// offset: 0x644 | func: 10
static void capy_func_644(Object *self, s32 arg1, Baddie *baddie, ObjFSA_Data *fsa) {
    Object* player;

    player = get_player();
    self->objhitInfo->unk58 |= 1;
    if (player == fsa->target) {
        if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2, 1) != 0) {
            fsa->target = NULL;
        }
    }
}

// offset: 0x704 | func: 11
static void capy_func_704(Object *self, Baddie *baddie, ObjFSA_Data *fsa) {
    Object* temp_v0_2;

    self->objhitInfo->unk58 &= ~0x1;
    
    temp_v0_2 = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
    if (temp_v0_2 != NULL) {
        fsa->target = temp_v0_2;
        fsa->unk33D = 0;
    }
}

// offset: 0x7A0 | func: 12
s32 capy_anim_state_0_standing(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 count;
    Object** objlist;
    s32 i;
    s32 distflag;
    Baddie* baddie;
    capy_data* capydata;

    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_3_Standing, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.03f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 5);
    if (fsa->enteredAnimState != 0) {
        capydata->timer = 0;
    }
    capydata->timer += 1;
    if ((get_player() == fsa->target) && (capydata->timer >= 61)) {
        if (capydata->ateMagicPlant != 0) {
            fsa->enteredAnimState = TRUE;
            fsa->unk33A = 0;
            fsa->logicState = CAPY_LSTATE_5_Underground;
            return CAPY_ASTATE_3_Burrow + 1;
        }
        objlist = obj_get_all_of_type(OBJTYPE_54, &count);
        distflag = 0;
        for (i = 0; i < count; i++) {
            if (!distflag && vec3_distance(&self->positionMirror, &objlist[i]->positionMirror) < 300.0f) {
                distflag = 1;
                fsa->target = objlist[i];
            }
        }
    }
    return 0;
}

// offset: 0x97C | func: 13
s32 capy_anim_state_1_burrowed(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie *objdata = self->data;
    capy_data* capydata;

    capydata = objdata->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_1_Unburrow, 0.0f, 0);
        fsa->unk33A = 0;
    }
    if (capydata->timer != 0) {
        capydata->timer -= 1;
    }
    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    return 0;
}

// offset: 0xA0C | func: 14
s32 capy_anim_state_2_unburrow(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_1_Unburrow, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->animTickDelta = 0.04f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (fsa->unk33A != 0) {
        fsa->logicState = CAPY_LSTATE_4_Idle;
    
    }
    return 0;
}


// offset: 0xA9C | func: 15
s32 capy_anim_state_3_burrow(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* capydata;
    CurveSetup* temp_v0_2;
    s32 temp_v0;
    s32 sp34;

    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_0_Burrow, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.04f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (fsa->unk33A != 0) {
        sp34 = 0x25;
        temp_v0 = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &sp34, 1, 0);
        if (temp_v0 != -1) {
            temp_v0_2 = gDLL_26_Curves->vtbl->func_39C(temp_v0);
            self->srt.transl.f[0] = temp_v0_2->pos.f[0];
            self->srt.transl.f[2] = temp_v0_2->pos.f[2];
        }
        capydata->timer = 80;
        return CAPY_ASTATE_1_Burrowed + 1;
    }
    return 0;
}

// offset: 0xBC4 | func: 16
s32 capy_anim_state_4_sniff(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* capydata;

    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_5_Sniffing, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.04f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (fsa->unk308 & 0x1) {
        fsa->unk308 &= ~0x1;
        capydata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_77C_Capy_Sniff, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(capydata->soundHandle, ((f32) rand_next(-0xA, 0xA) / 100.0f) + 1.0f);
    }
    if (fsa->unk33A != 0) {
        return CAPY_ASTATE_5_Eat + 1;
    } else {
        return 0;
    }
}

// offset: 0xD1C | func: 17
s32 capy_anim_state_5_eat(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* target;
    Object* player;
    Object* foodbag;
    Baddie* baddie;
    capy_data* capydata;
    
    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_6_Eating, 0.0f, 0);
        fsa->unk33A = 0;
    }
    target = fsa->target;
    fsa->animTickDelta = 0.1f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (target != NULL) {
        if (fsa->enteredAnimState != 0) {
            func_8002635C(target, NULL, 7, 1, 0);
        }
        target->srt.scale *= 0.97f;
    }

    if (fsa->unk308 & 0x1) {
        fsa->unk308 &= ~0x1;
        capydata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_77D_Capy_Eat, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(capydata->soundHandle, ((f32) rand_next(-0xA, 0xA) / 100.0f) + 1.0f);
    }
    if (fsa->unk33A != 0) {
        if (target != NULL) {
            if (target->srt.scale > 0.01f) {
                return 0;
            }
            if (target->id == OBJ_MagicPlant) {
                capydata->ateMagicPlant = TRUE;
                obj_destroy_object(target);
            } else {
                capydata->fed = TRUE;
                player = get_player();
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
                if (foodbag != NULL) {
                    ((DLL_IFoodbag*)foodbag->dll)->vtbl->destroy_placed_food(foodbag, target);
                }
            }
            fsa->target = NULL;
            
        }
        return CAPY_ASTATE_6_DoneEating + 1;
    }
    return 0;
}

// offset: 0xF8C | func: 18
s32 capy_anim_state_6_done_eating(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_7_DoneEating, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.08f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (fsa->unk33A != 0) {
        if ((gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 150.0f, _data_0, -1) == 0) 
                && (main_get_bits(sTunnelGamebits[baddie->unk3F8->unk9C->unk18]) == 0)) {
            fsa->logicState = CAPY_LSTATE_2_GoToDigSpot;
            return CAPY_ASTATE_8_Walking + 1;
        }
        fsa->logicState = CAPY_LSTATE_4_Idle;
        return CAPY_ASTATE_0_Standing + 1;
    }
    return 0;
}

// offset: 0x10BC | func: 19
s32 capy_anim_state_7_dig_wall(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* capydata;

    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_2_DigWall, 0.0f, 0);
        fsa->unk33A = 0;
    }
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (fsa->unk308 & 0x1) {
        fsa->unk308 &= ~0x1;
        capydata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_77D_Capy_Eat, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(capydata->soundHandle, ((f32) rand_next(-0xA, 0xA) / 100.0f) + 1.0f);
    }
    if (fsa->unk308 & 0x200) {
        main_set_bits(sTunnelGamebits[baddie->unk3F8->unk9C->unk18], 1);
    }
    if (fsa->unk33A != 0) {
        fsa->logicState = CAPY_LSTATE_4_Idle;
        return CAPY_ASTATE_0_Standing + 1;
    }
    return 0;
}

// offset: 0x1264 | func: 20
s32 capy_anim_state_8_walking(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    capy_data* capydata;
    f32 temp_fa0;
    f32 temp_fv1;
    
    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, CAPY_MODANIM_4_Walking, 0.0f, 0);
        fsa->unk33A = 0;
    }
    if (fsa->enteredAnimState != 0) {
        capydata->timer = 0;
        capydata->unkC = self->srt.transl.x;
        capydata->unk10 = self->srt.transl.z;
    }
    temp_fv1 = self->srt.transl.x - capydata->unkC;
    temp_fa0 = self->srt.transl.z - capydata->unk10;
    capydata->unkC = self->srt.transl.x;
    capydata->unk10 = self->srt.transl.z;
    if (((SQ(temp_fv1) + SQ(temp_fa0)) < 0.1f) && (fsa->logicState != CAPY_LSTATE_2_GoToDigSpot)) {
        capydata->timer += 1;
    } else {
        capydata->timer = 0;
    }
    if (capydata->timer >= 13) {
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = 0;
        fsa->logicState = CAPY_LSTATE_5_Underground;
        return CAPY_ASTATE_3_Burrow + 1;
    }
    fsa->animTickDelta = 0.08f;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_33_BaddieControl->vtbl->func3(self, fsa, baddie, 2.0f, 12.0f);
    return 0;
}

// offset: 0x1408 | func: 21
s32 capy_logic_state_0_init(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return CAPY_LSTATE_5_Underground + 1;
}

// offset: 0x1420 | func: 22
s32 capy_logic_state_1_dig_wall(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x1438 | func: 23
s32 capy_logic_state_2_go_to_dig_spot(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s32 _pad[2];
    capy_data* capydata;
    UnkCurvesStruct* baddieCurves;
    Baddie* baddie;
    s32 temp2;
    f32 var_fv0;
    f32 temp;
    s32 temp4;

    baddie = self->data;
    baddieCurves = baddie->unk3F8;
    capydata = baddie->objdata;
    temp2 = func_800053B0(baddieCurves, 5.0f / (SQ(baddieCurves->unk68.f[2] - self->srt.transl.f[2]) + SQ(baddieCurves->unk68.f[0] - self->srt.transl.f[0])));
    if (((temp2 != 0) || (baddieCurves->unk10 != 0)) && (gDLL_26_Curves->vtbl->func_4704(baddieCurves) != 0)) {
        fsa->animState = CAPY_ASTATE_7_DigWall;
        fsa->enteredAnimState = FALSE;
        fsa->unk33A = 0;
        fsa->logicState = CAPY_LSTATE_1_DigWall;
        if (self){} // @fake
        return 0;
    } else {
        temp4 = (arctan2_f(baddieCurves->unk74, baddieCurves->unk7C) & 0xFFFF);
        var_fv0 = (f32) (((temp4 - ((u16)self->srt.yaw & 0xFFFF))) + 0x8000);
        if (var_fv0 > 32768.0f) {
            var_fv0 = -65535.0f + var_fv0;
        }
        if (var_fv0 < -32768.0f) {
            var_fv0 = 65535.0f + var_fv0;
        }
        if (var_fv0 < 0.0f) {
            temp = -var_fv0;
        } else {
            temp = var_fv0;
        }
        fsa->unk278 = 1.0f - (temp / 32767.0f);
        if (fsa->unk278 < 0.01f) {
            fsa->unk278 = 0.01f;
        }
        fsa->unk278 *= (f32) capydata->unk2 / 100.0f;
        fsa->speed = fsa->unk278;
        gDLL_18_objfsa->vtbl->func6(self, fsa, baddieCurves->unk68.f[0], baddieCurves->unk68.f[2], 0, 0, 60.0f);
        fsa->animState = CAPY_ASTATE_8_Walking;
        return 0;
    }
}

// offset: 0x1698 | func: 24
s32 capy_logic_state_3_eating(Object *self, ObjFSA_Data* fsa, f32 updateRate) {
    if ((fsa->animState != CAPY_ASTATE_6_DoneEating) && ((fsa->target == 0) || (get_player() == fsa->target))) {
        fsa->animState = CAPY_ASTATE_3_Burrow;
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = 0;
        return CAPY_LSTATE_5_Underground + 1;
    }
    return 0;
}

// offset: 0x1728 | func: 25
s32 capy_logic_state_4_idle(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    Unk80009024* temp_s1;
    capy_data* capydata;
    u8 isTargetPlayer;

    baddie = self->data;
    capydata = baddie->objdata;
    if (fsa->target != NULL) {
        isTargetPlayer = get_player() == fsa->target;
        fsa->animState = CAPY_ASTATE_8_Walking;
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
        temp_s1 = &baddie->unk34C;
        bcopy(&self->srt.transl, &temp_s1->unk0, sizeof(Vec3f));
        bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
        func_80009024(temp_s1, &baddie->unk374);
        if (isTargetPlayer != 0) {
            if ((fsa->targetDist < 50.0f) || !capydata->fed) {
                fsa->animState = CAPY_ASTATE_0_Standing;
            }
        } else { 
            if ((fsa->targetDist < 20.0f) || ((capydata->timer >= 11) && (fsa->targetDist < 30.0f))) {
                fsa->animState = CAPY_ASTATE_4_Sniff;
                fsa->target->unkAF |= 8;
                return CAPY_LSTATE_3_Eating + 1;
            }
        }
        if (temp_s1->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, temp_s1->unk18.x, temp_s1->unk18.z, 0.0f, 0.0f, 60.0f);
        } else if (isTargetPlayer != 0) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, temp_s1->unk18.x, temp_s1->unk18.z, 20.0f, 30.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(self, fsa, temp_s1->unk18.x, temp_s1->unk18.z, 10.0f, 10.0f, 60.0f);
        }
    } else {
        fsa->animState = CAPY_ASTATE_3_Burrow;
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = 0;
        return CAPY_LSTATE_5_Underground + 1;
    }
    return 0;
}

// offset: 0x19DC | func: 26
s32 capy_logic_state_5_underground(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    capy_data* objdata = baddie->objdata;
    
    if ((fsa->target != NULL) && (fsa->animState == CAPY_ASTATE_1_Burrowed) && (objdata->timer == 0)) {
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, 3.0f, 1);
        fsa->animState = CAPY_ASTATE_2_Unburrow;
        fsa->enteredAnimState = TRUE;
        fsa->unk33A = 0;
    }
    return 0;
}
