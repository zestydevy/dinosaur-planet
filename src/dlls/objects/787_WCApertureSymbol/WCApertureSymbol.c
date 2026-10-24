#include "dlls/objects/210_player.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/animseq.h"
#include "sys/vi.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "dll.h"

typedef struct {
/*0*/ s16 targetOpacity;
/*2*/ u8 state;
/*3*/ u8 flags;
} WCApertureSymbol_Data;

typedef enum {
    STATE_Inactive,
    STATE_Waiting_for_View,
    STATE_Viewed
} WCApertureSymbol_States;

typedef enum {
    WCApertureSymbol_MODELIDX_Sun,
    WCApertureSymbol_MODELIDX_Moon
} WCApertureSymbol_ModelIndices;

typedef enum {
    WCApertureSymbol_FLAG_Visible = 1
} WCApertureSymbol_Flags;

#define VIEWING_TERRAIN_TYPE 0x21
#define VIEWING_DURATION_SUN 8000
#define VIEWING_DURATION_MOON 4500
#define PEAK_SUN 70000
#define PEAK_MOON 79250

static int WCApertureSymbol_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static s16 WCApertureSymbol_getTargetOpacity(Object* self, WCApertureSymbol_Data* objdata, f32 minTimeOfDay, f32 maxTimeOfDay, f32 timeOfDay);

// offset: 0x0 | ctor
void WCApertureSymbol_ctor(void* dll) { }

// offset: 0xC | dtor
void WCApertureSymbol_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCApertureSymbol_setup(Object* self, WCApertureSymbol_Setup* setup, s32 reset) {
    WCApertureSymbol_Data* objdata = self->data;

    self->srt.yaw = setup->yaw << 8;
    self->animCallback = WCApertureSymbol_animCallback;

    self->modelInstIdx = setup->type;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = WCApertureSymbol_MODELIDX_Sun;
    }

    if (mainGetBits(setup->bitEnabled)) {
        if (mainGetBits(setup->bitViewed)) {
            objdata->state = STATE_Viewed;
        } else {
            objdata->state = STATE_Waiting_for_View;
        }
    }

    self->opacity = 1;
    objdata->targetOpacity = 0;
}

// offset: 0xFC | func: 1 | export: 1
void WCApertureSymbol_control(Object* self) {
    WCApertureSymbol_Setup* setup;
    f32 time;
    WCApertureSymbol_Data* objdata;
    s32 opacity;
    Object* player;

    setup = (WCApertureSymbol_Setup*)self->setup;
    objdata = self->data;
    
    player = objGetPlayer();
    objdata->targetOpacity = 0;
    
    switch (objdata->state) {
    case STATE_Viewed:
        objdata->targetOpacity = OBJECT_OPACITY_MAX;
        break;
    case STATE_Inactive:
        break;
    case STATE_Waiting_for_View:
        if (gDLL_2_Camera->vtbl->get_dll_ID() == DLL_ID_CAM1STPERSON) {
            if ((((DLL_210_Player*)player->dll)->vtbl->func70(player) == VIEWING_TERRAIN_TYPE) && (vec3Distance(&player->globalPosition, &self->globalPosition) < 200.0f)) {
                gDLL_7_Newday->vtbl->func4(&time);

                if (setup->type == WCApertureSymbol_MODELIDX_Sun) {
                    objdata->targetOpacity = WCApertureSymbol_getTargetOpacity(self, objdata, 
                        PEAK_SUN - VIEWING_DURATION_SUN/2, 
                        PEAK_SUN + VIEWING_DURATION_SUN/2, 
                        time
                    );
                } else {
                    objdata->targetOpacity = WCApertureSymbol_getTargetOpacity(self, objdata, 
                        PEAK_MOON - VIEWING_DURATION_MOON/2, 
                        PEAK_MOON + VIEWING_DURATION_MOON/2, 
                        time
                    );
                }

                if (self->opacity > setup->opacityThreshold) {
                    mainSetBits(setup->bitViewed, TRUE);
                    objdata->state = STATE_Viewed;
                    objdata->targetOpacity = OBJECT_OPACITY_MAX;
                }
            }
        }
        break;
    }

    if (self->opacity < objdata->targetOpacity) {
        opacity = self->opacity + (gUpdateRate * 4);
        if (objdata->targetOpacity < opacity) {
            opacity = objdata->targetOpacity;
        }
        self->opacity = opacity;
    } else if (self->opacity > objdata->targetOpacity) {
        opacity = self->opacity - (gUpdateRate * 4);
        if (opacity < objdata->targetOpacity) {
            opacity = objdata->targetOpacity;
        }
        self->opacity = opacity;
    }
}

// offset: 0x358 | func: 2 | export: 2
void WCApertureSymbol_update(Object* self) { }

// offset: 0x364 | func: 3 | export: 3
void WCApertureSymbol_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    WCApertureSymbol_Data* objdata = self->data;

    if (visibility) {
        objdata->flags |= WCApertureSymbol_FLAG_Visible;
    } else {
        objdata->flags &= ~WCApertureSymbol_FLAG_Visible;
    }

    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3E4 | func: 4 | export: 4
void WCApertureSymbol_free(Object* self, s32 onlySelf) { }

// offset: 0x3F4 | func: 5 | export: 5
u32 WCApertureSymbol_get_model_flags(Object* self) {
    WCApertureSymbol_Setup* setup = (WCApertureSymbol_Setup*)self->setup;
    s8 modelIdx = setup->type;

    if (modelIdx >= self->def->numModels) {
        modelIdx = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}


// offset: 0x428 | func: 6 | export: 6
u32 WCApertureSymbol_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(WCApertureSymbol_Data);
}

// offset: 0x43C | func: 7
static int WCApertureSymbol_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    WCApertureSymbol_Data* objdata;
    s32 i;

    objdata = self->data;

    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i] == 1) {
            objdata->state = STATE_Waiting_for_View;
        }
    }

    return 0;
}

// offset: 0x490 | func: 8
static s16 WCApertureSymbol_getTargetOpacity(Object* self, WCApertureSymbol_Data* objdata, f32 minTimeOfDay, f32 maxTimeOfDay, f32 timeOfDay) {
    s32 screenX;
    s32 screenY;
    f32 projectedX;
    f32 projectedY;
    f32 projectedZ;
    f32 halfWidth;
    f32 halfHeight;
    f32 halfDuration;
    f32 dpos;
    f32 dposY;
    f32 x;
    f32 z;
    f32 dt;
    u32 screenDimensions;
    u32 screenWidth;
    u32 screenHeight;

    x = self->srt.transl.x - gWorldX;
    z = self->srt.transl.z - gWorldZ;

    //Return with 0 opacity if the symbol is off-screen
    camProjectPoint(x, self->srt.transl.y, z, &projectedX, &projectedY, &projectedZ);
    camClipToScreen(projectedX, projectedY, projectedZ, &screenX, &screenY, NULL);
    if (viContainsPoint(screenX, screenY) == FALSE) {
        return 0;
    }

    //Return with 0 opacity if the current time-of-day is outside viewing hours
    if (timeOfDay < minTimeOfDay || timeOfDay > maxTimeOfDay) {
        return 0;
    }

    screenDimensions = viGetCurrentSize();
    screenHeight = GET_VIDEO_HEIGHT(screenDimensions);
    screenWidth = GET_VIDEO_WIDTH(screenDimensions);

    //Return with 0 opacity if the symbol is off-screen
    if ((u32)screenX > screenWidth || screenX < 0) {
        return 0;
    }
    if ((u32)screenY > screenHeight || screenY < 0) {
        return 0;
    }

    //Get a position tValue expressing how close the symbol is to the centre of screen (1 at screen centre, 0 at edges)
    {
        halfWidth = screenWidth/2;
        halfHeight = screenHeight/2;

        dpos = screenX - halfWidth;
        if (dpos < 0.0f) {
            dpos = -dpos;
        }

        dposY = screenY - halfHeight;
        if (screenY < halfHeight) {
            dposY = -(screenY - halfHeight);
        }

        dpos = halfWidth - dpos;
        dposY = halfHeight - dposY;
        dpos *= dpos;
        dposY *= dposY;
        halfHeight *= halfHeight;
        halfWidth *= halfWidth;
        dpos /= halfWidth;
        dposY /= halfHeight;

        dpos += dposY;
        dpos *= 0.5f;
        if (dpos > 1.0f) {
            dpos = 1.0f;
        } else if (dpos < 0) {
            dpos = 0;
        }
    }

    //Get a time tValue based around the peak viewing hour
    {
        //Get the difference between current time-of-day and the min/max viewing hours' halfway time
        dt = timeOfDay - ((maxTimeOfDay + minTimeOfDay) * 0.5f);
        if (dt < 0.0f) { //Use absolute value
            dt = -dt;
        }

        //Get a tValue: 0 at min/max viewing time, 1 at the halfway/peak viewing time 
        halfDuration = ((maxTimeOfDay + minTimeOfDay) * 0.5f) - minTimeOfDay;
        dt = halfDuration - dt;
        dt /= halfDuration;
        if (dt > 1.0f) {
            dt = 1.0f;
        } else if (dt < 0) {
            dt = 0;
        }
    }

    diPrintf("dpos=%f dt=%f\n", &dpos, &dt);

    return (dpos * dt * OBJECT_OPACITY_MAX);
}
