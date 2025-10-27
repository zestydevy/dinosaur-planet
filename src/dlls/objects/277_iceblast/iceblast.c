#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/gfx/model.h"
#include "sys/objhits.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dlls/objects/210_player.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s8 unused18;
    s8 unk19; //collision-related
    s16 timer;
} IceBlast_Setup;

typedef struct {
    s16 timer;
    u8 _unk2[2];
} IceBlast_Data;

// offset: 0x0 | ctor
void iceblast_ctor(void* dll){
}

// offset: 0xC | dtor
void iceblast_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void iceblast_setup(Object* self, IceBlast_Setup* setup, s32 arg2) {
    IceBlast_Data* objdata = self->data;

    objdata->timer = setup->timer;
    func_80026128(self, 0x19, setup->unk19 ? 3 : 1, 0);
}

// offset: 0x80 | func: 1 | export: 1
void iceblast_control(Object* self) {
    Object* player;
    Object* weapon;
    IceBlast_Data* objdata;
    SRT transform;

    player = get_player();
    objdata = self->data;
    if (!player) {
        return;
    }

    weapon = player->linkedObject;
    if (!weapon) {
        return;
    }

    self->srt.roll = weapon->srt.roll;
    self->srt.pitch = weapon->srt.pitch;
    self->srt.yaw = weapon->srt.yaw;
    objdata->timer -= (s16)gUpdateRateF;
    if (objdata->timer <= 0) {
        objdata->timer = 30;
        self->speed.x = 0.0f; 
        self->speed.y = -5.0f;
        self->speed.z = 0.0f; 

        transform.transl.x = 0.0f;
        transform.transl.y = 0.0f;
        transform.transl.z = 0.0f;
        transform.scale = 1.0f;
        transform.roll = weapon->srt.roll;
        transform.pitch = weapon->srt.pitch;
        transform.yaw = weapon->srt.yaw;

        rotate_vec3(&transform, self->speed.f);
        self->srt.transl.x = weapon->positionMirror.x;
        self->srt.transl.y = weapon->positionMirror.y;
        self->srt.transl.z = weapon->positionMirror.z;
        self->srt.transl.x += self->speed.x * gUpdateRateF;
        self->srt.transl.y += self->speed.y * gUpdateRateF;
        self->srt.transl.z += self->speed.z * gUpdateRateF;
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, -1);
    }

    self->positionMirror2.x = self->srt.transl.x;
    self->positionMirror2.y = self->srt.transl.y;
    self->positionMirror2.z = self->srt.transl.z;
    self->srt.transl.x += self->speed.x * gUpdateRateF;
    self->srt.transl.y += self->speed.y * gUpdateRateF;
    self->srt.transl.z += self->speed.z * gUpdateRateF;

}

// offset: 0x26C | func: 2 | export: 2
void iceblast_update(void){
}

// offset: 0x274 | func: 3 | export: 3
void iceblast_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s32 visibility) {
    //@recomp: may want to hide these debug cubes
    draw_object(self, gfx, mtx, vtx, pols, 1.0f);
}

// offset: 0x2BC | func: 4 | export: 4
void iceblast_free(Object* self, s32 arg1){
}

// offset: 0x2CC | func: 5 | export: 5
u32 iceblast_get_model_flags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x2DC | func: 6 | export: 6
u32 iceblast_get_data_size(Object* self, s32 arg1){
    return sizeof(IceBlast_Data);
}
