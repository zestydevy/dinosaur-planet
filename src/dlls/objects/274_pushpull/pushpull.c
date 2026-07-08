#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
} DLL274_Setup;

typedef struct {
    u8 _unk0[0xC - 0x0];
    f32 unkC;
    u8 _unk10[0x78 - 0x10];
    u32 unk78;
    u32 unk7C;
    s16 unk80;
    u8 _unk82[0x8C - 0x82];
    Object* unk8C;
    u8 _unk90[0xC4 - 0x90];
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    u8 _unkD0[0xD4 - 0xD0];
    u16 unkD4;
    u8 _unkD6[0xDC - 0xD6];
} DLL274_Data;

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003
};
/*0x10*/ static u32 _data_10[] = {
    0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x30*/ static u32 _data_30[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x40*/ static u32 _data_40[] = {
    0x00000000, 0x00000000, 0x3dcccccd, 0x00000000
};

// offset: 0x0 | ctor
void dll_274_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_274_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_274_setup(Object *self, ObjSetup* setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_setup.s")

// offset: 0x638 | func: 1 | export: 1
void dll_274_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_control.s")

// offset: 0x87C | func: 2 | export: 2
void dll_274_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_update.s")

// offset: 0xEE4 | func: 3 | export: 3
void dll_274_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_print.s")

// offset: 0x1058 | func: 4 | export: 4
void dll_274_free(Object* self, s32 onlySelf) {
    DLL274_Data* objData;
    DLL274_Setup* objSetup;

    objSetup = (DLL274_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    case OBJ_NWSH_colpush:
        main_set_bits(objData->unk80, 0);
        break;
    case OBJ_NWSH_colpushped:
        main_set_bits(objData->unk80, 0);
        break;
    default:
        if (objSetup->unk18 >= 0) {
            map_save_object(&objSetup->base, self->mapID, objSetup->base.x, objSetup->base.y, objSetup->base.z);
        }
        break;
    }

    if (objData->unk78) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk78);
        objData->unk78 = 0;
    }
    
    obj_free_object_type(self, OBJTYPE_PushBlock);
}

// offset: 0x116C | func: 5 | export: 5
u32 dll_274_get_model_flags(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_get_model_flags.s")

// offset: 0x117C | func: 6 | export: 6
u32 dll_274_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL274_Data);
}

// offset: 0x1190 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1190.s")

// offset: 0x176C | func: 8 | export: 8
s32 dll_274_func_176C(Object* self, Object* obj) {
    Vec3f d;
    s32 pad2;
    DLL274_Data* objData;
    s32 pad1;

    objData = self->data;
    
    d.f[0] = obj->srt.transl.x - self->srt.transl.x;
    d.f[1] = obj->srt.transl.y - self->srt.transl.y;
    d.f[2] = obj->srt.transl.z - self->srt.transl.z;

    return (sqrtf(SQ(d.f[0]) + SQ(d.f[1]) + SQ(d.f[2])) < objData->unkC);
}

// offset: 0x1814 | func: 9 | export: 9
void dll_274_func_1814(Object* self, s32 bitIndex) {
    DLL274_Data* objData = self->data;
    objData->unk7C |= (1 << bitIndex);
}

// offset: 0x1834 | func: 10 | export: 10
s32 dll_274_func_1834(Object* self) {
    DLL274_Data* objData = self->data;
    return objData->unkD4 & 1;
}

// offset: 0x184C | func: 11
s32 dll_274_func_184C(Object* self, s32 arg1, f32 arg2, f32* arg3) {
    UnkFunc_80024108Struct sp24;
    s32 temp_v1;

    sp24.unk12 = 0;
    temp_v1 = func_80024108(self, arg2, 1.0f, &sp24);
    if (sp24.unk12 != 0) {
        *arg3 = sp24.unk0[2];
    } else {
        *arg3 = 0.0f;
    }
    
    return temp_v1;
}

// offset: 0x18C8 | func: 12
void dll_274_func_18C8(Object* self, s32 arg1) {
    DLL274_Data* objData;
    Object* outSender;
    u32 outMessageID;
    f32* outMesgArg;

    objData = self->data;
    outMesgArg = NULL;
    while (obj_recv_mesg(self, &outMessageID, &outSender, (void*)&outMesgArg)) {
        switch (outMessageID) {
        case 0xF0003:
            objData->unk8C = outSender;
            break;
        case 0xE:
            if ((self->id != OBJ_NWSH_colpush) && (self->id != OBJ_NWSH_colpushped)) {
                obj_destroy_object(self);
            }
            break;
        case 0x40001:
            if (self->id == OBJ_NWSH_colpush) {
                objData->unkC4 = *outMesgArg;
            }
            if (self->id == OBJ_NWSH_colpushped) {
                objData->unkC4 = *outMesgArg;
            }
            break;
        }
    }
}

// offset: 0x1A24 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1A24.s")

// offset: 0x1D18 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_1D18.s")

// offset: 0x2020 | func: 15
void dll_274_func_2020(Object* self, MtxF* oMtx) {
    SRT srt;

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    
    matrix_from_srt(oMtx, &srt);
}

// offset: 0x20A0 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_20A0.s")

// offset: 0x225C | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_225C.s")

// offset: 0x27C8 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/274_pushpull/dll_274_func_27C8.s")

// offset: 0x2A74 | func: 19
s32 dll_274_func_2A74(Object* arg0, DLL274_Data* arg1) {
    if ((arg1->unkCC == 0.0f) && (arg1->unkC8 > 0.0f)) {
        gDLL_6_AMSFX->vtbl->play(arg0, SOUND_3D8_Water_Splash, MAX_VOLUME, NULL, NULL, 0, NULL);
        main_set_bits(BIT_DIM_Pushed_Ice_Block_Into_Lake, 1);
    }
    return 0;
}

/*0x0*/ static const char str_0[] = "PUSHPULL OBJECT: hitpoint overflow\n";
/*0x24*/ static const char str_24[] = "handle is %d\n";
