#include "common.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "game/gamebits.h"
#include "macros.h"
#include "sys/map.h"
#include "sys/objmsg.h"

typedef struct {
    ObjSetup base;
    Object* fireCrystal;
    s8 colourR;
    s8 colourG;
    s8 colourB;
} CCfirecrystalin_Setup; //0x20

typedef struct {
    ObjSetup base;
    s16 gamebitCollected;
} CCfirecrystal_Setup;

typedef struct {
    u8 state;
    Object* lfxEmitters[4];
    NewLfxStruct* lfxEmitterSetup;
} CCfirecrystal_Data;

typedef enum {
    FireCrystal_State_Collectable = 0,
    FireCrystal_State_1 = 1,
    FireCrystal_State_2 = 2,
    FireCrystal_State_3 = 3
} CCfirecrystal_States;

void CCfirecrystal_free(Object* self, s32 arg1);
static Object* CCfirecrystal_create_light(Object* self, s8 colourR, s8 colourG, u8 colourB);
static s32 CCfirecrystal_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void CCfirecrystal_ctor(void *dll) { }

// offset: 0xC | dtor
void CCfirecrystal_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCfirecrystal_setup(Object* self, CCfirecrystal_Setup* objSetup, s32 arg2) {
    CCfirecrystal_Data* objData;
    void* lfxBuffer;

    objData = self->data;
    
    self->animCallback = (void*)CCfirecrystal_anim_callback;
    
    if (objSetup->gamebitCollected == NO_GAMEBIT) {
        objData->state = FireCrystal_State_2;
        self->unkAF = 8;
        self->objhitInfo->unk58 = 256;
    } else {
        objData->state = main_get_bits(objSetup->gamebitCollected);
    }
    
    if (objData->state == FireCrystal_State_1) {
        self->unkAF = 8;
        self->objhitInfo->unk58 = 256;
        objData->lfxEmitters[3] = 0;
        objData->lfxEmitters[2] = 0;
        objData->lfxEmitters[1] = 0;
        objData->lfxEmitters[0] = 0;
    } else {
        lfxBuffer = mmAlloc(sizeof(LFXEmitter_Setup), ALLOC_TAG_LFX_COL, NULL);
        objData->lfxEmitterSetup = lfxBuffer;
        queue_load_file_region_to_ptr(lfxBuffer, LACTIONS_BIN, 0x6158, 0x28);
        objData->lfxEmitterSetup->unk10 = 0xFFFE;
        gDLL_11_Newlfx->vtbl->func0(self, self, (LFXEmitter_Setup*)objData->lfxEmitterSetup, 0, 0, 0);
        objData->lfxEmitters[0] = CCfirecrystal_create_light(self, 0x40, 0, 0x18);
        objData->lfxEmitters[1] = CCfirecrystal_create_light(self, 0x40, 0x40, 0x18);
        objData->lfxEmitters[2] = CCfirecrystal_create_light(self, 0xC8, 0, 0x18);
        objData->lfxEmitters[3] = CCfirecrystal_create_light(self, 0xC8, 0x40, 0x18);
    }
    
    obj_init_mesg_queue(self, 1);
    self->unkB0 |= 0x2000;
}

// offset: 0x210 | func: 1 | export: 1
// Credit to "Carnivorous Herring"
void CCfirecrystal_control(Object* self) {
    CCfirecrystal_Data* objData;
    CCfirecrystal_Setup* objSetup;
    u32 message;

    objSetup = (CCfirecrystal_Setup*)self->setup;
    objData = self->data;
    
    self->opacity = rand_next(0, 56) + 100;
    
    switch (objData->state) {
    case FireCrystal_State_3:
        while (obj_recv_mesg(self, &message, 0, 0)){
            switch (message) {
            case 0x7000B:
                objData->state = FireCrystal_State_1;
                CCfirecrystal_free(self, 0);
            default:
                break;
            }
        } 
        break;
    case FireCrystal_State_Collectable:
        if (func_80032538(self) != 0) {
            main_set_bits(objSetup->gamebitCollected, 1);
            main_increment_bits(BIT_CC_Fire_Crystal);
            objData->state = FireCrystal_State_3;
            self->unkAF = 8;
            self->objhitInfo->unk58 = 0x100;
            obj_send_mesg(get_player(), 0x7000A, self, (void*)0x1EA);
        }
        break;
    }
}

// offset: 0x3A8 | func: 2 | export: 2
void CCfirecrystal_update(Object *self) { }

// offset: 0x3B4 | func: 3 | export: 3
void CCfirecrystal_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CCfirecrystal_Data* objData = self->data;
    if (visibility && (objData->state != FireCrystal_State_1)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x41C | func: 4 | export: 4
void CCfirecrystal_free(Object* self, s32 arg1) {
    CCfirecrystal_Data* objData;
    NewLfxStruct* lfxStruct;
    s32 index;

    objData = self->data;

    if (arg1 == 0){
        for (index = 0; index < 4; index++){
            if (objData->lfxEmitters[index] != NULL) {
                obj_destroy_object(objData->lfxEmitters[index]);
                objData->lfxEmitters[index] = NULL;
            }
        }
    }
    
    lfxStruct = objData->lfxEmitterSetup;
    if (lfxStruct != NULL) {
        STUBBED_PRINTF("trying to kill fire crystal light\n");
        lfxStruct->unk12.asByte = 2;
        objData->lfxEmitterSetup->unke = 0;
        objData->lfxEmitterSetup->unk1b = 0;
        gDLL_11_Newlfx->vtbl->func0(self, self, objData->lfxEmitterSetup, 0, 0, 0);
        mmFree(objData->lfxEmitterSetup);
        objData->lfxEmitterSetup = NULL;
    }
}

// offset: 0x518 | func: 5 | export: 5
u32 CCfirecrystal_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x528 | func: 6 | export: 6
u32 CCfirecrystal_get_data_size(Object *self, u32 a1) {
    return sizeof(CCfirecrystal_Data);
}

// offset: 0x53C | func: 7
Object* CCfirecrystal_create_light(Object* self, s8 colourR, s8 colourG, u8 colourB) {
    CCfirecrystalin_Setup* lightSetup;
    CCfirecrystal_Setup* objSetup;

    objSetup = (CCfirecrystal_Setup*)self->setup;

    lightSetup = obj_alloc_create_info(sizeof(CCfirecrystalin_Setup), OBJ_CCfirecrystalin);
    lightSetup->base.x = objSetup->base.x;
    lightSetup->base.y = objSetup->base.y;
    lightSetup->base.z = objSetup->base.z;
    lightSetup->base.loadFlags = 2;
    lightSetup->base.byte6 = objSetup->base.byte6;
    lightSetup->base.byte5 = objSetup->base.byte5;
    lightSetup->base.fadeDistance = objSetup->base.fadeDistance;
    lightSetup->fireCrystal = self;
    //r, g, b for light, maybe?
    lightSetup->colourR = colourR; 
    lightSetup->colourG = colourG;
    lightSetup->colourB = colourB;
    return obj_create((ObjSetup*)lightSetup, 5, -1, -1, self->parent);
}

// offset: 0x614 | func: 8
/** Randomises opacity */
s32 CCfirecrystal_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    self->opacity = rand_next(0, 56) + 100;
    return 0;
}

/*0x24*/ static const char str_24[] = "";
/*0x28*/ static const char str_28[] = "";
/*0x2C*/ static const char str_2C[] = "";
