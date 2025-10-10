#include "sys/camera.h"
#include "sys/gfx/map.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "functions.h"

typedef struct {
    ObjCreateInfo base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
} CFMagicWall_CreateInfo;

/*static*/ void CFMagicWall_func_384(Object* self, u8 opacity, s32 animatorID);

// offset: 0x0 | ctor
void CFMagicWall_ctor(void *dll) { }

// offset: 0xC | dtor
void CFMagicWall_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFMagicWall_create(Object *self, ObjCreateInfo *createInfo, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_32);
}

// offset: 0x5C | func: 1 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/398_CFMagicWall/CFMagicWall_update.s")
#else
// https://decomp.me/scratch/UYzEa
// stack allocation issue, matches otherwise
void CFMagicWall_update(Object* self) {
    CFMagicWall_CreateInfo* createInfo; // 44
    Object* player; // 40
    //f32 sp28;
    u8 var_a1;
    f32 var_fv0;
    f32 var_ft1; // 34
    //u8 pad[4];
    //f32 temp;

    createInfo = (CFMagicWall_CreateInfo*)self->createInfo;
    player = get_player();
    var_ft1 = (f32) createInfo->unk1A;
   // sp28 = vec3_distance(&self->positionMirror, &player->positionMirror);
    if (vec3_distance(&self->positionMirror, &player->positionMirror) < vec3_distance(&self->positionMirror, &get_sidekick()->positionMirror)) {
        var_fv0 = vec3_distance(&self->positionMirror, &player->positionMirror);
    } else {
        var_fv0 = vec3_distance(&self->positionMirror, &get_sidekick()->positionMirror);
    }
    //temp = func_80001884(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    if (func_80001884(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) < var_fv0) {
        var_fv0 = func_80001884(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    }
    if (var_fv0 < var_ft1) {
        var_a1 = ((var_fv0 / var_ft1) * 255.0f);
        if (createInfo->unk19 == 0) {
            var_a1 = (255 - var_a1);
        }
    } else {
        var_a1 = (createInfo->unk19 * 255);
    }
    CFMagicWall_func_384(self, var_a1, createInfo->unk18);
}
#endif

// offset: 0x2C0 | func: 2 | export: 2
void CFMagicWall_func_2C0(Object *self) { }

// offset: 0x2CC | func: 3 | export: 3
void CFMagicWall_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x320 | func: 4 | export: 4
void CFMagicWall_destroy(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_32);
}

// offset: 0x360 | func: 5 | export: 5
u32 CFMagicWall_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x370 | func: 6 | export: 6
u32 CFMagicWall_get_state_size(Object *self, u32 a1) {
    return 0;
}

// offset: 0x384 | func: 7
void CFMagicWall_func_384(Object* self, u8 opacity, s32 animatorID) {
    s32 blockIdx;
    Block* block;
    BlockShape* shapes;
    Vtx_t* vtxs;
    s32 vertexIdx;
    s32 shapeIdx;

    blockIdx = func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    block = func_80044BB0(blockIdx);
    if (block != NULL && (block->vtxFlags & 8)) {
        vtxs = block->vertices2[block->vtxFlags & 1];
        shapeIdx = 0;
        shapes = block->shapes;
        while (shapeIdx < block->shapeCount) {
            if (animatorID == shapes[shapeIdx].unk_0x14) {
                for (vertexIdx = shapes[shapeIdx].vtxBase; vertexIdx < shapes[shapeIdx + 1].vtxBase; vertexIdx++) {
                    vtxs[vertexIdx].cn[3] = opacity;
                }
            }
            shapeIdx++;
        }
    }
}
