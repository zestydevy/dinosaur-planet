#include "common.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/common/foodbag.h"
#include "dlls/objects/common/foodbag.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/math.h"
#include "sys/objanim.h"

typedef struct {
/*00*/    ObjSetup base;
/*18*/    u8 scale;
/*19*/    u8 maxSpeed;
/*1A*/    u8 ascendSpeed;
/*1B*/    u8 diveSpeed; 
/*1C*/    u16 swimOnBedDuration;
/*1E*/    u16 swimOnSurfaceDuration;
/*20*/    u16 respawnDuration;
/*22*/    u8 curveOffsetY;
/*23*/    u8 surfaceOffsetY; 
} curveFish_Setup;

typedef struct {
/*000*/    f32 surfaceY;
/*004*/    UnkCurvesStruct curves;
/*10C*/    u8 state;
/*10D*/    u8 initialised;
/*10E*/    s16 objHitsConfig;
/*110*/    f32 maxSpeed;
/*114*/    f32 ascendSpeed;
/*118*/    f32 diveSpeed;
/*11C*/    f32 speed;
/*120*/    f32 animSpeed;
/*124*/    f32 timer;
} curveFish_Data;

typedef enum {
    CurveFish_STATE_0_Hidden,
    CurveFish_STATE_1_Fade_In,
    CurveFish_STATE_2_Swim_on_Seabed,
    CurveFish_STATE_3_Swim_Ascending,
    CurveFish_STATE_4_Swim_on_Surface,
    CurveFish_STATE_5_Swim_Diving
} curveFish_States;

typedef enum {
    CurveFish_MODANIM_0_Swim_LOOP,
    CurveFish_MODANIM_1_Jump,
    CurveFish_MODANIM_2_Tilted_Up,
    CurveFish_MODANIM_3_Tilted_Down
} curveFish_ModAnim;

#define FADE_IN_DURATION 60.0f

/*0x0*/ static s32 dCurveType = 35;

static void curveFish_caught_by_net(Object* self);
static void curveFish_initialise(Object* self);

// offset: 0x0 | ctor
void curveFish_ctor(void *dll) { }

// offset: 0xC | dtor
void curveFish_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void curveFish_setup(Object* self, ObjSetup* setup, s32 reset) {
    curveFish_Data* objData = self->data;
    
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
    objData->initialised = FALSE;
    curveFish_initialise(self);
}

// offset: 0x6C | func: 1
static void curveFish_initialise(Object* self) {
    curveFish_Data* objData;
    curveFish_Setup* objSetup;
    s32 curveType;

    objData = self->data;
    objSetup = (curveFish_Setup*)self->setup;
    curveType = dCurveType;

    if (func_80058B1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &objData->surfaceY, 0)) {
        objData->surfaceY = (self->srt.transl.y + objData->surfaceY) - objSetup->surfaceOffsetY;
        gDLL_26_Curves->vtbl->func_4288((&objData->curves), self, 100.0f, &curveType, -1);
        objData->state = CurveFish_STATE_1_Fade_In;
        objData->initialised = TRUE;
        objData->objHitsConfig = self->objhitInfo->unk58;
        objData->maxSpeed = objSetup->maxSpeed / 100.0f;
        objData->ascendSpeed = objSetup->ascendSpeed / 1000.0f;
        objData->diveSpeed = objSetup->diveSpeed / 1000.0f;
        objData->speed = 0.0f;
 
        self->srt.scale = self->def->scale * (objSetup->scale / 100.0f);
        func_80026940(self, (self->def->hitbox_flagsB6 * objSetup->scale) / 100);
    }
}

// offset: 0x280 | func: 2 | export: 1
void curveFish_control(Object* self) {
    curveFish_Data* objData;
    curveFish_Setup* objSetup;
    Object* player;
    s32 damageType;
    f32 animSpeedFactor;

    objData = self->data;
    objSetup = (curveFish_Setup*)self->setup;
    player = objGetPlayer();

    if (objData->initialised == FALSE) {
        curveFish_initialise(self);
    } 
    if (objData->initialised == FALSE) {
        return;
    }

    //Allow targetting based on player-specific gamebits (whether Krystal/Sabre own a fishing net)
    if (player->id == OBJ_Krystal) {
        if (mainGetBits(BIT_Krystal_Fishing_Net) == FALSE) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    } else {
        if (mainGetBits(BIT_Sabre_Fishing_Net) == FALSE) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    }

    //Handle fishing net collision
    damageType = func_80025F40(self, NULL, NULL, NULL);
    if ((damageType != 0) && (damageType == Damage_Type_Fishing_Net)) {
        curveFish_caught_by_net(self);
        return;
    }

    objData->timer += gUpdateRateF;

    //Handle speed and animation speed
    {
        objData->speed += ((objData->maxSpeed * (s8)mathRnd(-50, 100)) / 1000.0f) * gUpdateRateF;
        if (objData->speed < 0) {
            objData->speed = 0.0f;
        } else if (objData->speed > objData->maxSpeed) {
            objData->speed = objData->maxSpeed;
        }

        if (objData->speed > (objData->maxSpeed * 0.5f)) {
            animSpeedFactor = objData->maxSpeed - objData->speed;
        } else {
            animSpeedFactor = objData->speed;
        }
        objData->animSpeed = (animSpeedFactor * 0.06f) / (objData->maxSpeed * 0.5f);
    }

    //Handle curve interpolation
    {
        curves_func_800053B0(&objData->curves.unk0, objData->speed);

        // Hide the fish when it reaches the end of its path
        /* (@bug?: doesn't check if it's already in the hidden state and doesn't 
            reset curve position, so it can't respawn through `objSetup->respawnDuration`) */
        if ((objData->curves.unk0.unk10) && (gDLL_26_Curves->vtbl->func_4704(&objData->curves))) {
            STUBBED_PRINTF("fish has reached the end of it life\n");
            objData->state = CurveFish_STATE_0_Hidden;
            objData->timer = 0.0f;
            self->objhitInfo->unk58 = 0x100;
            self->opacity = 0;
        }

        //Set lateral position via point on curve
        self->srt.transl.x = objData->curves.unk0.unk68.x;
        self->srt.transl.z = objData->curves.unk0.unk68.z;

        //Set yaw via tangent vector at curve point
        self->srt.yaw = mathAtan2f(objData->curves.unk0.unk74, objData->curves.unk0.unk7C) + M_90_DEGREES;
    }

    switch (objData->state) {
    case CurveFish_STATE_0_Hidden:
        self->unkAF |= ARROW_FLAG_8_No_Targetting;

        if (objData->timer >= objSetup->respawnDuration) {
            objData->state = CurveFish_STATE_1_Fade_In;
            objData->timer -= objSetup->respawnDuration;
        }
        break;
    case CurveFish_STATE_1_Fade_In:
        if (objData->timer <= FADE_IN_DURATION) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            self->opacity = (s32) ((objData->timer / FADE_IN_DURATION) * OBJECT_OPACITY_MAX);
        } else {
            self->objhitInfo->unk58 = objData->objHitsConfig;
            self->opacity = OBJECT_OPACITY_MAX;
            objData->state = CurveFish_STATE_2_Swim_on_Seabed;
            objData->timer -= FADE_IN_DURATION;
        }
        break;
    case CurveFish_STATE_2_Swim_on_Seabed:
        self->srt.transl.y = objData->curves.unk0.unk68.y + objSetup->curveOffsetY;
        if (objData->timer > objSetup->swimOnBedDuration) {
            objData->state = CurveFish_STATE_3_Swim_Ascending;
            objAnimSet(self, CurveFish_MODANIM_2_Tilted_Up, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    case CurveFish_STATE_3_Swim_Ascending:
        self->srt.transl.y += objData->ascendSpeed * gUpdateRateF;
        if (self->srt.transl.y > objData->surfaceY) {
            self->srt.transl.y = objData->surfaceY;
            objData->state = CurveFish_STATE_4_Swim_on_Surface;
            objData->timer = 0.0f;
            objAnimSet(self, CurveFish_MODANIM_0_Swim_LOOP, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    case CurveFish_STATE_4_Swim_on_Surface:
        if (objData->timer > objSetup->swimOnSurfaceDuration) {
            objData->state = CurveFish_STATE_5_Swim_Diving;
            objAnimSet(self, CurveFish_MODANIM_3_Tilted_Down, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    case CurveFish_STATE_5_Swim_Diving:
        self->srt.transl.y -= objData->diveSpeed * gUpdateRateF;
        if (self->srt.transl.y < objData->curves.unk0.unk68.y + objSetup->curveOffsetY) {
            self->srt.transl.y = objData->curves.unk0.unk68.y + objSetup->curveOffsetY;
            objData->state = CurveFish_STATE_2_Swim_on_Seabed;
            objData->timer = 0.0f;
            objAnimSet(self, CurveFish_MODANIM_0_Swim_LOOP, 0.0f, 0);
            objAnim_func_80024D74(self, 30);
        }
        break;
    }
    
    objAnimAdvance(self, objData->animSpeed, gUpdateRateF, NULL);
}


// offset: 0x8C8 | func: 3 | export: 2
void curveFish_update(Object *self) { }

// offset: 0x8D4 | func: 4 | export: 3
void curveFish_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    curveFish_Data* curvefishdata = self->data;
    
    if ((curvefishdata->state != CurveFish_STATE_0_Hidden) && visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}
// offset: 0x934 | func: 5 | export: 4
void curveFish_free(Object *self, s32 onlySelf) { }

// offset: 0x944 | func: 6 | export: 5
u32 curveFish_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x954 | func: 7 | export: 6
u32 curveFish_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(curveFish_Data);
}

// offset: 0x968 | func: 8
static void curveFish_caught_by_net(Object* self) {
    curveFish_Data* objData;
    Object* player;
    Object* foodbag;

    objData = self->data;

    if (self->id == OBJ_DF_Lantern) {
        /* OBJ_SC_golden_nugge is indexed just before OBJ_DF_Lantern: maybe a nugget was 
           once going to be fished out of the water, and it would've shared this DLL
           (or DLL 318, depending on which came first)? */
        mainSetBits(BIT_Gold_Nugget_LFV, 1);
    } else {
        player = objGetPlayer();
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Fish);
    }

    objData->timer = 0.0f;
    objData->state = CurveFish_STATE_0_Hidden;
    self->objhitInfo->unk58 = 0x100;
    self->opacity = 0;
}
