#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/engine/26_curves.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "functions.h"
#include "macros.h"
#include "sys/objects.h"
#include "sys/math.h"
#include "sys/main.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"
#include "game/gamebits.h"
#include "types.h"

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

static int perchobject_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

enum PerchObjectStates {
    STATE_0_Initialise = 0,
    STATE_1_Wait_for_Player_to_Instruct_Kyte = 1
};

// offset: 0x0 | ctor
void perchobject_ctor(void *dll) { }

// offset: 0xC | dtor
void perchobject_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void perchobject_setup(Object* self, s32 arg1, s32 arg2) {
    PerchObject_Data* objData = self->data;

    objData->stateIndex = STATE_0_Initialise;
    obj_add_object_type(self, OBJTYPE_48);
    self->animCallback = perchobject_anim_callback;
}

// offset: 0x74 | func: 1 | export: 1
void perchobject_control(Object* self) {
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
            curveSetup = gDLL_25->vtbl->func_2A50(self, objSetup->kyteFlightGroup);
            objData->curveSetup = curveSetup;
            if (curveSetup){
                objData->stateIndex = STATE_1_Wait_for_Player_to_Instruct_Kyte;
                return;
            }
            STUBBED_PRINTF(" Could Not Find node %i ");
            return;        
        case STATE_1_Wait_for_Player_to_Instruct_Kyte:
            kyte = get_sidekick();
            if (!kyte){
                break;
            }

            player = get_player();

            //Check distance between player and perch (either lateral X/Z distance or full 3D distance check)
            if (objSetup->useDistance3D){
                playerToCurveDistance = vec3_distance_squared(&player->positionMirror, (Vec3f *) (&objData->curveSetup->pos.x));
                playerIsNearby = (u8)(playerToCurveDistance <= (objSetup->interactionDistance * objSetup->interactionDistance));
            } else {
                playerToCurveDistance = vec3_distance_xz_squared(&player->positionMirror, (Vec3f *) (&objData->curveSetup->pos.x));
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
                if (gDLL_1_UI->vtbl->func_DF4(1)){
                    STUBBED_PRINTF("should activate the command\n");
                    main_set_bits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightGroup);
                }
            }
            break;

    }
}

// offset: 0x25C | func: 2 | export: 2
void perchobject_update(Object *self) { }

// offset: 0x268 | func: 3 | export: 3
void perchobject_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x2BC | func: 4 | export: 4
void perchobject_free(Object* self, s32 arg1) {
    obj_free_object_type(self, 0x30);
}

// offset: 0x2FC | func: 5 | export: 5
u32 perchobject_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x30C | func: 6 | export: 6
u32 perchobject_get_data_size(Object *self, u32 a1) {
    return sizeof(PerchObject_Data);
}

// offset: 0x320 | func: 7 | export: 7
// Called when Kyte finishes landing on unpressed switch (just before switch gets pressed down)
s32 perchobject_land_on_perch(Object* self, s32 arg1) {
    PerchObject_Data* objData;
    s32 landedOnPerch;

    objData = self->data;

    STUBBED_PRINTF(" Kyte Landed Action ");

    landedOnPerch = FALSE;
    if (arg1 == 5) {
        if (objData->curveSetup->type22.usedBit != NO_GAMEBIT) {
            main_set_bits(objData->curveSetup->type22.usedBit, 1);
            STUBBED_PRINTF("\n\n Setting Used Bit \n\n");
        }
        landedOnPerch = TRUE;
    }
    return landedOnPerch;
}

// offset: 0x388 | func: 8 | export: 8
u32 perchobject_func_388(Object *self, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x3A0 | func: 9 | export: 9
// Called when Kyte begins landing (entering slower flapping state while close to perch)
u32 perchobject_approach_perch(Object* self, s32 arg1, f32* deltaY) {
    Object *kyte;
    s16 flag;
    PerchObject_Data *objData;

    kyte = get_sidekick();
    objData = (PerchObject_Data*)self->data;

    switch (arg1){
        case 0:
            flag = objData->curveSetup->type22.usedBit;
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
u32 perchobject_func_460(Object *self) {
    return 4;
}

// offset: 0x470 | func: 11
static int perchobject_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    s32 pad;
    Object* kyte;
    PerchObject_Data* objData;
    PerchObject_Setup* objSetup;

    get_sidekick(); //@bug: not stored to variable, called again later!
    objData = self->data;
    objSetup = (PerchObject_Setup*)self->setup;

    kyte = get_sidekick();    
    if (kyte) {
        if (vec3_distance_squared(&get_player()->positionMirror, (Vec3f*)&(objData->curveSetup)->pos.x) <= (objSetup->interactionDistance * objSetup->interactionDistance)) {
            ((DLL_Unknown*)kyte->dll)->vtbl->func[14].withTwoArgs((s32)kyte, 1);
            if (gDLL_1_UI->vtbl->func_DF4(1)) {
                STUBBED_PRINTF("should activate the command\n");
                main_set_bits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightGroup);
            }
        }
    }
    return 0;
}
