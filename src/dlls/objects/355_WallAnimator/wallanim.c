#include "common.h"
#include "dlls/objects/common/sidekick.h"
#include "sys/map.h"
#include "sys/memory.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 gamebitA;
    s16 _unk1A;
    s16 unk1C;
    u8 _unk1E;
    u8 _unk1F;
    u8 unk20;
    u8 unk21;
    s8 _unk22;
    u8 unk23;
    s16 yaw;        //NOTE: 16-bit precision, instead of setups' usual 8-bit yaw.
    u8 unk26;
} DLL355_Setup;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32* unk10;
    Vec3f unk14;
    s32 unk20;
    f32 unk24;
    f32 unk28;
    s8 unk2C;
    u8 animatorID; //animatorID
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    s8 unk31;
    s16 gamebitDug;
} DLL355_Data;

#define OPACITY_MAX 0xFF

static void dll_355_func_7F4(Object* self, DLL355_Data* objData);

// offset: 0x0 | ctor
void dll_355_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_355_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_355_setup(Object* self, DLL355_Setup* objSetup, s32 reset) {
    s32 pad[2];
    s32 animatedVtxCount; //2C
    DLL355_Data* objData;

    objData = self->data;
    
    self->srt.yaw = objSetup->yaw;
    
    objData->animatorID = objSetup->unk23;
    objData->unk2E = objSetup->unk1C;
    objData->unk20 = 0;
    objData->unk2C = -1;

    objData->unk24 = objSetup->unk21;
    objData->gamebitDug = objSetup->gamebitA;
    objData->unk28 = objSetup->unk26;
    

    animatedVtxCount = block_get_animator_vertex_count(self, objData->animatorID);
    if (animatedVtxCount == 0) {
        objData->animatorID = 0;
    }

    if (objData->animatorID != 0) {
        objData->unk0 = fsin16_precise(self->srt.yaw);
        objData->unk4 = 0.0f;
        objData->unk8 = fcos16_precise(self->srt.yaw);
        objData->unkC = 0.0f;
        
        if (reset == 0) {
            objData->unk10 = mmAlloc(animatedVtxCount * sizeof(f32), ALLOC_TAG_TRACK_COL, NULL);
            dll_355_func_7F4(self, objData);
        }
        
        if (main_get_bits(objSetup->gamebitA)) {
            objData->unk30 = 1;
            objData->unk20 = 3000;
        }
        
        obj_add_object_type(self, OBJTYPE_TrickyTarget);
        obj_add_object_type(self, OBJTYPE_WallAnimator);
    }
}

// offset: 0x1E8 | func: 1 | export: 1
void dll_355_control(Object* self) {
    DLL355_Data* objData;
    Block* block;
    DLL355_Setup* objSetup;
    BlockVertex *vertices;
    BlockShape* shapes;
    s32 shapeIdx;
    s32 animVtxIdx;
    Object* pushblock;
    f32 displacement;
    f32 distance;
    u8 fadeOut;
    Object* player;
    Object* sidekick;
    s32 vertexIdx;

    objData = self->data;
    
    if (objData->animatorID == 0) {
        return;
    }
    
    objSetup = (DLL355_Setup*)self->setup;
    if (objData->unk20 != 3000) {
        player = get_player();
        sidekick = get_sidekick();
        distance = M_INFINITY_F;
        pushblock = obj_get_nearest_type_to(OBJTYPE_PushBlock, self, &distance);
        if (sidekick && ((pushblock == NULL) || (pushblock && (distance > 35.0f)))) {
            distance = vec3_distance_squared(&self->globalPosition, &player->globalPosition);
            if (distance <= SQ(objSetup->unk20)) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, Sidekick_Command_INDEX_1_Find);
            }
        }
    }
    
    if (objData->unk20 == 0) {
        return;
    }
    
    //Get local Blocks model and make sure its vertices are animatable
    block = map_get_block_by_index(map_world_coords_to_block_index(
        self->srt.transl.x, self->srt.transl.y, self->srt.transl.z)
    );
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return;
    }
    
    if (objData->unk20 != objData->unk2C) {
        objData->unk2F = 2;
        objData->unk2C = objData->unk20;
    }
    
    if (objData->unk2F == 0) {
        return;
    }
    objData->unk2F--;
    
    if ((objData->unk20 >= 3000) && (objData->unk30 == 0)) {
        objData->unk30 = 1;
        objData->unk20 = 3000;
        main_set_bits(objData->gamebitDug, TRUE);
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B01_Success_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
    }

    vertices = (BlockVertex*)block->vertices2[block->vtxFlags & 1]; 
    shapeIdx = 0;
    animVtxIdx = 0;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++){
        if (objData->animatorID == shapes[shapeIdx].animatorID) {

            for (vertexIdx = shapes[shapeIdx].vtxBase; vertexIdx < shapes[shapeIdx + 1].vtxBase; vertexIdx++, animVtxIdx++) {
                
                if (objData->unk10[animVtxIdx] > 0.0f) {
                    displacement = (objData->unk20 / 100.0f);
                    
                    //Extrude vertices
                    if ((shapes[shapeIdx].flags & RENDER_UNK20000000)) {
                        vertices[vertexIdx].ob[0] = block->vertices[vertexIdx].ob[0] + displacement*objData->unk0;
                        vertices[vertexIdx].ob[2] = block->vertices[vertexIdx].ob[2] + displacement*objData->unk8;
                    } else {
                        vertices[vertexIdx].ob[0] = block->vertices[vertexIdx].ob[0] + displacement*objData->unk0;
                        vertices[vertexIdx].ob[2] = block->vertices[vertexIdx].ob[2] + displacement*objData->unk8;
                    }
                    
                    //Animate vertex colours
                    displacement = displacement / 30;
                    fadeOut = displacement * 255;
                    vertices[vertexIdx].cn[0] = (block->vertices[vertexIdx].cn[0] * (0xFF - fadeOut)) >> 8;
                    vertices[vertexIdx].cn[1] = (block->vertices[vertexIdx].cn[1] * (0xFF - fadeOut)) >> 8;
                    vertices[vertexIdx].cn[2] = (block->vertices[vertexIdx].cn[2] * (0xFF - fadeOut)) >> 8;
                    vertices[vertexIdx].cn[3] = 0xFF - fadeOut;
                }
            }
        }
    }
}

// offset: 0x6E8 | func: 2 | export: 2
void dll_355_update(Object *self) { }

// offset: 0x6F4 | func: 3 | export: 3
void dll_355_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x748 | func: 4 | export: 4
void dll_355_free(Object* self, s32 onlySelf) {
    DLL355_Data* objData = self->data;

    if (objData->unk10 != NULL) {
        mmFree(objData->unk10);
    }
    
    obj_free_object_type(self, OBJTYPE_TrickyTarget);
    obj_free_object_type(self, OBJTYPE_WallAnimator);
}

// offset: 0x7D0 | func: 5 | export: 5
u32 dll_355_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x7E0 | func: 6 | export: 6
u32 dll_355_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL355_Data);
}

// offset: 0x7F4 | func: 7
void dll_355_func_7F4(Object* self, DLL355_Data* objData) {
    BlockShape* shapes;
    f32 distanceSquared;
    f32 dy;
    f32 objBlockX;
    f32 objBlockZ;
    f32 dx;
    f32 dz;
    f32 tValue;
    f32 minDistance;
    s32 closestVtxBase;
    s32 shapeIdx;
    s32 blockWorldGridX;
    s32 blockWorldGridZ;
    s32 vtxID;
    Block* block;
    s32 animVtxIdx;
    f32 radiusSq;

    block = map_get_block_by_index(map_world_coords_to_block_index(
        self->srt.transl.x, self->srt.transl.y, self->srt.transl.z)
    );
    
    if ((block == NULL) || !(block->vtxFlags & 8)) {
        return;
    }
    
    blockWorldGridX = floor_f((self->srt.transl.x - gWorldX) / BLOCKS_GRID_UNIT_F);
    blockWorldGridZ = floor_f((self->srt.transl.z - gWorldZ) / BLOCKS_GRID_UNIT_F);
    
    objBlockX = self->srt.transl.x - (blockWorldGridX * BLOCKS_GRID_UNIT_F + gWorldX);
    objBlockZ = self->srt.transl.z - (blockWorldGridZ * BLOCKS_GRID_UNIT_F + gWorldZ);
    
    minDistance = 10000.0f;
    shapeIdx = 0;
    closestVtxBase = -1;
    shapes = block->shapes;
    for (; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            for (vtxID = shapes[shapeIdx].vtxBase; vtxID < shapes[shapeIdx + 1].vtxBase; vtxID++) {
                dx = block->vertices[vtxID].ob[0];
                dz = block->vertices[vtxID].ob[2];
                dx -= objBlockX;
                dz -= objBlockZ;
                
                distanceSquared = SQ(dx) + SQ(dz);
                if (distanceSquared < minDistance) {
                    minDistance = distanceSquared;
                    closestVtxBase = vtxID;
                }
            }
        }
    }

    tValue = 1.0f; //fake?
    
    objData->unk14.f[0] = block->vertices[closestVtxBase].ob[0];
    objData->unk14.f[1] = block->vertices[closestVtxBase].ob[1];
    objData->unk14.f[2] = block->vertices[closestVtxBase].ob[2];

    shapes = block->shapes;
    for (shapeIdx = 0, animVtxIdx = 0; shapeIdx < block->shapeCount; shapeIdx++) {
        if (objData->animatorID == shapes[shapeIdx].animatorID) {
            for (vtxID = shapes[shapeIdx].vtxBase; vtxID < shapes[shapeIdx + 1].vtxBase; vtxID++, animVtxIdx++) {
                dx = block->vertices[vtxID].ob[0];
                dy = block->vertices[vtxID].ob[1];
                dz = block->vertices[vtxID].ob[2];
                
                dx -= objData->unk14.x;
                dy -= objData->unk14.y;
                dz -= objData->unk14.z;

                tValue = (SQ(dx) + SQ(dy) + SQ(dz));
                radiusSq = SQ(objData->unk24);
                tValue /= radiusSq;
                if (tValue > 1.0f) {
                    tValue = 1.0f;
                }

                tValue = SQ(tValue);
                tValue = 1.0f - tValue;
                objData->unk10[animVtxIdx] = tValue;
            }
        }
    }
}

// offset: 0xB54 | func: 8 | export: 7
f32 dll_355_func_B54(Object* self, Object* arg1) {
    DLL355_Data* objData;
    f32 dy;
    f32 dx;
    f32 dz;

    objData = self->data;
    
    dy = arg1->srt.transl.y - self->srt.transl.y;
    if ((dy < -20.0f) || (dy > 20.0f)) {
        return 0.0f;
    }
    
    dx = arg1->srt.transl.x - self->srt.transl.x;
    dz = arg1->srt.transl.z - self->srt.transl.z;
    if ((SQ(dx) + SQ(dz)) > 2500.0f) {
        return 0.0f;
    }
    
    objData->unk20 += 60;
    return (f32) objData->unk20 / 3000.0f;
}

// offset: 0xC20 | func: 9 | export: 8
u8 dll_355_func_C20(Object* self) {
    DLL355_Data* objData = self->data;
    return (objData->unk20 >= 3000);
}

// offset: 0xC3C | func: 10 | export: 9
u8 dll_355_func_C3C(Object* self) {
    DLL355_Data* objData = self->data;
    return objData->unk2E;
}
