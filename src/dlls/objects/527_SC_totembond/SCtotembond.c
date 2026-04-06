#include "common.h"

#include "dlls/objects/210_player.h"
#include "dlls/objects/519_SC_levelcontrol.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
} DLL527_Setup;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    s32 unk8;
} DLL527_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s8 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s8 unk32;
} SCLightFoot_Setup; //0x38

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    s8 unk1F;
} SCFlameGameFlame_Setup; //0x20

/*0x0*/ static u16 data_0[] = {
    0x064d, 0x0064e, 0x064f, 0x00650
};
/*0x8*/ static u16 data_8[] = {
    0x0768, 0x00769, 0x076a, 0x0076b
};
/*0x10*/ static u16 data_10[] = {
    0x0084, 0x00085, 0x0086, 0x00087
};

static int SCTotemBond_func_3A4(Object* self, Object* objOverride, AnimObj_Data* animData, s8 arg3);
static void SCTotemBond_func_7F8(Object* self, DLL527_Data* objData);
static void SCTotemBond_func_960(Object* self, DLL527_Data* objData);
static s32 SCTotemBond_func_A70(Object* self, DLL527_Data* objData);
static void SCTotemBond_func_B80(Object* self, u8 arg1);

// offset: 0x0 | ctor
void SCTotemBond_ctor(void *dll) { }

// offset: 0xC | dtor
void SCTotemBond_dtor(void *dll) { }


// offset: 0x18 | func: 0 | export: 0
void SCTotemBond_setup(Object* self, DLL527_Setup* objSetup, s32 arg2) {
    DLL527_Data* objData = self->data;
    
    objData->unk2 = (u16)self->srt.yaw / 16384;
    self->animCallback = SCTotemBond_func_3A4;
    self->unkB0 |= 0x6000;
}

// offset: 0x68 | func: 1 | export: 1
void SCTotemBond_control(Object* self) {
    DLL527_Data* objData;
    Object* player;
    Object* sidekick;

    objData = self->data;
    player = get_player();
    sidekick = get_sidekick();
    
    if (objData->unk0 & 1) {
        SCTotemBond_func_7F8(self, objData);
    }
    
    if (objData->unk0 & 2) {
        if (sidekick != NULL) {
            ((DLL_Unknown*)sidekick->dll)->vtbl->func[18].withTwoArgs(sidekick, objData->unk8);
        }
        
        ((DLL_Unknown*)player->dll)->vtbl->func[71].withFourArgs(player, &self->srt.transl, self, 0);
        
        if (main_get_bits(BIT_64C) != 0) {
            main_set_bits(BIT_64C, 0);
            if (SCTotemBond_func_A70(self, objData) != 0) {
                objData->unk0 |= 4;
            }
            
            objData->unk4 = gDLL_6_AMSFX->vtbl->play_sound(self, 0x796, 0x7F, 0, 0, 0, 0);
        }

        if (objData->unk2 != (((u16) ((0, self->srt)).yaw) / 16384)) {
            self->srt.yaw -= 256.0f * gUpdateRateF;
            if ((u16)self->srt.yaw / 16384 == objData->unk2) {
                main_set_bits(data_0[objData->unk2], 1);
            }
        }
        
        if (objData->unk0 & 0xC) {
            SCTotemBond_func_960(self, objData);
        }
    }
}

// offset: 0x2B8 | func: 2 | export: 2
void SCTotemBond_update(Object *self) { }

// offset: 0x2C4 | func: 3 | export: 3
void SCTotemBond_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x318 | func: 4 | export: 4
void SCTotemBond_free(Object* self, s32 arg1) {
    DLL527_Data* objData = self->data;
    
    if (objData->unk4 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objData->unk4);
        objData->unk4 = 0;
    }
}

// offset: 0x380 | func: 5 | export: 5
u32 SCTotemBond_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x390 | func: 6 | export: 6
u32 SCTotemBond_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL527_Data);
}

// offset: 0x3A4 | func: 7
int SCTotemBond_func_3A4(Object* self, Object* objOverride, AnimObj_Data* animData, s8 arg3) {
    s32 i;
    DLL527_Data* objData;

    objData = self->data;
    animData->unk62 = 0;

    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case 1:
            objData->unk0 |= 1;
            self->srt.yaw = 0x7FFF;
            gDLL_3_Animation->vtbl->func19(0x56, 1, 0, 0);
            break;
        case 2:
            SCTotemBond_func_B80(self, 3);
            break;
        case 3:
            SCTotemBond_func_B80(self, 2);
            break;
        }
    }
    
    return 0;
}

// offset: 0x510 | func: 8
static void SCTotemBond_func_510(Object* self, DLL527_Data* objData, f32 arg2) {
    s32 temp_s4;
    s8 var_s6;
    s8 i;
    SCFlameGameFlame_Setup* setupFlame;
    SCLightFoot_Setup* setupLF;
    DLL527_Setup* objSetup;

    for (i = 0, var_s6 = 1; i < 4; i++) {
        objSetup = (DLL527_Setup*)self->setup;
        temp_s4 = i << 0xE;
        
        setupLF = obj_alloc_setup(0x38, OBJ_SC_lightfootSpe);
        setupLF->base.x = (fsin16_precise(self->srt.yaw + temp_s4) * arg2) + self->srt.transl.x;
        setupLF->base.y = self->srt.transl.y;
        setupLF->base.z = (fcos16_precise(self->srt.yaw + temp_s4) * arg2) + self->srt.transl.z;
        setupLF->base.loadFlags = objSetup->base.loadFlags;
        setupLF->base.byte5 = objSetup->base.byte5;
        setupLF->base.byte6 = objSetup->base.byte6;
        setupLF->base.fadeDistance = objSetup->base.fadeDistance;
        setupLF->unk18 = -1;
        setupLF->unk1A = 0x64C;
        setupLF->unk1C = data_8[var_s6];
        setupLF->unk30 = data_0[var_s6];
        setupLF->unk2A = (self->srt.yaw + temp_s4 + 0x8000) >> 8;
        setupLF->unk32 = 1;
        
        setupFlame = obj_alloc_setup(0x20, OBJ_SC_flamegamefla);
        setupFlame->base.x = (fsin16_precise(self->srt.yaw + temp_s4) * arg2) + self->srt.transl.x;
        setupFlame->base.y = self->srt.transl.y;
        setupFlame->base.z = (fcos16_precise(self->srt.yaw + temp_s4) * arg2) + self->srt.transl.z;
        setupFlame->base.loadFlags = objSetup->base.loadFlags;
        setupFlame->base.byte5 = objSetup->base.byte5;
        setupFlame->base.byte6 = objSetup->base.byte6;
        setupFlame->base.fadeDistance = objSetup->base.fadeDistance;
        setupFlame->unk18 = 75;
        setupFlame->unk19 = 0;
        setupFlame->unk1A = data_10[var_s6];
        setupFlame->unk1C = 5;
        setupFlame->unk1D = 7;
        setupFlame->unk1E = 7;
        setupFlame->unk1F = (self->srt.yaw + temp_s4 + 0x8000) >> 8;
        
        obj_create((ObjSetup*)setupLF, 5, -1, -1, 0);
        obj_create((ObjSetup*)setupFlame, 5, -1, -1, 0);
        
        var_s6++;
        if (var_s6 >= 4) {
            var_s6 = 0;
        }
    }
}

// offset: 0x7F8 | func: 9
void SCTotemBond_func_7F8(Object* self, DLL527_Data* objData) {
    Object* player;
    Object* kyte;

    kyte = get_sidekick();
    if (kyte != NULL) {
        objData->unk8 = ((DLL_Unknown*)kyte->dll)->vtbl->func[16].withOneArgS32(kyte);
        if (objData->unk8 <= 0) {
            objData->unk8 = 1;
        }
    } else {
        objData->unk8 = 1;
    }
    
    self->srt.yaw = 0x7FFF;
    func_800267A4(self);
    SCTotemBond_func_510(self, objData, -70.0f);
    main_set_bits(data_0[objData->unk2], 1);
    self->opacity = 0;
    
    objData->unk0 &= ~1;
    objData->unk0 |= 2;
    main_set_bits(0x46E, 0x83);
    
    player = get_player();
    gDLL_29_Gplay->vtbl->restart_set(&player->srt.transl, player->srt.yaw, map_get_layer());
}


// offset: 0x960 | func: 10
void SCTotemBond_func_960(Object* self, DLL527_Data* objData) {
    Object* player;

    player = get_player();
    
    gDLL_29_Gplay->vtbl->restart_clear();
    gDLL_2_Camera->vtbl->change_camera_module(0x54, 0, 3, 0, 0, 0, 0);
    self->opacity = OBJECT_OPACITY_MAX;
    ((DLL_210_Player*)player->dll)->vtbl->func71(player, 0, 0, 0);
    
    func_8002674C(self);
    
    if (objData->unk0 & 4) {
        main_set_bits(BIT_2BC, 1);
    }
    objData->unk0 = 0;
}

// offset: 0xA70 | func: 11
s32 SCTotemBond_func_A70(Object* self, DLL527_Data* objData) {
    s8 sp44[4];
    u8 random;
    u8 matchIndex;
    u8 i;

    for (i = 0, matchIndex = 0; i < 4; i++) {
        if (main_get_bits(data_8[i]) == 0) {
            sp44[matchIndex++] = i;
        }
    }
    
    if (!matchIndex) {
        return 1;
    }
    
    random = sp44[rand_next(0, matchIndex - 1)];
    if (objData->unk2 == random) {
        main_set_bits(data_0[objData->unk2], 1);
    }
    objData->unk2 = random;
    
    return 0;
}

// offset: 0xB80 | func: 12
void SCTotemBond_func_B80(Object* self, u8 arg1) {
    Object** objects;
    s32 index;
    s32 count;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        if ((self != objects[index]) && (objects[index]->id == OBJ_SC_levelcontrol)) {
            ((DLL_519_SC_Levelcontrol*)objects[index]->dll)->vtbl->func7(objects[index], arg1);
            return;
        }
    }
}
