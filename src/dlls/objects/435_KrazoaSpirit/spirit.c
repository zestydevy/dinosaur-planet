#include "dll.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "game/objects/object_id.h"
#include "sys/gfx/animseq.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/rand.h"

/*0x0*/ static DLL_Unknown *data_modGfx = NULL;
/*0x4*/ static Object *data_lfxEmitter = NULL;

// offset: 0x0 | ctor
void Spirit_ctor(void *dll) { }

// offset: 0xC | dtor
void Spirit_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Spirit_setup(Object* self, AnimObj_Setup* objSetup, s32 arg2) {
    AnimObj_Data* objData;
    s32 temp_v0;
    s16 id;

    objData = self->data;
    
    objData->eventGamebit = objSetup->unk1A;
    objData->unk7A = -1;
    objData->unk24 = 1.0f / (objSetup->camEaseDuration + 1.0f);
    objData->unk28 = -1;
    
    temp_v0 = self->unkDC;
    
    if ((temp_v0 == 0) && (objSetup->sequenceIdBitfield != 1)) {
        gDLL_3_Animation->vtbl->init_curve(objData, objSetup);
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    } else if (temp_v0 && (temp_v0 != objSetup->sequenceIdBitfield + 1)) {
        gDLL_3_Animation->vtbl->free_curve(objData);
        if (objSetup->sequenceIdBitfield != -1) {
            gDLL_3_Animation->vtbl->init_curve(objData, objSetup);
        }
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    }
    
    id = self->id;
    self->opacityWithFade = 0;
    self->opacity = 0;
    
    if (id == OBJ_DBSH_SpiritPriz || 
        id == OBJ_DFSH_SpiritPriz || 
        id == OBJ_MMSH_SpiritPriz || 
        id == OBJ_ECSH_SpiritPriz || 
        id == OBJ_GPSH_SpiritPriz || 
        id == OBJ_CCSH_SpiritPriz || 
        id == OBJ_WGSH_SpiritPriz || 
        id == OBJ_NWSH_SpiritPriz) {   
        data_modGfx = dllLoadDeferred(0x102D, 1);
    }
}

// offset: 0x1FC | func: 1 | export: 1
void Spirit_control(Object* self) {
    s32 index;
    AnimObj_Setup *setup;
    s32 otherMatchCount;
    Object* obj;
    s8 searchValue;
    s16 id;
    Object* matchObject;
    LFXEmitter_Setup* lfxSetup;
    AnimObj_Data* objData;
    s32 numObjects;
    AnimObj_Data* objData2;
    Object** objects;
    SRT transform;
    AnimObj_Data *temp;

    objData = self->data;
    setup = (AnimObj_Setup*)self->setup;
    
    if (!setup || setup->sequenceIdBitfield == -1){
        return;
    }
    
    index = gDLL_3_Animation->vtbl->tick_obj(self, gUpdateRateMirror);

    if (index && (self->seqSlot == SEQSLOT_ANIMOBJ)) {
        objData2 = (AnimObj_Data *) self->data;
        temp = objData2;
        searchValue = temp->seqSlot;

        matchObject = NULL;
        objects = get_world_objects(&index, &numObjects);
        otherMatchCount = 0;

        for (index = 0; index < numObjects; index++){
            obj = objects[index];
            
            if (searchValue == obj->seqSlot) {
                matchObject = obj;
            }
            if ((obj->seqSlot == SEQSLOT_ANIMOBJ) && (obj->controlNo == OBJCONTROL_AnimObj)){
                objData2 = (AnimObj_Data *)obj->data;
                if (searchValue == objData2->seqSlot) {
                    otherMatchCount++;
                }
            }
        }        
        
        if ((otherMatchCount < 2) && matchObject && (matchObject->seqSlot != SEQSLOT_NONE)) {
            matchObject->seqSlot = SEQSLOT_NONE;
            gDLL_3_Animation->vtbl->end_obj_sequence(searchValue);
        }
        self->seqSlot = SEQSLOT_NONE;
        obj_destroy_object(self);
    }
    
    if ((objData->lastMessage == 1) || (objData->lastMessage == 3)) {
        lfxSetup = obj_alloc_setup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
        lfxSetup->base.loadDistance = 0xFF;
        lfxSetup->base.fadeDistance = 0xFF;
        lfxSetup->base.x = self->srt.transl.x;
        lfxSetup->base.y = self->srt.transl.y;
        lfxSetup->base.z = self->srt.transl.z;
        lfxSetup->unk20 = 0;
        
        if (objData->lastMessage == 1) {
            lfxSetup->unk1E = 0x1CB;
        } else {
            lfxSetup->unk1E = 0x1CF;
        }
        
        lfxSetup->unk22 = -1;
        lfxSetup->unk18 = 0;
        lfxSetup->unk1A = 0;
        lfxSetup->unk1C = 0;
        lfxSetup->unk24 = 0;
        data_lfxEmitter = obj_create((ObjSetup *) lfxSetup, 5, self->mapID, -1, self->parent);
        objData->lastMessage = 0;
    } else if (objData->lastMessage == 2) {
        if (data_lfxEmitter) {
            obj_destroy_object(data_lfxEmitter);
            data_lfxEmitter = NULL;
        }
        objData->lastMessage = 0;
    }
    
    if (data_lfxEmitter) {
        data_lfxEmitter->srt.transl.x = self->srt.transl.x;
        data_lfxEmitter->srt.transl.y = self->srt.transl.y;
        data_lfxEmitter->srt.transl.z = self->srt.transl.z;
    }
    
    id = self->id;
    if (id == OBJ_DBSH_SpiritPriz || 
        id == OBJ_DFSH_SpiritPriz || 
        id == OBJ_MMSH_SpiritPriz || 
        id == OBJ_ECSH_SpiritPriz || 
        id == OBJ_GPSH_SpiritPriz || 
        id == OBJ_CCSH_SpiritPriz || 
        id == OBJ_WGSH_SpiritPriz || 
        id == OBJ_NWSH_SpiritPriz) {   
        transform.scale = self->srt.scale;
        gDLL_17_partfx->vtbl->spawn(self, 0x545, &transform, 2, -1, 0);
        gDLL_17_partfx->vtbl->spawn(self, 0x546, &transform, 2, -1, 0);
    }
}

// offset: 0x5A0 | func: 2 | export: 2
void Spirit_update(Object *self) { }

// offset: 0x5AC | func: 3 | export: 3
void Spirit_print(Object* self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    s16 id;
    Camera *camera;

    camera = camGet();
    if (self->id == OBJ_ECSH_SpiritCup && camera) {
        self->srt.yaw = 0xFFFF - camera->srt.yaw;
    }
    
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (self->opacity) {
            id = self->id;
            if (id == OBJ_DFSH_SpiritPriz || id == OBJ_CCSH_SpiritPriz) {
                if ((s16)rand_next(0, 1)) {
                    data_modGfx->vtbl->func[0].withSixArgs((s32)self, 0, 0, 4, -1, 0);
                }
            } else if (id == OBJ_MMSH_SpiritPriz || id == OBJ_WGSH_SpiritPriz) {
                if ((s16)rand_next(0, 1)) {
                    data_modGfx->vtbl->func[0].withSixArgs((s32)self, 1, 0, 4, -1, 0);
                }
            } else if (id == OBJ_ECSH_SpiritPriz || id == OBJ_GPSH_SpiritPriz) {
                if ((s16)rand_next(0, 1)) {
                    data_modGfx->vtbl->func[0].withSixArgs((s32)self, 2, 0, 4, -1, 0);
                }
            } else if (id == OBJ_DBSH_SpiritPriz || id == OBJ_NWSH_SpiritPriz){
                if ((s16)rand_next(0, 1)) {
                    data_modGfx->vtbl->func[0].withSixArgs((s32)self, 3, 0, 4, -1, 0);
                }
            }
        }
    }
}

// offset: 0x834 | func: 4 | export: 4
void Spirit_free(Object* self, s32 arg1) {
    s16 id;

    gDLL_3_Animation->vtbl->free_curve(self->data);
    
    id = self->id;
    if (id == OBJ_DBSH_SpiritPriz || 
        id == OBJ_DFSH_SpiritPriz || 
        id == OBJ_MMSH_SpiritPriz || 
        id == OBJ_ECSH_SpiritPriz || 
        id == OBJ_GPSH_SpiritPriz || 
        id == OBJ_CCSH_SpiritPriz || 
        id == OBJ_WGSH_SpiritPriz || 
        id == OBJ_NWSH_SpiritPriz) {
        dllFree(data_modGfx);
    }
    if ((arg1 == 0) && data_lfxEmitter) {
        obj_destroy_object(data_lfxEmitter);
        data_lfxEmitter = NULL;
    }
}

// offset: 0x91C | func: 5 | export: 5
u32 Spirit_get_model_flags(Object *self){
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x92C | func: 6 | export: 6
u32 Spirit_get_data_size(Object *self, u32 a1) {
    return sizeof(AnimObj_Data);
}
