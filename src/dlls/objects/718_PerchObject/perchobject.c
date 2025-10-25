#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/26_curves.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "functions.h"
#include "sys/objects.h"
#include "sys/math.h"
#include "sys/main.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"
#include "game/gamebits.h"
#include "types.h"

// typedef struct {
// /*00*/	s16 objId;
// /*02*/	u8 quarterSize;
// /*03*/	u8 act;
// /*04*/	u8 nextCurveGroup;
// /*05*/	u8 prevCurveGroup;
// /*06*/	u8 branch1CurveGroup;
// /*07*/	u8 branch2CurveGroup;
// /*08*/	f32 x;
// /*0c*/	f32 y;
// /*10*/	f32 z;
// /*14*/	s32 uID;
// } CurveSetup; //curves use the base objSetup fields differently, so creating a unique base struct

typedef struct {
/*00*/ CurveSetup base;
/*36*/ s16 unk36;
/*38*/ s16 unk38; //gameBit?
} KyteCurve_Setup;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 interactionDistance;
/*19*/ u8 unk19;
/*1A*/ u16 kyteFlightGroup; //curve group Kyte should traverse when using "Find" command
/*1C*/ s16 unk1C;
/*1E*/ u8 useDistance3D; //affects how player-to-perch distance is calculated (2D X/Z vs. full 3D)
} PerchObject_Setup;

typedef struct {
    s8 stateIndex;
    CurveSetup* curveSetup;
} PerchObject_Data;

static s32 perchObject_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

enum PerchObjectStates {
    STATE_0_Initialise = 0,
    STATE_1_Wait_for_Player_to_Instruct_Kyte = 1
};

// offset: 0x0 | ctor
void perchObject_ctor(void *dll) { }

// offset: 0xC | dtor
void perchObject_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void perchObject_setup(Object* self, s32 arg1, s32 arg2) {
    PerchObject_Data* objData = self->data;

    objData->stateIndex = STATE_0_Initialise;
    obj_add_object_type(self, 0x30);
    self->unkBC = (void*)&perchObject_anim_callback;
}

// offset: 0x74 | func: 1 | export: 1
void perchObject_control(Object* self) {
    PerchObject_Data *objData;
    PerchObject_Setup *objSetup;
    Object *kyte;
    Object *player;
    CurveSetup *curveSetup;
    s32 playerIsNearby;
    u8 stateIndex;
    f32 playerToCurveDistance;

    objData = self->data;
    objSetup = (PerchObject_Setup *) self->setup;

    stateIndex = objData->stateIndex;
    switch (stateIndex){
        case STATE_0_Initialise:
            //Get the curveCreateInfo for Kyte's flight group
            curveSetup = gDLL_25->vtbl->dll_25_func_2A50(self, objSetup->kyteFlightGroup);
            objData->curveSetup = curveSetup;
            if (curveSetup){
                objData->stateIndex = STATE_1_Wait_for_Player_to_Instruct_Kyte;
                return;
            }
            return;        
        case STATE_1_Wait_for_Player_to_Instruct_Kyte:
            kyte = get_sidekick();
            if (!kyte){
                break;
            }

            player = get_player();

            //Check distance between player and perch (either lateral X/Z distance or full 3D distance check)
            if (objSetup->useDistance3D){
                playerToCurveDistance = vec3_distance_squared(&player->positionMirror, (Vec3f *) (&objData->curveSetup->base.x));
                playerIsNearby = (u8)(playerToCurveDistance <= (objSetup->interactionDistance * objSetup->interactionDistance));
            } else {
                playerToCurveDistance = vec3_distance_xz_squared(&player->positionMirror, (Vec3f *) (&objData->curveSetup->base.x));
                playerIsNearby = FALSE;
                if (playerToCurveDistance <= (objSetup->interactionDistance * objSetup->interactionDistance)) {
                    playerIsNearby = TRUE;
                }
                if (1) { } //fake?
                playerIsNearby &= 0xFF;
            }

            //Set Kyte's flight curve when player calls her
            if (playerIsNearby){
                ((DLL_Unknown *) kyte->dll)->vtbl->func[14].withTwoArgs((s32) kyte, 1);
                if (gDLL_1_UI->vtbl->func7(1)){
                    main_set_bits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightGroup);
                }
            }
            break;

    }
}

// offset: 0x25C | func: 2 | export: 2
void perchObject_update(Object *self) { }

// offset: 0x268 | func: 3 | export: 3
void perchObject_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x2BC | func: 4 | export: 4
void perchObject_free(Object* self, s32 arg1) {
    obj_free_object_type(self, 0x30);
}

// offset: 0x2FC | func: 5 | export: 5
u32 perchObject_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x30C | func: 6 | export: 6
u32 perchObject_get_data_size(Object *self, u32 a1) {
    return sizeof(PerchObject_Data);
}

// offset: 0x320 | func: 7 | export: 7
// Called when Kyte finishes landing on unpressed switch (just before switch gets pressed down)
s32 perchObject_land_on_perch(Object* self, s32 arg1) {
    PerchObject_Data* objData;
    s32 landedOnPerch;

    objData = self->data;

    landedOnPerch = FALSE;
    if (arg1 == 5) {
        if (objData->curveSetup->type22.usedBit != -1) {
            main_set_bits(objData->curveSetup->type22.usedBit, 1);
        }
        landedOnPerch = TRUE;
    }
    return landedOnPerch;
}

// offset: 0x388 | func: 8 | export: 8
u32 perchObject_func_388(Object *self, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x3A0 | func: 9 | export: 9
// Called when Kyte begins landing (entering slower flapping state while close to perch)
u32 perchObject_approach_perch(Object* self, s32 arg1, f32* deltaY) {
    Object *kyte;
    s16 flag;
    PerchObject_Data *state;

    kyte = get_sidekick();
    state = (PerchObject_Data*)self->data;

    switch (arg1){
        case 0:
            flag = state->curveSetup->type22.usedBit;
            return main_get_bits(flag);
        case 1:
            *deltaY = self->srt.transl.y - kyte->srt.transl.y;
            return 1;
        case 2:
            return 250;
        case 3:
            return 0;
        default:
            return 0;
    }
}

// offset: 0x460 | func: 10 | export: 10
u32 perchObject_func_460(Object *self) {
    return 4;
}

// offset: 0x470 | func: 11
s32 perchObject_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    s32 pad;
    Object* kyte;
    PerchObject_Data* state;
    PerchObject_Setup* createInfo;

    get_sidekick(); //@bug: not stored to variable, called again later!
    state = self->data;
    createInfo = (PerchObject_Setup*)self->setup;

    kyte = get_sidekick();    
    if (kyte) {
        if (vec3_distance_squared(&get_player()->positionMirror, (Vec3f*)&(state->curveSetup)->base.x) <= (createInfo->interactionDistance * createInfo->interactionDistance)) {
            ((DLL_Unknown*)kyte->dll)->vtbl->func[14].withTwoArgs((s32)kyte, 1);
            if (gDLL_1_UI->vtbl->func7(1)) {
                main_set_bits(BIT_Kyte_Flight_Curve, createInfo->kyteFlightGroup);
            }
        }
    }
    return 0;
}

/*0x0*/ static const char str_0[] = " Could Not Find node %i ";
/*0x1C*/ static const char str_1C[] = "should activate the command\n";
/*0x3C*/ static const char str_3C[] = " Kyte Landed Action ";
/*0x54*/ static const char str_54[] = "\n\n Setting Used Bit \n\n";
/*0x6C*/ static const char str_6C[] = "should activate the command\n";
/*0x8C*/ static const char str_8C[] = "";
