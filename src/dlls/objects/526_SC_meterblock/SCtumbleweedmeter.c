#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unused18;
    s16 weedsNeeded;            //The total Tumbleweeds that must be damaged to fill the meter
    s16 gamebitDeactivated;     //Meter deactivated while this Gamebit is set
    s16 gamebitActivated;       //Meter deactivated while this Gamebit is unset
    s16 gamebitMeterProgress;   //Gamebit tracking how many Tumbleweeds have been offered to the meter
} SCMeterBlock_Setup;

typedef struct {
    s32 unused0;
    s32 unused4;
    f32 meterUnitY;             //Distance to step along Y as each Tumbleweed is offered
    s16 weedsOffered;           //How many Tumbleweeds have been offered to the meter
    u32 soundHandle;            //Stops a looping sound when meter freed (unused, since no sound is played)
} SCMeterBlock_Data;

#define METER_HEIGHT 171.0f

// offset: 0x0 | ctor
void SCMeterBlock_ctor(void *dll) { }

// offset: 0xC | dtor
void SCMeterBlock_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCMeterBlock_setup(Object* self, SCMeterBlock_Setup* objSetup, s32 arg2) {
    SCMeterBlock_Data* objData = self->data;

    //Calculate the distance to step along Y whenever a Tumbleweed is offered
    objData->meterUnitY = METER_HEIGHT / objSetup->weedsNeeded;

    //Restore meter's position based on gamebit
    objData->weedsOffered = main_get_bits(objSetup->gamebitMeterProgress);
    self->srt.transl.y += objData->weedsOffered * objData->meterUnitY;
    
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
    
    //Using a gamebit to check when a Tumbleweed is damaged, initialise it to 0
    main_set_bits(BIT_Damaged_a_Tumbleweed, 0);
}

// offset: 0xD4 | func: 1 | export: 1
void SCMeterBlock_control(Object* self) {
    SCMeterBlock_Data* objData;
    SCMeterBlock_Setup* objSetup;

    objSetup = (SCMeterBlock_Setup*)self->setup;
    objData = self->data;

    //Do nothing if deactivated, or if the puzzle's complete
    if (!main_get_bits(objSetup->gamebitActivated) || 
        main_get_bits(objSetup->gamebitDeactivated) || 
        (objData->weedsOffered >= objSetup->weedsNeeded)) {
        return;
    }
    
    //Check if a Tumbleweed was damaged
    if (main_get_bits(BIT_Damaged_a_Tumbleweed)) {
        //Raise the meter block up by one step, and set gamebits
        main_set_bits(BIT_Damaged_a_Tumbleweed, 0);
        main_set_bits(objSetup->gamebitMeterProgress, objData->weedsOffered);
        self->srt.transl.y += objData->meterUnitY;
        objData->weedsOffered++;
        
        //Open the way to Golden Plains when the meter is fully raised
        if (objData->weedsOffered >= objSetup->weedsNeeded) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, 0, 0, 0, 0);
            main_set_bits(BIT_SC_MeterBlock_Completed, TRUE);
        } else {
            main_set_bits(BIT_SC_MeterBlock_Completed, FALSE);
        }
    }
}

// offset: 0x240 | func: 2 | export: 2
void SCMeterBlock_update(Object *self) { }

// offset: 0x24C | func: 3 | export: 3
void SCMeterBlock_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2A0 | func: 4 | export: 4
void SCMeterBlock_free(Object* self, s32 arg1) {
    SCMeterBlock_Setup* objSetup;
    SCMeterBlock_Data* objData;

    objSetup = (SCMeterBlock_Setup*)self->setup;
    objData = self->data;

    //Store meter progress to gamebit
    main_set_bits(objSetup->gamebitMeterProgress, objData->weedsOffered);

    //Stop looping sound (may suggest the meter was intended to play a sound while moving up)
    if (objData->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x328 | func: 5 | export: 5
u32 SCMeterBlock_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x338 | func: 6 | export: 6
u32 SCMeterBlock_get_data_size(Object *self, u32 a1){
    return sizeof(SCMeterBlock_Data);
}
