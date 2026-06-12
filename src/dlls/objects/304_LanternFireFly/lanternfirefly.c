#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} DLL304_Setup;

typedef struct {
    Object* unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10; //x (end of array of 4 last values, maybe?)
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20; //y
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30; //z
    Vec3f unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    Vec3f unk54;
    s32 unk60;
    u32 unk64;
    s16 unk68;
    s16 unk6A;
    s16 unk6C;
    s8 unk6E;
    s8 unk6F;
    s8 unk70;
    s8 unk71;
    s8 unk72;
    s8 unk73;
    union { //TODO: try to rematch later as just a bitfield
        u32 unk74;
        u8 unk74_bytes[4];
        struct {
            u32 unk74_1 : 2;  
            u32 unk74_0 : 30;  
        };
    };
} DLL304_Data; //0x78

/*0x0*/ static s8 data_0 = 0;
/*0x4*/ static s8 data_4 = 0;

static void dll_304_func_984(Object* self);
void dll_304_func_BE0(Object* self, Vec3f* arg1);
void dll_304_func_E54(Object* self, f32 x, f32 y, f32 z);

// offset: 0x0 | ctor
void dll_304_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_304_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_304_setup(Object* self, DLL304_Setup* objSetup, s32 reset) {
    DLL304_Data* objData = self->data;
    Vec3f sp28;

    obj_add_object_type(self, OBJTYPE_FireFly);
    
    sp28.x = 0.0f;
    sp28.y = 0.0f; 
    sp28.z = 0.0f;
    
    objData->unk4 = sp28.f[0];
    objData->unk14 = sp28.f[1];
    objData->unk24 = sp28.f[2];
    
    objData->unk8 = sp28.f[0];
    objData->unk18 = sp28.f[1];
    objData->unk28 = sp28.f[2];
    
    objData->unkC = sp28.f[0]; 
    objData->unk1C = sp28.f[1];
    objData->unk2C = sp28.f[2];
    
    objData->unk10 = sp28.f[0];
    objData->unk20 = sp28.f[1];
    objData->unk30 = sp28.f[2];

    objData->unk0 = 0;
    objData->unk72 = 0;
    objData->unk44 = 0.08f;
    objData->unk48 = 0.0275f;
    objData->unk40 = 1.0f;
    objData->unk70 = 0;
    objData->unk6F = 0;
    objData->unk6A = rand_next(500, 1500);
    objData->unk68 = rand_next(0, 65000);
    objData->unk6C = 4;
    objData->unk6E = objSetup->unk19;
    objData->unk4C = 0.0f;
    objData->unk50 = 5;
    objData->unk54.x = objSetup->base.x;
    objData->unk54.y = objSetup->base.y;
    objData->unk54.z = objSetup->base.z;
    objData->unk73 = 0;
    objData->unk74_bytes[0] &= (u16)~(0x80 | 0x40);
}

/*0x0*/ static const char str_0[] = " Creating Light ";

// offset: 0x190 | func: 1 | export: 1
void dll_304_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/304_LanternFireFly/dll_304_control.s")

// offset: 0x84C | func: 2 | export: 2
void dll_304_update(Object *self) { }

// offset: 0x858 | func: 3 | export: 3
void dll_304_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8AC | func: 4 | export: 4
void dll_304_free(Object* self, s32 onlySelf) {
    DLL304_Data* objData = self->data;
    
    if (onlySelf == FALSE) {
        if (objData->unk0 != NULL) {
            obj_destroy_object(objData->unk0);
            objData->unk0 = NULL;

            if ((objData->unk74_1) != 1) {
                data_4 = 0;
            }
        }
    }
    
    obj_free_object_type(self, OBJTYPE_FireFly);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x960 | func: 5 | export: 5
u32 dll_304_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x970 | func: 6 | export: 6
u32 dll_304_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL304_Data);
}

// offset: 0x984 | func: 7
void dll_304_func_984(Object* self) {
    DLL304_Data* objData = self->data;
    
    objData->unk4 = objData->unk8;
    objData->unk14 = objData->unk18;
    objData->unk24 = objData->unk28;
    
    objData->unk8 = objData->unkC;
    objData->unk18 = objData->unk1C;
    objData->unk28 = objData->unk2C;
    
    objData->unkC = objData->unk10;
    objData->unk1C = objData->unk20;
    objData->unk2C = objData->unk30;
    
    if ((objData->unk74 >> 30) == 1) {
        objData->unk44 = (vec3_distance(&self->globalPosition, &get_player()->globalPosition) * 0.0015f) + 0.0001f;
    } else {
        objData->unk44 = rand_next(60, 90) * 0.0015f;
    }
    
    objData->unk10 = objData->unk34.x;
    objData->unk20 = objData->unk34.y;
    objData->unk30 = objData->unk34.z;
}

// offset: 0xAB0 | func: 8
void dll_304_func_AB0(Object* self) {
    DLL304_Data* objData;
    SRT transform;

    objData = self->data;
    
    objData->unk34.x = 0.0f;
    objData->unk34.y = rand_next(-objData->unk6C, objData->unk6C);
    if (objData->unk50 < 21.0f) {
        objData->unk34.z = 0.0f;
    } else {
        objData->unk34.z = objData->unk50 - rand_next(20, (s16)objData->unk50);
    }

    objData->unk68 += (s16)rand_next(3000, 5000);
    
    transform.transl.x = 0;
    transform.transl.y = 0;
    transform.transl.z = 0;
    transform.scale = 1;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = objData->unk68;
    rotate_vec3(&transform, objData->unk34.f);
}

// offset: 0xBE0 | func: 9 | export: 7
void dll_304_func_BE0(Object* self, Vec3f* arg1) {
    DLL304_Data* objData = self->data;
    
    arg1->x -= objData->unk54.x;
    arg1->y -= objData->unk54.y;
    arg1->z -= objData->unk54.z;
    
    objData->unk34.x = arg1->x;
    objData->unk34.y = arg1->y;
    objData->unk34.z = arg1->z;

    objData->unk70 = 4;
}

// offset: 0xC3C | func: 10 | export: 8
void dll_304_func_C3C(Object* self) {
    DLL304_Data* objData;
    DLL304_Setup* objSetup;
    Object* player;
    Vec3f sp38; //38

    objSetup = (DLL304_Setup*)self->setup;
    objData = self->data;
    
    objData->unk6C = objSetup->unk18;
    objData->unk6E = objSetup->unk19;
    objData->unk4C = 1.0f;
    objData->unk50 = objSetup->unk1C;
    objData->unk73 = 0;
    func_8005B5B8(self, NULL, 1);
    
    player = get_player();
    sp38.x = player->globalPosition.x;
    sp38.y = player->globalPosition.y;
    sp38.z = player->globalPosition.z;
    sp38.y += 40.0f;
    dll_304_func_E54(self, player->globalPosition.x, player->globalPosition.y + 34.0f, player->globalPosition.z);
    dll_304_func_BE0(self, &sp38);
    dll_304_func_984(self);
    dll_304_func_984(self);
    dll_304_func_984(self);
    dll_304_func_984(self);
    dll_304_func_984(self);
    dll_304_func_984(self);
    
    objData->unk74_bytes[0] = (objData->unk74_bytes[0] & 0xFF3F) | 0x40;
    if (objData->unk64 == 0) {
        gDLL_6_AMSFX->vtbl->play(self, 0x7F6, MAX_VOLUME, &objData->unk64, NULL, 0, NULL);
    }
    objData->unk60 = objSetup->unk1A;
    
    main_increment_bits(0x698);
}

// offset: 0xE54 | func: 11 | export: 9
void dll_304_func_E54(Object* self, f32 x, f32 y, f32 z) {
    DLL304_Data* objData;

    objData = self->data;
    objData->unk54.x = x;
    objData->unk54.y = y;
    objData->unk54.z = z;
}
