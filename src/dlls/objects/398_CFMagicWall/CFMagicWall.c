#include "sys/camera.h"
#include "sys/gfx/map.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "functions.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
} CFMagicWall_Setup;

static void CFMagicWall_func_384(Object* self, u8 opacity, s32 animatorID);

// offset: 0x0 | ctor
void CFMagicWall_ctor(void *dll) { }

// offset: 0xC | dtor
void CFMagicWall_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFMagicWall_create(Object *self, ObjSetup *setup, s32 arg2) {
    obj_add_object_type(self, OBJTYPE_32);
}

// offset: 0x5C | func: 1 | export: 1
void CFMagicWall_update(Object* self) {
    CFMagicWall_Setup* setup;
    Object* player;
    u8 var_a1;
    f32 var_fv0;
    f32 var_ft1;
    s32 pad[2];
    f32 temp;

    setup = (CFMagicWall_Setup*)self->setup;
    player = get_player();
    var_ft1 = (f32)setup->unk1A;
    temp = vec3_distance(&self->positionMirror, &player->positionMirror);
    if (temp < vec3_distance(&self->positionMirror, &get_sidekick()->positionMirror)) {
        var_fv0 = vec3_distance(&self->positionMirror, &player->positionMirror);
    } else {
        var_fv0 = vec3_distance(&self->positionMirror, &get_sidekick()->positionMirror);
    }
    if (func_80001884(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) < var_fv0) {
        var_fv0 = func_80001884(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    }
    if (var_fv0 < var_ft1) {
        var_a1 = ((var_fv0 / var_ft1) * 255.0f);
        if (setup->unk19 == 0) {
            var_a1 = (255 - var_a1);
        }
    } else {
        var_a1 = (setup->unk19 * 255);
    }
    CFMagicWall_func_384(self, var_a1, setup->unk18);
}

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
static void CFMagicWall_func_384(Object* self, u8 opacity, s32 animatorID) {
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
