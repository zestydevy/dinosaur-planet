#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "macros.h"
#include "sys/objtype.h"

#include "dlls/objects/469_ECSHshrine.h"

#define SINK_DISTANCE 50.0f

/*0x0*/ static Object* dShrine = NULL; //ECSH_Shrine object

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 cupIndex;
} ECSHCup_Setup;

typedef struct {
    Vec3f home;
    f32 speedX;
    f32 speedY;
    f32 speedZ;
    f32 groundY;
    s32 prevState;
    s32 cupIndex;
    s16 fxTimer;
    s16 rotateSpeed;
    s16 bobTimer;
    s8 bobSpeed;
} ECSHCup_Data;

// offset: 0x0 | ctor
void ECSHCup_ctor(void *dll) { }

// offset: 0xC | dtor
void ECSHCup_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void ECSHCup_setup(Object* self, ECSHCup_Setup* objSetup, s32 arg2) {
    ECSHCup_Data* objData;
    f32 objectDistance;

    objData = self->data;
    objectDistance = 500.0f;
    
    objData->home.x = self->srt.transl.x;
    objData->home.y = self->srt.transl.y;
    objData->home.z = self->srt.transl.z;
    objData->groundY = self->srt.transl.y;
    self->srt.transl.y -= SINK_DISTANCE;
    objData->prevState = Cup_STATE_Stopped;
    
    if (0) { }
    
    objData->speedX = 0;
    objData->speedY = 0;
    objData->speedZ = 0;

    objData->cupIndex = objSetup->cupIndex;
    objData->bobTimer = rand_next(0, 600);
    objData->rotateSpeed = rand_next(-800, 800);
    objData->bobSpeed = 1;
    self->opacityWithFade = 0;
    objData->fxTimer = 0;
    
    if (dShrine == NULL) {
        dShrine = obj_get_nearest_type_to(OBJTYPE_13, self, &objectDistance);
    }
}

// offset: 0x13C | func: 1 | export: 1
void ECSHCup_control(Object* self) {
    ECSHCup_Data *objData = self->data;
    s32 state = Cup_STATE_Unknown;
    Vec3f goal = {0, 0, 0};
    u8 cupWithSpirit = 0;
    s16 opacity;
    f32 objectDistance = 500.0f;
    Object *player;
    
    player = get_player();
    
    if (dShrine == NULL){
        dShrine = obj_get_nearest_type_to(OBJTYPE_13, self, &objectDistance);
    }
    if (dShrine == NULL){
        return;
    }
    
    //Get cup minigame state, and the index of the cup holding the Spirit
    ((DLL_469_ECSHshrine*)dShrine->dll)->vtbl->get_minigame_state(&state, &cupWithSpirit);

    //Spin
    self->srt.yaw += objData->rotateSpeed; //@bug: framerate dependent

    //Create particles under cup
    if (state != Cup_STATE_Rise_Up){
        objData->fxTimer -= gUpdateRate;
        if (objData->fxTimer <= 0){
            objData->fxTimer = 10;
            if (((state != Cup_STATE_Underground) && (state != Cup_STATE_Rise_Up)) && (state != Cup_STATE_Sink_Down)){
                gDLL_17_partfx->vtbl->spawn(self, 0x270, 0, 0, -1, 0);
            }
        }
    }
    
    //Animate up/down oscillation (no easing, even spacing)
    objData->bobTimer -= gUpdateRate;
    if (objData->bobTimer <= 0){
        objData->bobSpeed = -objData->bobSpeed;
        objData->bobTimer = 100;
    }
    self->srt.transl.y += 0.09f * objData->bobSpeed;

    if (objData->cupIndex == cupWithSpirit){
    }
    
    //Move
    if ((state == Cup_STATE_Moving) && (objData->prevState == Cup_STATE_Moving)) {
        self->srt.transl.x += objData->speedX;
        self->srt.transl.z += objData->speedZ;
    }
    
    //STATE MACHINE
    if (state == Cup_STATE_Rise_Up) {
        //Rise up from ground
        if (self->srt.transl.y < objData->groundY) {
            self->srt.transl.y += 0.5f * gUpdateRateF;
        }
        
        //Fade in
        if (self->opacityWithFade != OBJECT_OPACITY_MAX) {
            opacity = self->opacityWithFade;
            opacity += gUpdateRate * 2;
            if (opacity >= OBJECT_OPACITY_MAX) {
                opacity = OBJECT_OPACITY_MAX;
            }
            self->opacityWithFade = opacity;
        }
        
        //Create particles
        objData->fxTimer -= gUpdateRate;
        if (objData->fxTimer <= 0) {
            objData->fxTimer = 10;
            gDLL_17_partfx->vtbl->spawn(self, 0x271, 0, 0, -1, 0);
        }
    } else if (state == Cup_STATE_Sink_Down) {
        //Sink into ground
        if ((objData->groundY - SINK_DISTANCE) < self->srt.transl.y) {
            self->srt.transl.y -= 0.5f * gUpdateRateF;
            
            //Create particles
            objData->fxTimer -= gUpdateRate;
            if (objData->fxTimer <= 0) {
                objData->fxTimer = 10;
                if (state != Cup_STATE_Underground) {
                    gDLL_17_partfx->vtbl->spawn(self, 0x271, 0, 0, -1, 0);
                }
            }
        }
        
        //Fade out
        if (self->opacityWithFade != 0) {
            opacity = self->opacityWithFade;
            opacity -= gUpdateRate * 2;
            if (opacity <= 0) {
                opacity = 0;
            }
            self->opacityWithFade = opacity;
        }
        
        gDLL_6_AMSFX->vtbl->func_1218(self);
    } else if ((state == Cup_STATE_Round_Start) && (state != objData->prevState)) {
        if (objData->cupIndex == cupWithSpirit) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            gDLL_6_AMSFX->vtbl->play_sound(0, SOUND_343_Eerie_Ringing, 0x57, 0, 0, 0, 0);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_33F_Cup_Slide_Loop, MAX_VOLUME, 0, 0, 0, 0);
        } else {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_33F_Cup_Slide_Loop, 0x1E, 0, 0, 0, 0);
        }
        objData->prevState = state;
    } else if ((state == Cup_STATE_Moving) && (state != objData->prevState)) {
        ((DLL_469_ECSHshrine*)dShrine->dll)->vtbl->get_cup_coords(objData->cupIndex, &goal.x, &goal.z);
        objData->speedX = (goal.x - self->srt.transl.x) / 12.0f;
        objData->speedZ = (goal.z - self->srt.transl.z) / 12.0f;
        objData->home.x = self->srt.transl.x;
        objData->home.z = self->srt.transl.z;
        objData->prevState = state;
    } else if ((state == Cup_STATE_Stopped) && (state != objData->prevState)) {
        objData->speedX = 0.0f;
        objData->speedZ = 0.0f;
        objData->prevState = state;
    } else if ((state == Cup_STATE_Shuffle) && (state != objData->prevState)) {
        objData->speedX = 0.0f;
        objData->speedZ = 0.0f;
        ((DLL_469_ECSHshrine*)dShrine->dll)->vtbl->set_cup_coords(objData->cupIndex, self->srt.transl.x, self->srt.transl.z);
        objData->prevState = state;
    } else if ((state == Cup_STATE_Underground) && (state != objData->prevState)) {
        objData->prevState = state;
    } else if ((state == Cup_STATE_Move_Finished) && (state != objData->prevState)) {
        ((DLL_469_ECSHshrine*)dShrine->dll)->vtbl->get_cup_coords(objData->cupIndex, &goal.x, &goal.z);
        self->srt.transl.x = goal.x;
        self->srt.transl.z = goal.z;
        objData->prevState = state;
    } else if ((state == Cup_STATE_Await_Choice) && player && vec3_distance(&self->positionMirror, &player->positionMirror) < 30.0f) {
        ((DLL_469_ECSHshrine*)dShrine->dll)->vtbl->choose_cup(objData->cupIndex);
        if (objData->cupIndex == cupWithSpirit) {
            gDLL_3_Animation->vtbl->func17(1, self, -1);
        }
    }
}

// offset: 0x8E8 | func: 2 | export: 2
void ECSHCup_update(Object *self) { }

// offset: 0x8F4 | func: 3 | export: 3
void ECSHCup_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x948 | func: 4 | export: 4
void ECSHCup_free(Object* arg0, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(arg0);
}

// offset: 0x990 | func: 5 | export: 5
u32 ECSHCup_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x9A0 | func: 6 | export: 6
u32 ECSHCup_get_data_size(Object *self, u32 a1){
    return sizeof(ECSHCup_Data);
}
