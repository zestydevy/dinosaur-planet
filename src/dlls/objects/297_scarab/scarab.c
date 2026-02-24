#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objmsg.h"
#include "sys/rand.h"
#include "types.h"

typedef struct {
/*00*/ u32 soundHandle;
/*04*/ f32 speedX;
/*08*/ f32 speedZ;
/*0C*/ f32 unkC;
/*10*/ f32 unk10; //y
/*14*/ s16 unk14;
/*16*/ s8 unk16[0x18 - 0x16];
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ s8 unk1C[0x20 - 0x1C];
/*20*/ s16 unk20;
/*22*/ s16 unk22;
/*24*/ s16 unk24;
/*26*/ u8 unk26;
/*27*/ s8 unk27;
/*27*/ s8 unk28;
/*29*/ u8 scarabTypeIndex;
} Scarab_Data;

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

/*0x0*/ static u16 _data_0[] = {
    0x0451, 0x0452, 0x0453, 0x0453
};
/*0x8*/ static u32 _data_8[] = {
    0, 0, 0
};
/*0x14*/ static u32 _data_14[] = {
    0, 0, 0
};

static Vec4f _bss_0;

// offset: 0x0 | ctor
void dll_297_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_297_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_297_setup(Object* self, Scarab_Setup* setup, s32 arg2) {
    Scarab_Data* objdata = self->data;

    objdata->unk26 = 0;
    objdata->unk18 = setup->unk1A;
    objdata->unk1A = rand_next(1000, 4000);
    objdata->unk20 = rand_next(50, 100);
    objdata->unk10 = setup->base.y;

    self->unkB0 |= 0x2000;

    switch (self->id) {
        case OBJ_Green_scarab:
            objdata->unk22 = 0xB6E;
            objdata->unk24 = 0x1FF;
            objdata->scarabTypeIndex = 0;
            break;
        case OBJ_Red_scarab:
            objdata->unk22 = 0xB6F;
            objdata->unk24 = 0x79;
            objdata->scarabTypeIndex = 1;
            break;
        case OBJ_Gold_scarab:
            objdata->unk22 = 0xB70;
            objdata->unk24 = 0x1FE;
            objdata->scarabTypeIndex = 2;
            break;
        default:
        case OBJ_Rain_scarab:
            objdata->unk22 = 0xB71;
            objdata->unk24 = 0x7B;
            objdata->scarabTypeIndex = 3;
            break;
    }

    obj_init_mesg_queue(self, 2);
}

// offset: 0x174 | func: 1 | export: 1
void dll_297_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/297_scarab/dll_297_control.s")

// offset: 0x1164 | func: 2 | export: 2
void dll_297_update(Object *self) { }

// offset: 0x1170 | func: 3 | export: 3
void dll_297_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    Scarab_Data* objdata = self->data;

    if (objdata->unk14) {
        return;
    }

    if (self->unkE0) {
        if (visibility != -1) {
            return;
        }
    } else if (!visibility){
        return;
    }

    draw_object(self, gfx, mtx, vtx, pols, 1.0f);
}

// offset: 0x11F8 | func: 4 | export: 4
void dll_297_free(Object* self, s32 arg1) {
    Scarab_Data* objdata;

    objdata = self->data;
    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
}

// offset: 0x1260 | func: 5 | export: 5
u32 dll_297_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1270 | func: 6 | export: 6
u32 dll_297_get_data_size(Object *self, u32 a1) {
    return sizeof(Scarab_Data);
}

// offset: 0x1284 | func: 7
void dll_297_func_1284(Object* self, Object* player, Scarab_Data* objdata) {
  u8 values[4] = {1, 5, 10, 50};

  ((DLL_Unknown*)player->dll)->vtbl->func[19].withTwoArgs((s32) player, values[objdata->scarabTypeIndex]);

  objdata->unk14 = 0x50;
  objdata->unk18 = 0;
}

// offset: 0x1304 | func: 8
void dll_297_func_1304(Object* self, ScarabFunc1304Arg1* arg1, u8 arg2, ScarabFunc1304Arg3* arg3) {
    SRT transform;
    f32 speed;
    Vec3f v;
    u32 new_var;
    f32 doubleSpeed;
    Scarab_Data *objdata;

    objdata = self->data;

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
        objdata->speedX = self->speed.x;
        objdata->speedZ = self->speed.z;
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
    rotate_vec3(&transform, v.f);

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
s32 dll_297_func_1544(Object* self) {
    Vec3f spF0[4];
    Vec3f spC0[4];
    Unk80027934 sp54;
    AABBs32 sp3C;
    ObjectHitInfo* objHits;
    u8 i;
    f32* temp;

    objHits = self->objhitInfo;
    if (objHits != NULL) {
        spF0[0].f[0] = self->srt.transl.x;
        spF0[0].f[1] = self->srt.transl.y;
        spF0[0].f[2] = self->srt.transl.z;
        spC0[0].f[0] = self->positionMirror2.x;
        spC0[0].f[1] = self->positionMirror2.y;
        spC0[0].f[2] = self->positionMirror2.z;
        sp54.unk40[0] = objHits->unk52;
        sp54.unk50[0] = -1;
        sp54.unk54[0] = 3;
    } else {
        return 0;
    }
        
    fit_aabb_around_cubes(&sp3C, spC0, spF0, sp54.unk40, 1);
    func_80053750(self, &sp3C, objHits->unkA1);
    i = func_8005509C(self, (f32*)&spC0, (f32*)&spF0, 1, &sp54, 0);
    if (i) {
        if (i & 1) {
            i = 0;
        } else if (i & 2) {
            i = 1;
        } else if (i & 4) {
            i = 2;
        } else {
            i = 3;
        }

        objHits->unk9C = sp54.unk50[i];
        temp = (f32*)spF0;
        objHits->unk34 = temp[i * 3 + 0];
        objHits->unk38 = temp[i * 3 + 1];
        objHits->unk3C = temp[i * 3 + 2];

        _bss_0.f[0] = sp54.unk0[i].f[0];
        _bss_0.f[1] = sp54.unk0[i].f[1];
        _bss_0.f[2] = sp54.unk0[i].f[2];
        _bss_0.f[3] = sp54.unk0[i].f[3];
        
        if (sp54.unk58[i] != 0) {
            objHits->unk9D |= 2;
            self->srt.transl.f[0] = objHits->unk34;
            self->srt.transl.f[1] = objHits->unk38;
            self->srt.transl.f[2] = objHits->unk3C;
            objHits->unk10.f[0] = self->positionMirror2.f[0];
            objHits->unk10.f[1] = self->positionMirror2.f[1];
            objHits->unk10.f[2] = self->positionMirror2.f[2];
            return 1;
        } else {
            objHits->unk9D |= 1;
            self->srt.transl.f[0] = objHits->unk34;
            self->srt.transl.f[1] = objHits->unk38;
            self->srt.transl.f[2] = objHits->unk3C;
            objHits->unk10.f[0] = self->positionMirror2.f[0];
            objHits->unk10.f[1] = self->positionMirror2.f[1];
            objHits->unk10.f[2] = self->positionMirror2.f[2];
            return 1;
        }
    } else {
        return 0;
    }
}

/*0x0*/ static const char str_0[] = "fy %f %f\n";
/*0xC*/ static const char str_C[] = "Crashed\n";
