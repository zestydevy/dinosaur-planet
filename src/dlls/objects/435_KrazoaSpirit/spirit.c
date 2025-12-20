#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "functions.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/rand.h"

typedef struct {
    AnimObj_Setup base;
} Spirit_Setup;

typedef struct {
    AnimObj_Data base;
} Spirit_Data;

/*0x0*/ static DLL_Unknown *data_modGfx = NULL;
/*0x4*/ static Object *data_lfxEmitter = NULL;

// offset: 0x0 | ctor
void Spirit_ctor(void *dll) { }

// offset: 0xC | dtor
void Spirit_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Spirit_setup(Object* self, Spirit_Setup* objSetup, s32 arg2) {
    Spirit_Data* objData;
    s32 temp_v0;
    s16 id;

    objData = self->data;
    
    objData->base.unk76 = objSetup->base.unk1A;
    objData->base.unk7A = -1;
    objData->base.unk24 = 1.0f / ((u8)objSetup->base.unk24 + 1.0f);
    objData->base.unk28 = -1;
    
    temp_v0 = self->unkDC;
    
    if ((temp_v0 == 0) && (objSetup->base.sequenceIdBitfield != 1)) {
        gDLL_3_Animation->vtbl->func6(objData, objSetup);
        self->unkDC = objSetup->base.sequenceIdBitfield + 1;
    } else if (temp_v0 && (temp_v0 != objSetup->base.sequenceIdBitfield + 1)) {
        gDLL_3_Animation->vtbl->func8(objData);
        if (objSetup->base.sequenceIdBitfield != -1) {
            gDLL_3_Animation->vtbl->func6(objData, objSetup);
        }
        self->unkDC = objSetup->base.sequenceIdBitfield + 1;
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
        data_modGfx = dll_load_deferred(0x102D, 1);
    }
}

// offset: 0x1FC | func: 1 | export: 1
void Spirit_control(Object* self) {
    s32 index;
    Spirit_Setup *setup;
    s32 otherMatchCount;
    Object* obj;
    s8 searchValue;
    s16 id;
    Object* matchObject;
    LFXEmitter_Setup* lfxSetup;
    Spirit_Data* objData;
    s32 numObjects;
    AnimObj_Data* objData2;
    Object** objects;
    SRT transform;
    AnimObj_Data *temp;

    objData = self->data;
    setup = (Spirit_Setup*)self->setup;
    
    if (!setup || setup->base.sequenceIdBitfield == -1){
        return;
    }
    
    index = gDLL_3_Animation->vtbl->func4(self, gUpdateRateMirror);

    if (index && (self->unkB4 == -2)) {
        objData2 = (AnimObj_Data *) self->data;
        temp = objData2;
        searchValue = temp->unk63;

        matchObject = NULL;
        objects = get_world_objects(&index, &numObjects);
        otherMatchCount = 0;

        for (index = 0; index < numObjects; index++){
            obj = objects[index];
            
            if (searchValue == obj->unkB4) {
                matchObject = obj;
            }
            if ((obj->unkB4 == -2) && (obj->group == 0x10)){
                objData2 = (AnimObj_Data *)obj->data;
                if (searchValue == objData2->unk63) {
                    otherMatchCount++;
                }
            }
        }        
        
        if ((otherMatchCount < 2) && matchObject && (matchObject->unkB4 != -1)) {
            matchObject->unkB4 = -1;
            gDLL_3_Animation->vtbl->func18(searchValue);
        }
        self->unkB4 = -1;
        obj_destroy_object(self);
    }
    
    if ((objData->base.unk8D == 1) || (objData->base.unk8D == 3)) {
        lfxSetup = obj_alloc_create_info(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
        lfxSetup->base.loadDistance = 0xFF;
        lfxSetup->base.fadeDistance = 0xFF;
        lfxSetup->base.x = self->srt.transl.x;
        lfxSetup->base.y = self->srt.transl.y;
        lfxSetup->base.z = self->srt.transl.z;
        lfxSetup->unk20 = 0;
        
        if (objData->base.unk8D == 1) {
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
        objData->base.unk8D = 0;
    } else if (objData->base.unk8D == 2) {
        if (data_lfxEmitter) {
            obj_destroy_object(data_lfxEmitter);
            data_lfxEmitter = NULL;
        }
        objData->base.unk8D = 0;
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

    camera = get_camera();
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

    gDLL_3_Animation->vtbl->func8(self->data);
    
    id = self->id;
    if (id == OBJ_DBSH_SpiritPriz || 
        id == OBJ_DFSH_SpiritPriz || 
        id == OBJ_MMSH_SpiritPriz || 
        id == OBJ_ECSH_SpiritPriz || 
        id == OBJ_GPSH_SpiritPriz || 
        id == OBJ_CCSH_SpiritPriz || 
        id == OBJ_WGSH_SpiritPriz || 
        id == OBJ_NWSH_SpiritPriz) {
        dll_unload(data_modGfx);
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
    return sizeof(Spirit_Data);
}
