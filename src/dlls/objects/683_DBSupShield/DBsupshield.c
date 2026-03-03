#include "common.h"
#include "macros.h"

typedef struct {
    ObjSetup base;
    s16 unused18;
    s16 unk1A; //objhits-related
} DBSupShield_Setup;

// offset: 0x0 | ctor
void DBSupShield_ctor(void *dll) { }

// offset: 0xC | dtor
void DBSupShield_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBSupShield_setup(Object* self, DBSupShield_Setup* objSetup, s32 arg2) {
    if (objSetup->unk1A) {
        func_80026940(self, objSetup->unk1A);
    }
}

// offset: 0x68 | func: 1 | export: 1
void DBSupShield_control(Object *self) { }

// offset: 0x74 | func: 2 | export: 2
void DBSupShield_update(Object *self) {
    s32 collisionType;
    Object* bullet;

    collisionType = func_80025F40(self, &bullet, 0, 0);
    if (collisionType == 18) {
        //@bug? Should this be checking "if (bullet->id == OBJ_DBbullet)" before calling its DLL function?
        bullet->id = OBJ_DBbullet; 
        if (bullet->id != 0) {
            STUBBED_PRINTF(" Killing Bullet ");
            ((DLL_Unknown*)bullet->dll)->vtbl->func[10].withOneArg((s32)bullet); //TO-DO: proper interface
        }
    }
}

// offset: 0xF4 | func: 3 | export: 3
void DBSupShield_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x10C | func: 4 | export: 4
void DBSupShield_free(Object *self, s32 a1) { }

// offset: 0x11C | func: 5 | export: 5
u32 DBSupShield_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x12C | func: 6 | export: 6
u32 DBSupShield_get_data_size(Object *self, u32 a1) {
    return 0;
}
