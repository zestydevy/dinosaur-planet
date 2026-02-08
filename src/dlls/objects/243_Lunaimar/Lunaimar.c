#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/objects/common/sidekick.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "unktypes.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    s16 unk12;
    Object *unk14;
} Lunaimar_ActualData;

typedef struct {
    Baddie unk0;
    Lunaimar_ActualData unk3FC;
} Lunaimar_Data;

typedef struct {
    Baddie_Setup base;
} Lunaimar_Setup;

enum LunaimarAnimStates {
    LUNAIMAR_ASTATE_0 = 0,
    LUNAIMAR_ASTATE_1 = 1,
    LUNAIMAR_ASTATE_2 = 2,
    LUNAIMAR_ASTATE_3 = 3,
    LUNAIMAR_ASTATE_4 = 4,
    LUNAIMAR_ASTATE_5 = 5
};

enum LunaimarLogicStates {
    LUNAIMAR_LSTATE_0 = 0,
    LUNAIMAR_LSTATE_1 = 1,
    LUNAIMAR_LSTATE_2 = 2,
    LUNAIMAR_LSTATE_3 = 3,
    LUNAIMAR_LSTATE_4 = 4,
    LUNAIMAR_LSTATE_5 = 5
};

/*0x0*/ static u16 _data_0[] = { // sound IDs
    0x0a81, 0x0a82, 0x0a84, 0x0a85
};
/*0x8*/ static u16 _data_8[] = { 0x0ac5, 0x0ac6 }; // sound IDs
/*0xC*/ static s16 _data_C[] = {
    0x0000, 0x0000, 0x0001, 0x0001, 0x0002, 0x0000
};
/*0x18*/ static f32 _data_18[] = {
    0.004f, 0.006f, 0.01f, 0.01f, 0.01f
};
/*0x2C*/ static s32 _data_2C[] = { // anim states
    4, 4, 4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4
};
/*0x9C*/ static s8 _data_9C[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1, -1, -1, -1, -1, 
    -1, -1, -1, -1
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[6];
/*0x18*/ static ObjFSA_StateCallback sLogicStateCallbacks[6];

static void dll_243_func_700(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void dll_243_func_EEC(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void dll_243_func_C44(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void dll_243_func_11C0(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void dll_243_func_1484(Object *self, Baddie *baddie);
static void dll_243_func_1614(Object *self, Lunaimar_ActualData *objdata);

static s32 dll_243_anim_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_anim_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_anim_state_2(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_anim_state_3(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_anim_state_4(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_anim_state_5(Object *self, ObjFSA_Data *fsa, f32 updateRate);

static s32 dll_243_logic_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_logic_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_logic_state_2(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_logic_state_3(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_logic_state_4(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_243_logic_state_5(Object *self, ObjFSA_Data *fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void dll_243_func_0(void) {
    sAnimStateCallbacks[0] = dll_243_anim_state_0;
    sAnimStateCallbacks[1] = dll_243_anim_state_1;
    sAnimStateCallbacks[2] = dll_243_anim_state_2;
    sAnimStateCallbacks[3] = dll_243_anim_state_3;
    sAnimStateCallbacks[4] = dll_243_anim_state_4;
    sAnimStateCallbacks[5] = dll_243_anim_state_5;

    sLogicStateCallbacks[0] = dll_243_logic_state_0;
    sLogicStateCallbacks[1] = dll_243_logic_state_1;
    sLogicStateCallbacks[2] = dll_243_logic_state_2;
    sLogicStateCallbacks[3] = dll_243_logic_state_3;
    sLogicStateCallbacks[4] = dll_243_logic_state_4;
    sLogicStateCallbacks[5] = dll_243_logic_state_5;
}


// offset: 0xB4 | ctor
void dll_243_ctor(void *dll) {
    dll_243_func_0();
}

// offset: 0xF4 | dtor
void dll_243_dtor(void *dll) { }

// offset: 0x100 | func: 1 | export: 0
void dll_243_setup(Object *self, Baddie_Setup *setup, s32 arg2) {
    f32 temp_ft4;
    s32 var_v0;
    Baddie *baddie;
    Lunaimar_ActualData *temp_s2;

    baddie = self->data;
    var_v0 = 6;
    if (arg2 != 0) {
        var_v0 = 7;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 6, 6, 0, (u8) var_v0, 20.0f);
    self->animCallback = NULL;
    temp_s2 = baddie->objdata;
    func_80023D30(self, 4, 0.0f, 0);
    self->unkAF |= 8;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &baddie->fsa, LUNAIMAR_ASTATE_0);
    baddie->fsa.logicState = LUNAIMAR_LSTATE_0;
    baddie->fsa.unk4.mode = 0;
    func_800267A4(self);
    temp_s2->unk0 = rand_next(0xA0, 0x12C);
    temp_s2->unk4 = rand_next(0, 0x1F4);
    temp_s2->unk14 = NULL;
    temp_s2->unk8 = 0.0f;
    self->unkB0 |= 0x2000;
}

// offset: 0x268 | func: 2 | export: 1
void dll_243_control(Object *self) {
    Baddie *baddie;
    Lunaimar_Setup *setup;

    baddie = (Baddie*)self->data;
    setup = (Lunaimar_Setup*)self->setup;
    if (self->unkDC != 0) {
        if (baddie->fsa.logicState != LUNAIMAR_LSTATE_3) {
            if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.base.uID) != 0) {
                gDLL_33_BaddieControl->vtbl->setup(self, &setup->base, baddie, 6, 6, 0, 0x26, 20.0f);
                baddie->unk3B6 = 0;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
                func_80023D30(self, 4, 0.0f, 0x10);
                baddie->fsa.unk33A = 0;
                self->opacity = 0xFF;
                baddie->unk3B6 = 0;
                self->unkAF |= 8;
            }
        }
    } else {
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 0) == 0) {
            baddie->unk3B6 = 0;
            return;
        }
        dll_243_func_700(self, baddie, &baddie->fsa);
        if (baddie->unk3B6 == 0) {
            dll_243_func_EEC(self, baddie, &baddie->fsa);
        } else {
            dll_243_func_C44(self, baddie, &baddie->fsa);
        }
    }
}

// offset: 0x44C | func: 3 | export: 2
void dll_243_update(Object *self) { }

// offset: 0x458 | func: 4 | export: 3
void dll_243_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    Baddie *baddie;

    baddie = self->data;
    if ((visibility != 0) && (self->unkDC == 0) && (baddie->unk3B6 != 0)) {
        if (baddie->unk3E8 != 0.0f) {
            func_80036FBC(0xC8, 0, 0, (u8) baddie->unk3E8);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (baddie->unk3B2 & 0x60) {
            if (baddie->unk3B2 & 0x20) {
                gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
                gDLL_32->vtbl->func2(self, 0x330, &baddie->unk3E8);
            }
            gDLL_32->vtbl->func2(self, 0x32F, &baddie->unk3E8);
        }
    }
}

// offset: 0x644 | func: 5 | export: 4
void dll_243_free(Object *self, s32 a1) {
    Baddie *baddie;

    baddie = self->data;
    obj_free_object_type(self, OBJTYPE_4);
    gDLL_33_BaddieControl->vtbl->func15(self, baddie, 0x20);
}

// offset: 0x6B4 | func: 6 | export: 5
u32 dll_243_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x6C4 | func: 7 | export: 6
u32 dll_243_get_data_size(Object *self, u32 a1) {
    return sizeof(Lunaimar_Data);
}

// offset: 0x6D8 | func: 8 | export: 7
s16 dll_243_func_6D8(Object *self, UNK_TYPE_32 a1) {
    Baddie *baddie = self->data;
    return baddie->fsa.animState;
}

// offset: 0x6EC | func: 9 | export: 8
void dll_243_func_6EC(Object *self, UNK_TYPE_32 a1, UNK_TYPE_32 a2) { }

// offset: 0x700 | func: 10
static void dll_243_func_700(Object *self, Baddie *baddie, ObjFSA_Data *fsa) {
    /*.bss+0x30*/ static SRT _bss_30;
    Lunaimar_ActualData *objdata = baddie->objdata;
    s16 sp84[] = {0x0206, 0x0167, 0x0165, 0x0206};
    s16 sp7C[] = {0x0206, 0x0167, 0x0165, 0x0206};
    Object *player;
    Vec3f sp6C;
    Object *temp_a0;
    s32 i;
    u16 sp62;
    u16 sp60;
    u16 sp5E;
    s32 var_v1;

    player = get_player();
    if (fsa->target != NULL) {
        VECTOR_SUBTRACT(fsa->target->positionMirror, self->positionMirror, sp6C);
        fsa->targetDist = VECTOR_MAGNITUDE(sp6C);
    }
    gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 4);
    gDLL_33_BaddieControl->vtbl->func4(self, player, 4, &sp62, &sp60, &sp5E);
    if ((sp62 == 1) || (sp62 == 2)) {
        if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, (s32) baddie->unk39E, _data_2C, _data_9C, 1, &baddie->unk3A8, &_bss_30) != 0) {
            for (i = 50; i != 0; i--) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_711, NULL, PARTFXFLAG_4, -1, NULL);
            }
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_713, NULL, PARTFXFLAG_2, -1, NULL);
            temp_a0 = player->linkedObject;
            var_v1 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[19].withOneArgS32((s32)temp_a0);
            if (var_v1 >= 4) {
                var_v1 = 3;
            }
            _bss_30.scale = (f32) sp84[var_v1];
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &_bss_30, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            _bss_30.transl.x -= self->srt.transl.x;
            _bss_30.transl.y -= self->srt.transl.y;
            _bss_30.transl.z -= self->srt.transl.z;
            _bss_30.scale = (f32) sp7C[var_v1];
            for (i = 0; i < 4; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &_bss_30, PARTFXFLAG_2, -1, NULL);
            }
        }
    } else if (gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, (s32) baddie->unk39E, NULL, NULL, 1, &baddie->unk3A8, &_bss_30) != 0) {
        dll_243_func_1614(self, objdata);
    }
    if (objdata->unk14 != NULL) {
        if (objdata->unkC <= 0.0f) {
            objdata->unk14->opacity = 0;
            objdata->unkC = 0.0f;
            return;
        }
        objdata->unk14->opacity = rand_next(0, (u8) objdata->unkC);
        objdata->unk14->srt.roll = self->srt.roll;
        objdata->unk14->srt.pitch = self->srt.pitch;
        objdata->unk14->srt.yaw = self->srt.yaw;
        objdata->unkC -= gUpdateRateF * 4.0f;
    }
}

// offset: 0xC44 | func: 11
static void dll_243_func_C44(Object *self, Baddie *baddie, ObjFSA_Data *fsa) {
    Lunaimar_ActualData *objdata;
    Object *sidekick;
    Vec3f sp44;
    f32 sp40;
    s32 *sp3C;

    objdata = (Lunaimar_ActualData*)baddie->objdata;
    sidekick = get_sidekick();
    sp3C = func_800348A0(self, 0, 0);
    objdata->unk12 += 0x1000;
    *sp3C = (s32) ((fsin16_precise(objdata->unk12) + 1.0f) * 127.0f);
    VECTOR_SUBTRACT(sidekick->positionMirror, self->positionMirror, sp44);
    sp40 = VECTOR_MAGNITUDE(sp44);
    if (((DLL_ISidekick*)sidekick->dll)->vtbl->func24(sidekick) != 0 && (sp40 < baddie->unk3E2)) {
        baddie->unk3B2 |= 4;
    } else {
        baddie->unk3B2 &= ~0x4;
    }
    if (baddie->unk3B2 & 4) {
        fsa->target = sidekick;
    } else {
        fsa->target = get_player();
    }
    dll_243_func_11C0(self, baddie, fsa);
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, -1);
    gDLL_18_objfsa->vtbl->func11(self, fsa, gUpdateRateF, 5);
    baddie->unk3AC = self->unkC0;
    self->unkC0 = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->unkC0 = baddie->unk3AC;
}

// offset: 0xEEC | func: 12
static void dll_243_func_EEC(Object *self, Baddie *baddie, ObjFSA_Data *fsa) {
    Lunaimar_ActualData *objdata;

    objdata = baddie->objdata;
    objdata->unk0 -= gUpdateRateF;
    if (objdata->unk0 <= 0.0f) {
        gDLL_6_AMSFX->vtbl->play_sound(self, _data_0[rand_next(2, 3)], 0x30, &baddie->unk3A8, NULL, 0, NULL);
        objdata->unk0 = (f32) rand_next(0xA0, 0x12C);
    }
    objdata->unk4 -= gUpdateRateF;
    if (objdata->unk4 <= 0.0f) {
        gDLL_6_AMSFX->vtbl->play_sound(self, _data_8[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
        objdata->unk4 = (f32) rand_next(0xFA, 0x1F4);
        objdata->unk8 = (f32) rand_next(0x1E, 0x46);
    }
    if (objdata->unk8 > 0.0f) {
        objdata->unk8 -= gUpdateRateF;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_718, NULL, PARTFXFLAG_2, -1, &objdata->unk8);
    }
    if (gDLL_33_BaddieControl->vtbl->func17(self, fsa, (f32) baddie->unk3E2, 0x8000) != NULL) {
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, 0, 0, 4, -1);
        fsa->unk33D = 0;
        baddie->unk3B6 = 1;
    }
}

// offset: 0x11C0 | func: 13
static void dll_243_func_11C0(Object *self, Baddie *baddie, ObjFSA_Data *fsa) {
    Lunaimar_ActualData *objdata;
    s32 i;
    s32 var_s0_2;

    objdata = baddie->objdata;
    if (fsa->unk308 & 1) {
        fsa->unk308 &= ~1;
        gDLL_6_AMSFX->vtbl->play_sound(self, _data_0[3], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    if (fsa->unk308 & 0x80) {
        objdata->unk10 = rand_next(0, 2);
        fsa->unk308 &= ~0x80;
        gDLL_6_AMSFX->vtbl->play_sound(self, _data_0[objdata->unk10], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
        i = ((s32) -objdata->unk10 * 10) + 20;
        while (i != 0) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_711, NULL, PARTFXFLAG_4, -1, NULL);
            i--;
        }
    }
    if (fsa->unk308 & 0x40) {
        fsa->unk308 &= ~0x40;
        dll_243_func_1484(self, baddie);
    }
    if (fsa->unk308 & 0x800) {
        fsa->unk308 &= ~0x800;
        dll_243_func_1484(self, baddie);
    }
    if (fsa->unk308 & 0x200) {
        fsa->unk308 &= ~0x200;
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AA4_Hock_Up, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (fsa->unk308 & 0x400) {
        objdata->unk10 = 3;
        i = 10;
        while (i != 0) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_710, NULL, PARTFXFLAG_4, -1, NULL);
            i--;
        }
        fsa->unk308 &= ~0x400;
    }
}

// offset: 0x1484 | func: 14
void dll_243_func_1484(Object *self, Baddie *baddie) {
    ObjSetup *spitSetup;
    Object *spit;
    f32 temp_fv0;

    spitSetup = obj_alloc_create_info(0x24, OBJ_LunaimarSpit);
    spitSetup->x = self->srt.transl.x;
    spitSetup->y = self->srt.transl.y + 20.0f;
    spitSetup->z = self->srt.transl.z;
    spitSetup->loadFlags = OBJSETUP_LOAD_FLAG1;
    spitSetup->fadeFlags = OBJSETUP_FADE_DISABLE;
    spitSetup->loadDistance = 0xFF;
    spitSetup->fadeDistance = 0xFF;
    spit = obj_create(spitSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
    if (spit != NULL) {
        temp_fv0 = (baddie->fsa.targetDist / (f32) baddie->unk3E2) * 60.0f;
        spit->speed.x = (baddie->fsa.target->srt.transl.x - spitSetup->x) / temp_fv0;
        spit->speed.y = (((f32) rand_next(-0xA, 0xA) + (baddie->fsa.target->srt.transl.y + 20.0f)) - spitSetup->y) / temp_fv0;
        spit->speed.z = (baddie->fsa.target->srt.transl.z - spitSetup->z) / temp_fv0;
        spit->unkC4 = self;
    }
}

// offset: 0x1614 | func: 15
static void dll_243_func_1614(Object *self, Lunaimar_ActualData *objdata) {
    ObjSetup *setup;
    ObjSetup *shieldSetup;
    s32 i;
    Object *shield;

    objdata->unkC = 255.0f;
    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AB4_Magic_Barrier_Deflect, MAX_VOLUME, NULL, NULL, 0, NULL);
    for (i = 20; i > 0; i--) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_717, NULL, PARTFXFLAG_4, -1, NULL);
    }
    if (objdata->unk14 == NULL) {
        setup = self->setup;
        shieldSetup = obj_alloc_create_info(0x24, OBJ_LunaimarShield);
        shieldSetup->x = self->srt.transl.x;
        shieldSetup->y = self->srt.transl.y + 10.0f;
        shieldSetup->z = self->srt.transl.z;
        shieldSetup->loadFlags = setup->loadFlags;
        shieldSetup->byte5 = setup->byte5;
        shieldSetup->byte6 = setup->byte6;
        shieldSetup->fadeDistance = setup->fadeDistance;
        shield = obj_create(shieldSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
        objdata->unk14 = shield;
        shield->unkC4 = self;
    }
}

// offset: 0x1780 | func: 16
static s32 dll_243_anim_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;

    baddie = self->data;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 4, 0.0f, 0);
        fsa->unk33A = 0;
    }
    if (fsa->enteredAnimState != 0) {
        fsa->unk4.mode = 1;
        main_set_bits(baddie->unk39E, 1);
        self->unkAF &= ~0x8;
        self->opacity = 0xFF;
        fsa->unk341 = 1;
        fsa->animTickDelta = ((f32) baddie->unk3B8 / 10000.0f) + 0.012f;
        func_8002674C(self);
    } else {
        func_80026128(self, 0xA, 1, -1);
        self->objhitInfo->unk5D = 0xA;
        self->objhitInfo->unk5E = 1;
        func_80028D2C(self);
        if (fsa->unk33A != 0) {
            baddie->unk3B6 = 1;
        }
    }
    return 0;
}

// offset: 0x18F0 | func: 17
static s32 dll_243_anim_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;

    baddie = self->data;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    if (fsa->enteredAnimState != 0) {
        func_800267A4(self);
        fsa->animTickDelta = 0.01f;
        fsa->unk278 = 0.0f;
    }
    if (fsa->unk33A != 0) {
        main_set_bits(baddie->unk39E, 0);
        func_80023D30(self, 4, 0.0f, 0);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B6 = 0;
    }
    return 0;
}

// offset: 0x19F8 | func: 18
static s32 dll_243_anim_state_2(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;
    Lunaimar_ActualData *objdata;

    baddie = self->data;
    objdata = baddie->objdata;
    if (fsa->enteredAnimState != 0) {
        if (fsa->enteredAnimState != 0) {
            func_80023D30(self, (s32) _data_C[rand_next(0, 4)], 0.0f, 0);
            fsa->unk33A = 0;
        }
        func_8002674C(self);
        objdata->unk10 = 4;
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = _data_18[objdata->unk10];
    fsa->unk341 = 1;
    return 0;
}

// offset: 0x1B40 | func: 19
static s32 dll_243_anim_state_3(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
        func_8002674C(self);
        if (fsa->enteredAnimState != 0) {
            func_80023D30(self, rand_next(6, 7), 0.0f, 0);
            fsa->unk33A = 0;
        }
    }
    func_80026128(self, 0xA, 1, -1);
    self->objhitInfo->unk5D = 0xA;
    self->objhitInfo->unk5E = 1;
    func_80028D2C(self);
    fsa->animTickDelta = 0.02f;
    fsa->unk341 = 1;
    return 0;
}

// offset: 0x1C48 | func: 20
static s32 dll_243_anim_state_4(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;

    if (fsa->enteredAnimState != 0) {
        baddie = self->data;
        if (fsa->enteredAnimState != 0) {
            func_80023D30(self, 3, 0.0f, 0);
            fsa->unk33A = 0;
        }
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A83_Lunaimar_Grunt, MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AA5_Heavy_Thwack, MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    return 0;
}

// offset: 0x1D58 | func: 21
static s32 dll_243_anim_state_5(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;

    baddie = self->data;
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.008f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    if (fsa->enteredAnimState != 0) {
        func_80023D30(self, 5, 0.0f, 0);
        fsa->unk33A = 0;
    }
    if (!(fsa->unk34A & 1)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A83_Lunaimar_Grunt, MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AA5_Heavy_Thwack, MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
        gDLL_33_BaddieControl->vtbl->func18(self, (s32) baddie->unk3E0, -1, 0);
    }
    if (!(fsa->unk34A & 2) && (self->animProgress > 0.3f)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
    }
    return 0;
}

// offset: 0x1F54 | func: 22
static s32 dll_243_logic_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        if (fsa->enteredLogicState != 0) {
            fsa->unk27C = 0.0f;
            fsa->unk278 = 0.0f;
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, LUNAIMAR_ASTATE_0);
        }
        if (fsa->unk33A != 0) {
            return LUNAIMAR_LSTATE_5 + 1;
        }
    }

    return 0;
}


// offset: 0x1FE4 | func: 23
static s32 dll_243_logic_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    if (fsa->hitpoints <= 0) {
        return LUNAIMAR_LSTATE_2 + 1;
    }
    if (fsa->unk33A != 0) {
        return LUNAIMAR_LSTATE_5 + 1;
    }
    return 0;
}

// offset: 0x2020 | func: 24
static s32 dll_243_logic_state_2(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, LUNAIMAR_ASTATE_5);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        func_800267A4(self);
        self->unkAF |= 8;
    } else if (fsa->unk33A != 0) {
        if (self->setup == NULL) {
            obj_destroy_object(self);
        }
        return LUNAIMAR_LSTATE_3 + 1;
    }

    return 0;
}

// offset: 0x20EC | func: 25
static s32 dll_243_logic_state_3(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;

    if (fsa->enteredLogicState != 0) {
        baddie = self->data;
        baddie->unk3B4 = 0;
        main_set_bits(baddie->unk39E, 0);
        main_set_bits(baddie->unk39C, 1);
    }
    return 0;
}

// offset: 0x2170 | func: 26
static s32 dll_243_logic_state_4(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, LUNAIMAR_ASTATE_1);
    }
    return 0;
}

// offset: 0x21CC | func: 27
static s32 dll_243_logic_state_5(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie;
    Lunaimar_ActualData *objdata;
    Lunaimar_Setup *setup;

    baddie = self->data;
    objdata = baddie->objdata;
    if (fsa->animState == LUNAIMAR_ASTATE_2) {
        objdata->unk0 -= gUpdateRateF;
        if (objdata->unk0 <= 0.0f) {
            fsa->unk33A = 1;
        }
    }
    if ((fsa->unk33A != 0) || (fsa->enteredLogicState != 0)) {
        if (baddie->unk3B2 & 4) {
            if ((f32) baddie->unk3E2 < fsa->targetDist) {
                return LUNAIMAR_LSTATE_4 + 1;
            }
        } else {
            if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, (f32) baddie->unk3E2, 1) != 0) {
                return LUNAIMAR_LSTATE_4 + 1;
            }
        }
        setup = (Lunaimar_Setup*)self->setup;
        if (rand_next(0, 0x63) < (s32) setup->base.unk2F) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, LUNAIMAR_ASTATE_3);
        } else {
            objdata->unk0 = (f32) rand_next(0xA0, 0x12C);
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, LUNAIMAR_ASTATE_2);
        }
    }

    return 0;
}
