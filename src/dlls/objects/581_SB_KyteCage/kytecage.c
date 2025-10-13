#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/objects/214_animobj.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "game/objects/object_def.h"
#include "segment_334F0.h"
#include "sys/main.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/gfx/gx.h"
#include "sys/dll.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "types.h"

void func_80034FF0(s32); //arg seems to be a MtxF*

// This is the cage object ("DLL 583 CageKyte" is the baby version of Kyte in the cage)

typedef struct {
s16 x;
s16 y;
s16 z;
} BoneRotation;

typedef struct {
    Object* kyte; //baby version
    union {
        s8 createLightning;
        u8 createLightningU;
    };
    s8 unk5;
    s8 unk6;
    s8 unk7;
} KyteCage_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
} KyteCage_Setup;

static s32 kyteCage_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void kyteCage_ctor(void *dll) { }

// offset: 0xC | dtor
void kyteCage_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void kyteCage_setup(Object* self, KyteCage_Setup* setup, s32 arg2) {
    KyteCage_Data* data = self->data;

    self->unk0xbc = (void*)&kyteCage_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    data->createLightning = 0;
    if (!main_get_bits(BIT_75)) {
        func_80000450(self, self, 0x58, 0, 0, 0);
        func_80000450(self, self, 0x6D, 0, 0, 0);
    }
}

// offset: 0xD0 | func: 1 | export: 1
void kyteCage_control(Object* self) {
    KyteCage_Data* objData;
    Object** objects;
    s32 count;
    s32 index;
    s32 parentDC;
    BoneRotation* rotate;

    objData = self->data;
    if (!objData->kyte) {
        objects = get_world_objects(&index, &count);

        for (index = 0; index < count; index++){
            if (objects[index]->id == OBJ_SB_CageKyte){
                objData->kyte = objects[index];
                index = count;
            }
        }
    }

    if (self->unk0xdc == 0) {
        self->unk0xdc = 1;
    }

    if (self->parent) {
        parentDC = self->parent->unk0xdc;
        rotate = (BoneRotation*)func_80034804(self, 0);
        if ((parentDC < 9) && (self->curModAnimId != 5)) {
            rotate->z = self->parent->srt.roll;
            func_80023D30(self, 5, 0, 0);
        } else if ((parentDC >= 9) && (self->curModAnimId != 9)) {
            rotate->z = 0;
            func_80023D30(self, 9, 0, 0);
        }
    }

    func_80024108(self, 0.004, delayByte, 0);
}

// offset: 0x298 | func: 2 | export: 2
void kyteCage_update(Object *self) { }

// offset: 0x2A4 | func: 3 | export: 3
void kyteCage_print(Object* self, Gfx** gfx, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    KyteCage_Data* objData;
    Object* kyte;
    u32 dYaw;
    ModelInstance* model;
    s8 boneIdx;
    s8 matrixIndex;
    MtxF* boneMatrix;
    MtxF resultMatrix;
    SRT galleonTransform;
    SRT boneTransform;
    void** dll;
    ObjDef* objDef;

    if (!visibility ||  main_get_bits(BIT_WM_Played_Randorn_First_Meeting)) {
        return;
    }

    draw_object(self, gfx, mtxs, vtxs, pols, 1.0f);

    objData = self->data;
    if (objData != NULL) {
        if (objData->kyte != NULL) {
            kyte = objData->kyte;
            if (kyte->unk0xb0 & 0x40) {
                objData->kyte = NULL;
            }
            objDef = self->def;
            model = self->modelInsts[self->modelInstIdx];
            
            /* Using the end of the cage's joint chain to update Kyte's 
             * transformation in sync with the swinging cage animation */
            if (objDef->numAttachPoints != 0) {
                if (model->unk_0x34 & 8) { 
                    boneIdx = objDef->pAttachPoints[1].bones[self->modelInstIdx];
                    boneMatrix = (MtxF *) &(((f32 **)model->matrices)[(model->unk_0x34 & 1)][boneIdx << 4]);

                    boneTransform.transl.x = 200.0f;
                    boneTransform.transl.y = 0.0f;
                    boneTransform.transl.z = -200.0f;
                    boneTransform.scale = 0.8f;
                    boneTransform.yaw = kyte->srt.yaw - self->srt.yaw;
                    boneTransform.pitch = 0;
                    boneTransform.roll = 0;
                    matrix_from_srt(&resultMatrix, &boneTransform);
                    matrix_concat_4x3(&resultMatrix, boneMatrix, &resultMatrix);
                    func_80034FF0((s32)&resultMatrix);
                    draw_object(kyte, gfx, mtxs, vtxs, pols, 1.0f);
                    func_80034FF0(NULL);
                }
                kyte->unk0xdc = 2;
            }

            //Create a lightning strike from above (at a random angle)
            if (objData->createLightningU == 1) {
                galleonTransform.roll = 0;
                galleonTransform.pitch = 0;
                galleonTransform.yaw = 0;
                galleonTransform.transl.x = 0.0f;
                galleonTransform.transl.y = 0.0f;
                galleonTransform.transl.z = 0.0f;
                galleonTransform.scale = 1.0f;

                boneIdx = self->def->pAttachPoints->bones[0];
                boneMatrix = (MtxF *) &(((f32 **)model->matrices)[(model->unk_0x34 & 1)][boneIdx << 4]);
                boneTransform.yaw = 0;
                boneTransform.roll = 0;
                boneTransform.pitch = 0;
                boneTransform.scale = 1.0f;
                boneTransform.transl.x = self->def->pAttachPoints->pos.x;
                boneTransform.transl.y = self->def->pAttachPoints->pos.y;
                boneTransform.transl.z = self->def->pAttachPoints->pos.z;
                vec3_transform(boneMatrix, boneTransform.transl.x, boneTransform.transl.y, boneTransform.transl.z, &boneTransform.transl.x, &boneTransform.transl.y, &boneTransform.transl.z);
                if (self->parent) {
                    galleonTransform.yaw = self->parent->srt.yaw;
                    rotate_vec3(&galleonTransform, boneTransform.transl.f);
                    boneTransform.transl.x += self->parent->srt.transl.x;
                    boneTransform.transl.y += self->parent->srt.transl.y;
                    boneTransform.transl.z += self->parent->srt.transl.z;
                } else {
                    boneTransform.transl.x += gWorldX;
                    boneTransform.transl.z += gWorldZ;
                }
                objData->createLightning = 0;
                dll = dll_load_deferred(0x200D, 1);
                ((DLL_Unknown*)dll)->vtbl->func[0].withSevenArgs((s32)self, 0, (s32)&boneTransform, 1, -1, 0xD, 0);
                dll_unload(dll);

            //Create a lightning trail across deck (at a random angle)
            } else if (objData->createLightningU == 2) {
                galleonTransform.roll = 0;
                galleonTransform.pitch = 0;
                galleonTransform.yaw = 0;
                galleonTransform.transl.x = 0.0f;
                galleonTransform.transl.y = 0.0f;
                galleonTransform.transl.z = 0.0f;
                galleonTransform.scale = 1.0f;

                boneIdx = self->def->pAttachPoints->bones[0];
                boneMatrix = (MtxF *) &(((f32 **)model->matrices)[(model->unk_0x34 & 1)][boneIdx << 4]);
                boneTransform.yaw = 0;
                boneTransform.roll = 0;
                boneTransform.pitch = 0;
                boneTransform.scale = 1.0f;
                boneTransform.transl.x = self->def->pAttachPoints->pos.x;
                boneTransform.transl.y = self->def->pAttachPoints->pos.y;
                boneTransform.transl.z = self->def->pAttachPoints->pos.z;

                vec3_transform(boneMatrix, boneTransform.transl.x, boneTransform.transl.y, boneTransform.transl.z, &boneTransform.transl.x, &boneTransform.transl.y, &boneTransform.transl.z);
                if (self->parent) {
                    galleonTransform.yaw = self->parent->srt.yaw;
                    rotate_vec3(&galleonTransform, boneTransform.transl.f);
                    boneTransform.transl.x += self->parent->srt.transl.x;
                    boneTransform.transl.y += self->parent->srt.transl.y;
                    boneTransform.transl.z += self->parent->srt.transl.z;
                } else {
                    boneTransform.transl.x += gWorldX;
                    boneTransform.transl.z += gWorldZ;
                }
                objData->createLightning = 0;
                dll = dll_load_deferred(0x200F, 1);
                ((DLL_Unknown*)dll)->vtbl->func[0].withSevenArgs((s32)self, 0, (s32)&boneTransform, 1, -1, 0xF, 0);
                dll_unload(dll);
            }
        }
    }
}

// offset: 0x848 | func: 4 | export: 4
void kyteCage_free(Object *self, s32 a1) { }

// offset: 0x858 | func: 5 | export: 5
u32 kyteCage_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x868 | func: 6 | export: 6
u32 kyteCage_get_data_size(Object *self, u32 a1) {
    return sizeof(KyteCage_Data);
}

// offset: 0x87C | func: 7
s32 kyteCage_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    s32 index;
    KyteCage_Data *data;
    
    data = self->data;
    
    for (index = 0; index < animObjData->unk98; index++){
        if ((u32)animObjData->unk8E[index] == 1){
            data->createLightning = 1;
        } else if ((u32)animObjData->unk8E[index] == 2){
            data->createLightning = 2;
        }
    }
    
    animObjData->unk7A = -4;
    animObjData->unk62 = 0;
    return 0;
}
