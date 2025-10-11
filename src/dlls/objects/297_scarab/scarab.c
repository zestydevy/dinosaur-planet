#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "dlls/objects/210_player.h"
#include "sys/objmsg.h"
#include "sys/rand.h"

typedef struct {
u32 soundHandle;
f32 unk4; //speedX
f32 unk8; //speedZ
f32 unkC;
f32 unk10; //y
s16 unk14;
s8 unk16[0x18 - 0x16];
s16 unk18;
s16 unk1A;
s8 unk1C[0x20 - 0x1C];
s16 unk20;
s16 unk22;
s16 unk24;
u8 unk26;
s8 unk27[0x29 - 0x27];
u8 unk29; //scarabTypeIndex
} ScarabState;

typedef struct {
ObjSetup base;
s16 unk18;
s16 unk1A;
} Scarab_Setup;

typedef struct {
f32 unk0;
f32 unk4;
f32 unk8;
f32 unkC;
} ScarabFunc1304Arg1;

typedef struct {
f32 unk0;
f32 unk4;
f32 unk8;
f32 unkC;
} ScarabFunc1304Arg3;

typedef union {
    u8 bytes[4];
    u32 word;
} ScarabValues;

/*0x0*/ static u16 _data_0[] = {
    0x0451, 0x0452, 0x0453, 0x0453
};
/*0x8*/ static u32 _data_8[] = {
    0, 0, 0
};
/*0x14*/ static u32 _data_14[] = {
    0, 0, 0
};
/*0x20*/ static ScarabValues _data_20 = {
    {1, 5, 10, 50}
};

/*0x0*/ static u8 _bss_0[0x10];

// offset: 0x0 | ctor
void dll_297_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_297_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_297_create(Object* self, Scarab_Setup* setup, s32 arg2) {
    ScarabState* state = self->state;

    state->unk26 = 0;
    state->unk18 = setup->unk1A;
    state->unk1A = rand_next(1000, 4000);
    state->unk20 = rand_next(50, 100);
    state->unk10 = setup->base.y;

    self->unk0xb0 |= 0x2000;

    switch (self->id) {
        case OBJ_Green_scarab:
            state->unk22 = 0xB6E;
            state->unk24 = 0x1FF;
            state->unk29 = 0;
            break;
        case OBJ_Red_scarab:
            state->unk22 = 0xB6F;
            state->unk24 = 0x79;
            state->unk29 = 1;
            break;
        case OBJ_Gold_scarab:
            state->unk22 = 0xB70;
            state->unk24 = 0x1FE;
            state->unk29 = 2;
            break;
        default:
        case OBJ_Rain_scarab:
            state->unk22 = 0xB71;
            state->unk24 = 0x7B;
            state->unk29 = 3;
            break;
    }

    obj_init_mesg_queue(self, 2);
}

// offset: 0x174 | func: 1 | export: 1
void dll_297_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/297_scarab/dll_297_update.s")

// offset: 0x1164 | func: 2 | export: 2
void dll_297_func_1164(Object *self) { }

// offset: 0x1170 | func: 3 | export: 3
void dll_297_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    ScarabState* state = self->state;

    if (state->unk14) {
        return;
    }

    if (self->unk_0xe0) {
        if (visibility != -1) {
            return;
        }
    } else if (!visibility){
        return;
    }

    draw_object(self, gfx, mtx, vtx, pols, 1.0f);
}

// offset: 0x11F8 | func: 4 | export: 4
void dll_297_destroy(Object* self, s32 arg1) {
    ScarabState* state;

    state = self->state;
    if (state->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandle);
        state->soundHandle = 0;
    }
}

// offset: 0x1260 | func: 5 | export: 5
u32 dll_297_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1270 | func: 6 | export: 6
u32 dll_297_get_state_size(Object *self, u32 a1) {
    return sizeof(ScarabState);
}

// offset: 0x1284 | func: 7
void dll_297_func_1284(Object* self, Object* player, ScarabState* state) {
  ScarabValues values = _data_20;

  ((DLL_Unknown*)player->dll)->vtbl->func[19].withTwoArgs((s32) player, values.bytes[state->unk29]);

  state->unk14 = 0x50;
  state->unk18 = 0;
}

// offset: 0x1304 | func: 8
void dll_297_func_1304(Object* self, ScarabFunc1304Arg1* arg1, u8 arg2, ScarabFunc1304Arg3* arg3) {
    SRT transform;
    f32 speed;
    Vec3f v;
    u32 new_var;
    f32 doubleSpeed;
    ScarabState *state;

    state = self->state;

    if (arg2 == 1){
        v.x = arg1->unk4;
        v.y = arg1->unk8;
        v.z = arg1->unkC;
    }

    else if (arg2 == 0){
        v.x = arg3->unk0;
        v.y = arg3->unk4;
        v.z = arg3->unk8;
    }

    else if (arg2 == 2){
        self->speed.x = arg3->unk0;
        self->speed.z = arg3->unk8;

        speed = (self->speed.x * self->speed.x) + (self->speed.z * self->speed.z);
        if (speed){
            speed = sqrtf(speed);
        }
        doubleSpeed = 2.0f * speed;

        self->speed.x /= doubleSpeed;
        self->speed.z /= doubleSpeed;
        state->unk4 = self->speed.x;
        state->unk8 = self->speed.z;
        self->srt.yaw = arctan2_f(-arg3->unk0, -arg3->unk8);
        return;
    }

    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.scale = 1.0f;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = self->srt.yaw;
    rotate_vec3(&transform, &v);

    if (arg1){
        new_var = arctan2_f(v.x, v.y);
        self->srt.pitch = arctan2_f(v.z, v.y);
        self->srt.roll = new_var;
        return;
    }

    self->srt.roll = 0;
    self->srt.pitch = arctan2_f(arg3->unk8 + arg3->unk0, arg3->unk4);
    if (self->srt.pitch < 0){
        self->srt.pitch = -self->srt.pitch;
    }
    self->srt.yaw = arctan2_f(arg3->unk0, arg3->unk8);
    return;
}

// offset: 0x1544 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/297_scarab/dll_297_func_1544.s")

/*0x0*/ static const char str_0[] = "fy %f %f\n";
/*0xC*/ static const char str_C[] = "Crashed\n";
