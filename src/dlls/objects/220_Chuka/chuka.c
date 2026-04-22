#include "common.h"
#include "dlls/engine/6_amsfx.h"

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 _unk5;
    u16 unk6;
    u16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 _unkF;
    s16 unk10;
    u16 unk12;
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

static u8 _data_0[16] = {
    0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0
}; 
static u32 _data_10[4] = {SOUND_491, SOUND_492, SOUND_493, SOUND_4B1}; 

static void dll_220_func_8A4(Object* self, DLL220_Data* arg1);
static void dll_220_func_778(Object* self);

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
            dll_220_func_8A4(self, objdata);
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
void dll_220_control(Object* self) {
    DLL220_Data* objdata;
    TextureAnimator* texAnim;
    f32 dx;
    f32 dz;
    u16 distance2D;
    Object* player;
    Object* hitBy;
    f32 pad;
    s32 hitSphereID;
    s32 hitDamage;
    s32 angle;
    Vec3f vPlayer;

    objdata = self->data;
    if (objdata->unk4 & 2) {
        return;
    }

    //Blink behaviour?
    texAnim = func_800348A0(self, 0, 0);
    if (objdata->unk0 < 16.0f) {
        if ((s32)objdata->unk0 == 0xA) {
            objdata->unk4 |= 1;
        }
        texAnim->frame = _data_0[(s32)objdata->unk0] << 8;
        objdata->unk0 += 1.0f;
        if (objdata->unk0 == 16.0f) {
            objdata->unk0 = (f32) rand_next(0x10, 0xF5);
        }
    } else {
        if (gUpdateRateF <= (255.0f - objdata->unk0)) {
            objdata->unk0 += gUpdateRateF;
        } else {
            objdata->unk0 = 0.0f;
        }
        texAnim->frame = 0;
    }
    
    player = get_player();
    dx = player->srt.transl.f[0] - self->srt.transl.f[0];
    dz = player->srt.transl.f[2] - self->srt.transl.f[2];
    distance2D = sqrtf(SQ(dx) + SQ(dz));
    if (distance2D < objdata->unk6) {
        if (objdata->unk12 >= objdata->unk6) {
            objdata->unk4 = 5;
            objdata->unk0 = 0.0f;
        }
        if (objdata->unk4 & 5) {
            //Get vector to player, and angle to player
            VECTOR_SUBTRACT(player->globalPosition, self->globalPosition, vPlayer); 
            angle = arctan2_f(vPlayer.f[0], vPlayer.f[2]) - (self->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(angle);
            if (((u16)angle < objdata->unk8) || (((0xFFFF - objdata->unk8) & 0xFFFF) < (u16)angle)) {
                if ((rand_next(0, 0x63) < objdata->unkD) || (objdata->unk4 & 4)) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, _data_10[2], MAX_VOLUME, NULL, NULL, 0, NULL);
                    dll_220_func_778(self);
                } else {
                    gDLL_6_AMSFX->vtbl->play_sound(self, _data_10[0], MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            } else {
                gDLL_6_AMSFX->vtbl->play_sound(self, _data_10[0], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
    } else if (objdata->unk4 & 1) {
        gDLL_6_AMSFX->vtbl->play_sound(self, _data_10[0], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    objdata->unk12 = distance2D;

    //Check for Projectile Spell attacks
    if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage) == Damage_Type_Projectile) {
        objdata->unkC--; //Lose health
        if (objdata->unkC <= 0) {
            dll_220_func_8A4(self, objdata); //Suggests arg1 of dll_220_func_8A4 might be a DLL220_Data*
            gDLL_33_BaddieControl->vtbl->func18(self, objdata->unkA, -1, 1);
            gDLL_6_AMSFX->vtbl->play_sound(self, _data_10[3], MAX_VOLUME, NULL, NULL, 0, NULL);
            main_set_bits(objdata->unk10, 1);
        }
    }
    objdata->unk4 &= ~5;
}


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
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_492, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x778 | func: 8
static void dll_220_func_778(Object* self) {
    DLL220_Data* objdata;
    Object* sp2C;
    Object* temp_v0_2;
    ObjSetup* temp_v0;
    Object* player;

    objdata = self->data;
    temp_v0 = obj_alloc_setup(0x24, OBJ_ChukaChuck);
    temp_v0->x = self->srt.transl.f[0];
    temp_v0->y = self->srt.transl.f[1];
    temp_v0->z = self->srt.transl.f[2];
    temp_v0->loadFlags = 2;
    temp_v0->byte5 = 4;
    temp_v0->fadeDistance = 0xFF;
    temp_v0_2 = obj_create(temp_v0, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);;
    if (temp_v0_2 != NULL) {
        sp2C = temp_v0_2;
        player = get_player();
        sp2C->velocity.f[0] = (player->srt.transl.f[0] - self->srt.transl.f[0]) / 42.0f;
        sp2C->velocity.f[1] = ((player->srt.transl.f[1] + objdata->unkE) - self->srt.transl.f[1]) / 42.0f;
        sp2C->velocity.f[2] = (player->srt.transl.f[2] - self->srt.transl.f[2]) / 42.0f;
        sp2C->unkC4 = self;
    }
}

// offset: 0x8A4 | func: 9
static void dll_220_func_8A4(Object* self, DLL220_Data* arg1) {
    func_800267A4(self);
    self->srt.flags |= OBJFLAG_INVISIBLE;
    arg1->unk4 |= 2;
}


/*0x0*/ static const char str_0[] = "BADDIE:Chuka Unknown message [%d]\n";
/*0x24*/ static const char str_24[] = "";
/*0x28*/ static const char str_28[] = "";
/*0x2C*/ static const char str_2C[] = "";
