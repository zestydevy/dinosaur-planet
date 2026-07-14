#include "common.h"
#include "macros.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/609_WMFireFly.h"

typedef struct {
    ObjSetup base;
    u8 variance;
    u8 kyteCommandOutValue;
    u16 kyteFlightGroup;
    u8 fireflyCount;
} KyteFireFlys_Setup;

typedef struct {
    Object* fireflies[13];
    u8 fireflyCount;
    u8 initialFireflyCount;
    u8 haveCurveSetup;
    CurveSetup* curve;
} KyteFireFlys_Data;

static Object* KyteFireFlys_create_firefly(Object* self, s32 variance, s32 quarterVariance);

// offset: 0x0 | ctor
void KyteFireFlys_ctor(void *dll) { }

// offset: 0xC | dtor
void KyteFireFlys_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void KyteFireFlys_setup(Object* self, KyteFireFlys_Setup* objSetup, s32 reset) {
    KyteFireFlys_Data* objData;
    s32 i;

    objData = self->data;
    
    for (i = 0; i < objSetup->fireflyCount; i++) {
        objData->fireflies[i] = KyteFireFlys_create_firefly(self, objSetup->variance, objSetup->variance >> 2);
    }
    
    objData->initialFireflyCount = objData->fireflyCount = objSetup->fireflyCount;
    objData->haveCurveSetup = FALSE;
    objAddObjectType(self, OBJTYPE_KyteTarget);
}

// offset: 0xE0 | func: 1 | export: 1
void KyteFireFlys_control(Object* self) {
    KyteFireFlys_Data* objData;
    KyteFireFlys_Setup* objSetup;
    Object* sidekick;

    objData = self->data;
    objSetup = (KyteFireFlys_Setup*)self->setup;

    if (objData->haveCurveSetup == FALSE) {
        objData->curve = gDLL_25->vtbl->func_2A50(self, objSetup->kyteFlightGroup);
        if (objData->curve != NULL) {
            objData->haveCurveSetup = TRUE;
            self->srt.transl.x = objData->curve->pos.x;
            self->srt.transl.y = objData->curve->pos.y;
            self->srt.transl.z = objData->curve->pos.z;
        }
        return;
    } 
    
    if (objData->fireflyCount != 0) {
        sidekick = objGetSidekick();
        if (sidekick != NULL) {
            if (vec3_distance_squared(&objGetPlayer()->globalPosition, &objData->curve->pos) <= SQ(objSetup->variance)) {
                //Show Find command option
                ((DLL_ISidekick*)sidekick->dll)->vtbl->enable_command(sidekick, Sidekick_Command_INDEX_1_Find);

                if (gDLL_1_cmdmenu->vtbl->was_this_item_used(Sidekick_Command_INDEX_1_Find)) {
                    STUBBED_PRINTF("should activate the command\n");
                    mainSetBits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightGroup);
                }
            }
        }
    }
}

// offset: 0x26C | func: 2 | export: 2
void KyteFireFlys_update(Object *self) { }

// offset: 0x278 | func: 3 | export: 3
void KyteFireFlys_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x290 | func: 4 | export: 4
void KyteFireFlys_free(Object* self, s32 onlySelf) {
    KyteFireFlys_Data* objData;
    s32 i;

    objData = self->data;
    
    if (onlySelf == FALSE) {
        for (i = 0; i < objData->fireflyCount; i++) {
            objFreeObject(objData->fireflies[i]);
        }
    }

    objFreeObjectType(self, OBJTYPE_KyteTarget);
}

// offset: 0x330 | func: 5 | export: 5
u32 KyteFireFlys_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x340 | func: 6 | export: 6
u32 KyteFireFlys_get_data_size(Object *self, u32 a1) {
    return sizeof(KyteFireFlys_Data);
}

// offset: 0x354 | func: 7 | export: 7
s32 KyteFireFlys_func_354(Object* self, s32 arg1) {
    KyteFireFlys_Setup* objSetup;
    KyteFireFlys_Data* objData;
    s32 outValue;
    Object* firefly;
    Vec3f splineCoord;
    s16 gamebitID;
    u8 count;

    objSetup = (KyteFireFlys_Setup*)self->setup;
    objData = self->data;
    
    outValue = 0;

    if (arg1 == 2) {
        count = objData->fireflyCount;
        if (count != 0) {
            gamebitID = objData->curve->type22.usedBit;
            if (gamebitID != NO_GAMEBIT) {
                outValue = 0;
                mainSetBits(gamebitID, TRUE);
            }
            
            if (objData->fireflyCount != 0) {
                objData->fireflyCount--;
                firefly = objData->fireflies[objData->fireflyCount];
                splineCoord.x = self->srt.transl.x;
                splineCoord.y = self->srt.transl.y;
                splineCoord.z = self->srt.transl.z;
                splineCoord.y += 25.0f;
                ((DLL_609_WMFireFly*)firefly->dll)->vtbl->set_next_spline_coord(firefly, &splineCoord);
                outValue = objSetup->kyteCommandOutValue;
            }
        }
    }
    
    if (arg1 == 3) {
        objFreeObject(objData->fireflies[objData->fireflyCount]);
    }
    
    return outValue;
}

// offset: 0x49C | func: 8 | export: 8
s32 KyteFireFlys_func_49C(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x4B4 | func: 9 | export: 9
s32 KyteFireFlys_func_4B4(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x4CC | func: 10 | export: 10
s32 KyteFireFlys_func_4CC(Object* arg0) {
    return 8;
}

// offset: 0x4DC | func: 11
Object* KyteFireFlys_create_firefly(Object* self, s32 variance, s32 quarterVariance) {
    ObjSetup* objSetup;
    WMFireFly_Setup* setup;

    objSetup = self->setup;

    setup = objAllocSetup(sizeof(WMFireFly_Setup), OBJ_WLFireFly);
    setup->base.fadeDistance = objSetup->fadeDistance;
    setup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    setup->base.fadeFlags = objSetup->byte5;
    setup->base.x = rand_next(-quarterVariance, quarterVariance) + self->srt.transl.x;
    setup->base.y = self->srt.transl.y;
    setup->base.z = rand_next(-quarterVariance, quarterVariance) + self->srt.transl.z;
    setup->effectType = 3;
    setup->fxRange = variance;
    setup->varianceZ = quarterVariance;
    setup->varianceY = quarterVariance;
    return objSetupObject(&setup->base, (4 | 1), -1, -1, NULL);
}
