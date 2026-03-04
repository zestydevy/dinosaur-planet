#include "common.h"

typedef struct {
    s32 _unk0;
    s16 _unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s8 unkC;
    u8 unkD;
    u8 unkE;
    u8 _unkF;
    s16 unk10;
    s16 _unk12;
    //[0x14];
} DLL220_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 _unk1C;
    s16 _unk1E;
    s16 _unk20;
    s16 unk22;
    s16 _unk24; 
    s8 _unk26;
    s8 unk27;
    s8 unk28;
    u8 unk29;
    s8 unk2A;
    u8 _unk2B;
    u8 _unk2C;
    u8 _unk2D;
    u8 _unk2E;
    u8 unk2F;
    s16 _unk30;
    u8 unk32;
}DLL220_Setup;

static void dll_220_func_8A4(Object* self, ObjSetup* setup);

/*0x0*/ static u32 _data_0[] = {
    0x00010102, 0x02020202, 0x02020202, 0x02010100, 0x00000491, 0x00000492, 0x00000493, 0x000004b1
};

// offset: 0x0 | ctor
void dll_220_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_220_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_220_setup(Object* self, DLL220_Setup* setup, s32 arg2) {
    DLL220_Data* objdata;

    objdata = self->data; 
    self->unkAF |= 8;
    objdata->unk10 =  setup->unk18;
    if (objdata->unk10 != -1) {
        if (main_get_bits( objdata->unk10) != 0) {
            dll_220_func_8A4(self, (ObjSetup* ) objdata);
            return;
        }
    }
    objdata->unk6 = (setup->unk29 * 8);
    objdata->unkA = setup->unk22;
    objdata->unkC = setup->unk32;
    objdata->unk8 = ((setup->unk28 & 0xFFFF) * 0xB6);
    objdata->unkD = setup->unk2F;
    objdata->unkE = setup->unk27;
    self->srt.yaw = setup->unk2A << 8;
}


// offset: 0x118 | func: 1 | export: 1
void dll_220_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/220_Chuka/dll_220_control.s")

// offset: 0x670 | func: 2 | export: 2
void dll_220_update(Object *self) { }

// offset: 0x67C | func: 3 | export: 3
void dll_220_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x6D0 | func: 4 | export: 4
void dll_220_free(Object *self, s32 a1) { }

// offset: 0x6E0 | func: 5 | export: 5
u32 dll_220_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6F0 | func: 6 | export: 6
u32 dll_220_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL220_Data);
}

// offset: 0x704 | func: 7 | export: 7
void dll_220_func_704(Object* self, u8 arg1) {
    
    if (arg1 == 0x80) {
        gDLL_6_AMSFX->vtbl->play_sound(self, 0x492U, 0x7FU, NULL, NULL, 0, NULL);
    }
}

// offset: 0x778 | func: 8
void dll_220_func_778(Object* self) {
    DLL220_Data* objdata;
    Object* sp2C;
    Object* temp_v0_2;
    ObjSetup* temp_v0;
    Object* player;

    objdata = self->data;
    temp_v0 = obj_alloc_create_info(0x24, 0x2CB);
    temp_v0->x = self->srt.transl.f[0];
    temp_v0->y = self->srt.transl.f[1];
    temp_v0->z = self->srt.transl.f[2];
    temp_v0->loadFlags = 2;
    temp_v0->byte5 = 4;
    temp_v0->fadeDistance = 0xFF;
    temp_v0_2 = obj_create(temp_v0, 5U, -1, -1, NULL);
    if (temp_v0_2 != NULL) {
        sp2C = temp_v0_2;
        player = get_player();
        sp2C->speed.f[0] = (player->srt.transl.f[0] - self->srt.transl.f[0]) / 42.0f;
        sp2C->speed.f[1] = ((player->srt.transl.f[1] + objdata->unkE) - self->srt.transl.f[1]) / 42.0f;
        sp2C->speed.f[2] = (player->srt.transl.f[2] - self->srt.transl.f[2]) / 42.0f;
        sp2C->unkC4 = self;
    }
}

// offset: 0x8A4 | func: 9
static void dll_220_func_8A4(Object* self, ObjSetup* setup) {
    func_800267A4(self);
    self->srt.flags |= 0x4000;
    setup->loadFlags = (setup->loadFlags | 2);
}

/*0x0*/ static const char str_0[] = "BADDIE:Chuka Unknown message [%d]\n";
/*0x24*/ static const char str_24[] = "";
/*0x28*/ static const char str_28[] = "";
/*0x2C*/ static const char str_2C[] = "";
