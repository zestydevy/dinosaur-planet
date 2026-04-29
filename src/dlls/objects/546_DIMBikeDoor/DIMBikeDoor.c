#include "common.h"

typedef struct {
    ObjSetup base;
    s16 _unk18;
    s16 unk1A;
    s16 _unk1C;
    s16 unk1E;    
}DLL546_Setup;

// offset: 0x0 | ctor
void dll_546_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_546_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_546_setup(Object* self, DLL546_Setup* setup, s32 arg2) {
    if (main_get_bits(setup->unk1E) != 0) {
        self->srt.pitch = ((setup->unk1A << 0xD) / 45);
    }
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED | OBJSTATE_CONTROL_DISABLED);
}

// offset: 0x98 | func: 1 | export: 1
void dll_546_control(Object *self) { }

// offset: 0xA4 | func: 2 | export: 2
void dll_546_update(Object *self) { }

// offset: 0xB0 | func: 3 | export: 3
void dll_546_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x104 | func: 4 | export: 4
void dll_546_free(Object *self, s32 a1) { }

// offset: 0x114 | func: 5 | export: 5
u32 dll_546_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x124 | func: 6 | export: 6
s32 dll_546_get_data_size(s32 arg0, s32 arg1) {
    return 0;
}

/*0x0*/ static const char str_0[] = "id %d angle %d\n";
