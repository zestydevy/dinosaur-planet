#include "common.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/objprint.h"
#include "functions.h"
#include "dlls/objects/210_player.h"
#include "sys/gfx/modgfx.h"

typedef struct {
/*00*/ Object *magic;
/*04*/ f32 progress;
/*08*/ f32 unk8;
/*0C*/ u32 soundHandle;
/*10*/ s16 unk10;
/*12*/ s8 _unk12;
/*13*/ s8 unk13;
} DLL290_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u16 time;
/*1A*/ s8 _unk1A;
/*1B*/ u8 id;
/*1C*/ u8 modelInstIdx;
/*1D*/ u8 yaw;
}DLL290_Setup;

typedef struct {
    ObjSetup base;
    u16 unk18;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    u16 _unk1E;
    s32 _unk20;
    s16 unk24;
    s16 _unk26;
    s32 _unk28;
    s16 unk2C;
}MagicDustSetup;

/*0x0*/ static s16 _data_0[] = {
    OBJ_MagicDustSmall, OBJ_MagicDustMid, OBJ_MagicDustLarge, OBJ_MagicDustHuge
};
/*0x8*/ static f32 _data_8[] = {
    -40, -35, -30, -25
};

static f32 dll_290_func_F94(DLL290_Setup* setup);
static void dll_290_func_ED8(Object* self, DLL290_Setup* setup, DLL290_Data* objdata);
static void dll_290_func_E04(Object* self, DLL290_Setup* setup, DLL290_Data* objdata);
static void dll_290_func_C00(Object* self, DLL290_Setup* setup, DLL290_Data* objdata);
static void dll_290_func_7E8(Object* self, DLL290_Setup* setup, DLL290_Data* objdata);
static void dll_290_func_6CC(Object* self, DLL290_Setup* setup, DLL290_Data* objdata);

// offset: 0x0 | ctor
void dll_290_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_290_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_290_setup(Object* self, DLL290_Setup* setup, s32 arg2) {
    ObjectShadow* temp_v0;
    DLL290_Data* objdata;

    objdata = self->data;
    obj_add_object_type(self, OBJTYPE_54);
    obj_add_object_type(self, OBJTYPE_63);
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) == 0) {
        objdata->progress = dll_290_func_F94(setup);
    } else {
        objdata->progress = 1.0f;
    }
    objdata->unk8 = 0.0f;
    objdata->unk13 = 0;
    func_800240BC(self, objdata->progress);
    self->srt.yaw = setup->yaw << 8;
    self->unkB0 |= 0x2000;
    self->modelInstIdx = setup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }
}


// offset: 0x160 | func: 1 | export: 1
void dll_290_control(Object* self) {
    DLL290_Setup* setup;
    DLL290_Data* objdata;

    setup = (DLL290_Setup*)self->setup;
    objdata = self->data;
    self->unkAF |= 8;
    if (self->unkAF & 4) {
        if (main_get_bits(BIT_914) == 0) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            main_set_bits(BIT_914, 1U);
            return;
        }
    }
    switch (objdata->unk13) {
    case 0:
        dll_290_func_6CC(self, setup, objdata);
        break;
    case 1:
        dll_290_func_7E8(self, setup, objdata);
        break;
    case 4:
        dll_290_func_C00(self, setup, objdata);
        break;
    case 2:
        dll_290_func_E04(self, setup, objdata);
        break;
    case 3:
        dll_290_func_ED8(self, setup, objdata);
        break;
    }
    func_80024108(self, objdata->unk8, gUpdateRateF, NULL);
}

// offset: 0x300 | func: 2 | export: 2
void dll_290_update(Object *self) { }

// offset: 0x30C | func: 3 | export: 3
void dll_290_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MtxF* spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    MtxF sp60;
    SRT srt;
    Object* temp_v0;
    DLL290_Setup* setup;
    DLL290_Data* objdata;
    ModelInstance *modelInstance;
    s32 temp_v0_2;

    objdata = self->data;
    setup = (DLL290_Setup*)self->setup;
    if (visibility == 0) {
        return;
    }

    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    modelInstance = self->modelInsts[self->modelInstIdx];
    if ((objdata->magic != NULL) && (modelInstance->unk34 & 8) && (objdata->magic->unkC4 != NULL)) {
        spAC = func_80032170(self, 5);
        if (spAC == NULL) {
            return;
        }

        func_800321E4(self, 5, &spA8, &spA4, &spA0);

        temp_v0_2 = setup->id;
        srt.transl.x = spA8;
        srt.transl.y = _data_8[temp_v0_2 & 3];
        srt.transl.z = spA0;
        srt.yaw = 0;
        srt.pitch = 0;
        srt.roll = 0;
        srt.scale = objdata->magic->srt.scale / self->srt.scale;
        matrix_from_srt(&sp60, &srt);
        matrix_concat_4x3(&sp60, spAC, &sp60);
        objdata->magic->srt.transl.f[0] = sp60.m[3][0] + gWorldX;
        objdata->magic->srt.transl.f[1] = sp60.m[3][1];
        objdata->magic->srt.transl.f[2] = sp60.m[3][2] + gWorldZ;
        func_80034FF0(&sp60);
        draw_object(objdata->magic, gdl, mtxs, vtxs, pols, 1.0f);
        func_80034FF0(NULL);
    }
}

// offset: 0x52C | func: 4 | export: 4
void dll_290_free(Object* self, s32 a1) {
    DLL290_Data* objdata;
    u32 temp_a1;

    objdata = self->data;
    temp_a1 = objdata->soundHandle;
    if (temp_a1 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(temp_a1);
        objdata->soundHandle = 0U;
    }
    obj_free_object_type(self, OBJTYPE_54);
    obj_free_object_type(self, OBJTYPE_63);
}

// offset: 0x5CC | func: 5 | export: 5
u32 dll_290_get_model_flags(Object* self) {
    DLL290_Setup* setup;

    setup = (DLL290_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(setup->modelInstIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5E4 | func: 6 | export: 6
u32 dll_290_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL290_Data);
}

// offset: 0x5F8 | func: 7
static void dll_290_func_5F8(Object* self, s32 arg1) {
    MagicDustSetup* temp_v0;
    ObjSetup* setup;
    Object* magicDust; 
    DLL290_Data* objdata;
    
    setup = self->setup;
    objdata = self->data;
    
    temp_v0 = obj_alloc_create_info(sizeof(MagicDustSetup), arg1);
    temp_v0->unk1A = 0x14;
    temp_v0->unk2C = -1;
    temp_v0->unk1C = -1;
    temp_v0->base.x = self->srt.transl.f[0];
    temp_v0->base.y = self->srt.transl.f[1];
    temp_v0->base.z = self->srt.transl.f[2];
    temp_v0->unk24 = -1;
    temp_v0->base.loadFlags = setup->loadFlags;
    temp_v0->base.byte5 = setup->byte5;
    temp_v0->base.byte6 = setup->byte6;
    temp_v0->base.fadeDistance = setup->fadeDistance - 0xF;
    magicDust = obj_create(&temp_v0->base, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
    magicDust->unkC4 = self;
    objdata->magic = magicDust;
}

// offset: 0x6CC | func: 8
void dll_290_func_6CC(Object* self, DLL290_Setup* setup, DLL290_Data* objdata) {
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) != 0) {
        dll_290_func_5F8(self, _data_0[setup->id & 3]);
        objdata->unk13 = 1;
        objdata->unk10 = rand_next(0x12C, 0x258);
    } else {
        objdata->progress = dll_290_func_F94(setup);
    }
    if (self->curModAnimId != 0) {
        func_80023D30(self, 0, objdata->progress, 0U);
    }
    func_800240BC(self, objdata->progress);
} 


// offset: 0x7E8 | func: 9
void dll_290_func_7E8(Object *self, DLL290_Setup* setup, DLL290_Data* objdata) {
    s32 hitType;
    s32 hitDamage;
    s32 i;
    s32 hitSphereID;
    Object *hitBy;
    f32 distanceToPlayer;
    s32 random;
    Object *player = get_player();
    u32 sp58[4] = { 8, 0xff, 0xff, 0x78 }; 
    DLL_IModgfx* dll;
    SRT transform;

    self->unkAF &= ~8;
    hitType = func_8002601C(self, &hitBy, &hitSphereID, &hitDamage, &transform.transl.x, &transform.transl.y, &transform.transl.z);
    if ((hitType != 0) && (hitDamage != 0) && (hitType != 0)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_618, MAX_VOLUME, 0, NULL, 0, NULL);
        objdata->unk13 = 4;
        objdata->unk8 = 0.03f;
        func_80023D30(self, 3, 0.0f, 0);
        for (i = 20; i > 0; i--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_34E, NULL, 2, -1, NULL);
        }
        transform.transl.x += gWorldX;
        transform.transl.z += gWorldZ;
        transform.roll = 0;
        transform.pitch = 0;
        transform.yaw = 0;
        transform.scale = 1.0f;
        dll = dll_load(DLL_ID_106, 1, FALSE);
        ((DLL_IModgfx*)dll)->vtbl->func0(NULL, 1, &transform, 0x401, -1, sp58);
        if (dll) {
            dll_unload(dll);
        }
    }
    
    if (objdata->unk13 == 1) {
        if (self->curModAnimId == 1) {
            if (self->animProgress >= 1.0f) {
                objdata->unk8 = 0.005f;
                func_80023D30(self, 4, 0.0f, 0);
            } else {
                objdata->unk8 = 0.01f;
            }
        } else {
            objdata->unk10 -= gUpdateRate;
            if (objdata->unk10 <= 0) {
                objdata->unk10 = rand_next(300, 600);
            } else if (self->curModAnimId != 4) {
                objdata->unk8 = 0.005f;
                random = rand_next(0, 99);
                func_80023D30(self, 4, random * 0.01f, 0);
            }
        }
    }
    
    distanceToPlayer = vec3_distance(&self->positionMirror, &player->positionMirror);
    if (objdata->soundHandle == 0) {
        if (distanceToPlayer < 50.0f) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_619, 96, &objdata->soundHandle, NULL, 0, NULL);
        }
    } else if (distanceToPlayer > 70.0f) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
}

// offset: 0xC00 | func: 10
void dll_290_func_C00(Object* self, DLL290_Setup *setup, DLL290_Data* objdata) {
    Object* temp_s0;
    Object* player;
    f32 sp3C;
    s16 temp_v0_2;
    f32 xdiff;
    f32 zdiff;

    player = get_player();
    if (objdata->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
    temp_s0 = objdata->magic;
    if ((temp_s0 != NULL) && (temp_s0->unkC4 != 0)) {
        if ((self->animProgress >= 0.8f)) {
            objdata->magic = NULL;
            temp_s0->unkC4 = 0;
            sp3C = (f32) rand_next(0x27, 0x2C) / 100.0f;
            xdiff = self->srt.transl.x - player->srt.transl.x;
            zdiff = self->srt.transl.z - player->srt.transl.z;
            temp_v0_2 = arctan2_f(xdiff, zdiff);
            rand_next(temp_v0_2 - 0x1000, temp_v0_2 + 0x1000);
            temp_s0->speed.x = (f32) (fsin16_precise(self->srt.yaw) * sp3C);
            temp_s0->speed.z = (f32) (fcos16_precise(self->srt.yaw) * sp3C);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_61A, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
    if (self->animProgress >= 1.0f) {
        objdata->unk13 = 2;
        objdata->unk8 = 0.004f;
        func_80023D30(self, 2, 0.0f, 0U);
    }
}


// offset: 0xE04 | func: 11
void dll_290_func_E04(Object* self, DLL290_Setup* setup, DLL290_Data* objdata) {
    s32 sp24;

    if (self->animProgress >= 1.0f) {
        sp24 = self->opacity;
        sp24 -= (gUpdateRate * 2);
        
        if (sp24 < 0) {
            sp24 = 0;
            objdata->unk13 = 3; 
            objdata->progress = 0.0f;
            objdata->unk8 = 0.0f;
            func_80023D30(self, 0, 0.0f, 0U);
            func_800240BC(self, 0.0f);
        }
        self->opacity = sp24;
    }
    self->objhitInfo->unk58 &= ~1;
}

// offset: 0xED8 | func: 12
void dll_290_func_ED8(Object* self, DLL290_Setup* setup, DLL290_Data* objdata) {
    s32 opacity;

    opacity = self->opacity;
    opacity += gUpdateRate;
    if (opacity >= 0xFF) { 
        objdata->unk13 = 0;
        opacity = 0xFF;
        gDLL_29_Gplay->vtbl->add_time(setup->base.uID, setup->time);
    }
    self->opacity = opacity;
    self->objhitInfo->unk58 |= 1;
}

// offset: 0xF94 | func: 13
f32 dll_290_func_F94(DLL290_Setup* setup) {
    f32 temp_fa0;
    f32 temp_fv0;
    s32 var_v0;

    temp_fv0 = gDLL_29_Gplay->vtbl->get_time_remaining(setup->base.uID);
    var_v0 = setup->time;
    if ((s32) var_v0 < 0x64) {
        var_v0 = 0x64;
    }
    temp_fa0 = temp_fv0 /  var_v0;
    if (temp_fa0 > 1.0f) {
        temp_fa0 = 1.0f;
    } else if (temp_fa0 < 0.0f) {
        temp_fa0 = 0.0f;
    }
    return temp_fa0 = 1.0f - temp_fa0;
}


/*0x0*/ static const char str_0[] = "MAGICPLANT: modelno error\n";
