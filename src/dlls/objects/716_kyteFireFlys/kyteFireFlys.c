#include "common.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    u8 _unk0[0x34-0x00];
    u8 unk34;
    u8 _unk35;
    u8 unk36;
    u8 _unk37;
    CurveSetup* unk38;
    //[0x3C];
} DLL716_Data;
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u16 unk1A;
    s16 unk1C;
}DLL716_Setup;

// offset: 0x0 | ctor
void dll_716_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_716_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_716_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/716_kyteFireFlys/dll_716_setup.s")

// offset: 0xE0 | func: 1 | export: 1
void dll_716_control(Object* self) {
    DLL716_Data* objdata;
    DLL716_Setup* setup;
    Object* sidekick;
    CurveSetup* temp_v0_2;

    objdata = self->data;
    setup = (DLL716_Setup*)self->setup;
    if (objdata->unk36 == 0) {
        temp_v0_2 = gDLL_25->vtbl->func_2A50(self, (s32) setup->unk1A);
        objdata->unk38 = temp_v0_2;
        if (temp_v0_2 != NULL) {
            objdata->unk36 = 1U;
            self->srt.transl.x = objdata->unk38->pos.x;
            self->srt.transl.y = objdata->unk38->pos.y;
            self->srt.transl.z = objdata->unk38->pos.z;
        }
    } else if (objdata->unk34 != 0) {
        sidekick = get_sidekick();
        if (sidekick != NULL) {
            if (vec3_distance_squared(&get_player()-> positionMirror, &objdata->unk38->pos) <= SQ(setup->unk18)) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, 1);
                if (gDLL_1_UI->vtbl->func_DF4(1) != 0) {
                    main_set_bits(0x46E, (u32) setup->unk1A);
                }
            }
        }
    }
}

// offset: 0x26C | func: 2 | export: 2
void dll_716_update(Object *self) { }

// offset: 0x278 | func: 3 | export: 3
void dll_716_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x290 | func: 4 | export: 4
void dll_716_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/716_kyteFireFlys/dll_716_free.s")

// offset: 0x330 | func: 5 | export: 5
u32 dll_716_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x340 | func: 6 | export: 6
u32 dll_716_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL716_Data);
}

// offset: 0x354 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/716_kyteFireFlys/dll_716_func_354.s")

// offset: 0x49C | func: 8 | export: 8
s32 dll_716_func_49C(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
} //bs'd the s32s here

// offset: 0x4B4 | func: 9 | export: 9
s32 dll_716_func_4B4(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
} //bs'd the s32s here

// offset: 0x4CC | func: 10 | export: 10
s32 dll_716_func_4CC(s32 arg0) {
    return 8;
}

// offset: 0x4DC | func: 11
void dll_716_func_4DC(Object* self, s32 arg1, s32 arg2) {
    ObjSetup* objSetup;
    DLL716_Setup* setup;

    objSetup = self->setup;
    setup = obj_alloc_create_info(0x24, 0x259);
    setup->base.fadeDistance = objSetup->fadeDistance;
    setup->base.loadFlags = 2;
    setup->base.byte5 = objSetup->byte5;
    setup->base.x = rand_next(-arg2, arg2) + self->srt.transl.x;
    setup->base.y = self->srt.transl.y;
    setup->base.z = rand_next(-arg2, arg2) + self->srt.transl.z;
    setup->unk19 = 3;
    setup->unk1A = arg1;
    setup->unk1C = arg2;
    setup->unk18 = arg2;
    obj_create(&setup->base, 5U, -1, -1, NULL);
}

/*0x0*/ static const char str_0[] = "should activate the command\n";
