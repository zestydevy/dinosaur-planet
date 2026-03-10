#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 unused18;
    s8 gamebitIndexWin;
    s16 unused1A;
    s16 unused1C;
    s16 gamebitVehicleHit;
    s16 gamebitFreeTick;
} CRSpellStone_Setup;

typedef struct {
/*00*/ u8 state;
} CRSpellStone_Data;

typedef enum {
    CRSpellStone_STATE_Invisible = 0,
    CRSpellStone_STATE_Visible = 1,
    CRSpellStone_STATE_Rescued = 2
} CRSpellStone_States;

/** An array of possible gamebitIDs to set when winning the race */
/*0x0*/ static s16 dWinBits[] = {
    BIT_Play_Seq_02A9_CF_Race_End, BIT_Play_Seq_02A9_CF_Race_End
};

static int CRSpellStone_anim_callback(Object* self, Object* override, AnimObj_Data* aData, s8 arg3);

// offset: 0x0 | ctor
void CRSpellStone_ctor(void *dll) { }

// offset: 0xC | dtor
void CRSpellStone_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CRSpellStone_setup(Object *self, CRSpellStone_Setup *objSetup, s32 arg2) {
    CRSpellStone_Data* objData = self->data;
    obj_add_object_type(self, OBJTYPE_32);
    objData->state = CRSpellStone_STATE_Visible;
    self->animCallback = CRSpellStone_anim_callback;
}

// offset: 0x80 | func: 1 | export: 1
void CRSpellStone_control(Object* self) {
    CRSpellStone_Data* objData;
    CRSpellStone_Setup* objSetup;

    objData = self->data;
    objSetup = (CRSpellStone_Setup*)self->setup;
    
    if (objData->state == CRSpellStone_STATE_Rescued) {
        self->srt.pitch = 0;
        self->srt.roll = 0;
        self->srt.yaw += 0x100;
    }

    if (main_get_bits(objSetup->gamebitVehicleHit) != 0) {
        STUBBED_PRINTF(" DO Win BIT ");
        main_set_bits(dWinBits[objSetup->gamebitIndexWin], 1);
        self->srt.flags |= 0x4000;
        obj_free_tick(self);
    } else {
        if (main_get_bits(objSetup->gamebitFreeTick) != 0) {
            self->srt.flags |= 0x4000;
            obj_free_tick(self);
        }

        if ((objData->state == CRSpellStone_STATE_Rescued) && 
            (vec3_distance(&self->positionMirror, &get_player()->positionMirror) < 105.0f)
        ) {
            STUBBED_PRINTF("\n\n Hit By ONVEHICLE HIT \n\n");
            main_set_bits(objSetup->gamebitVehicleHit, 1);
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        }
    }
}

// offset: 0x218 | func: 2 | export: 2
void CRSpellStone_update(Object *self) { }

// offset: 0x224 | func: 3 | export: 3
void CRSpellStone_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    CRSpellStone_Data* objData = self->data;
    
    if (visibility && objData->state != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x288 | func: 4 | export: 4
void CRSpellStone_free(Object* self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_32);
}

// offset: 0x2C8 | func: 5 | export: 5
u32 CRSpellStone_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2D8 | func: 6 | export: 6
u32 CRSpellStone_get_data_size(Object *self, u32 a1) {
    return sizeof(CRSpellStone_Data);
}

// offset: 0x2EC | func: 7 | export: 7
/** 
  * Sets the SpellStone's state value, and returns a Boolean:
  * checking whether the SpellStone was previously in its "visible" state 
  */
int CRSpellStone_set_state(Object* self, s32 newValue) {
    CRSpellStone_Data* objData = self->data;
    u8 previousValue = objData->state;
    
    objData->state = newValue;
    if (newValue == 2) {
        self->srt.transl.y += 4.0f;
    }
    
    return !(previousValue == CRSpellStone_STATE_Visible);
}

// offset: 0x334 | func: 8 | export: 8
/** 
  * Returns a Boolean: checks whether the SpellStone has been rescued 
  */
int CRSpellStone_is_not_rescued(Object* self) {
    CRSpellStone_Data* objData = self->data;
    return objData->state != CRSpellStone_STATE_Rescued;
}

// offset: 0x34C | func: 9
static int CRSpellStone_anim_callback(Object* self, Object* override, AnimObj_Data* aData, s8 arg3) {
    return 0;
}
