#include "common.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"
#include "sys/curves.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/315_sidefoodbag.h"

typedef struct {
    UnkCurvesStruct unk0;
    f32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    f32 unk120;
    f32 unk124;
    f32 unk128;
    u32 unk12C;
    s16 unk130;
    s16 _unk132;
    s16 unk134;
    u8 unk136;
    u8 unk137;
    s32 _unk138;
} DLL502_Data;
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    u8 unk1C; 
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
} DLL502_Setup;

/*0x0*/ static s16 _data_0[] = {
    0x0000, 0x0001, 0x0006, 0x0002, 0x0003, 0x0004, 0x0000, 0x0005, 0x0006,0x0007
};
/*0x14*/ static f32 _data_14[] = {
    0.005f, 0.01f, 0.005f, 0.01f, 0.01f, 0.015f, 0.005f, 0.01f, 
    0.005f, 0.012f, 0.0f
};

static s16 dll_502_func_910(Object* arg0, Object* arg1, DLL502_Data* arg2, f32 arg3);
static s16 dll_502_func_C70(Object* arg0, Object* arg1, DLL502_Data* arg2, f32 arg3);
static void dll_502_func_D74(Object* arg0, DLL502_Data* DLL502Data, DLL502_Setup* DLL502Setup); 

// offset: 0x0 | ctor
void dll_502_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_502_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_502_setup(Object* self, DLL502_Setup* setup, s32 arg2) {
    DLL502_Data* DLL502Data;
    UnkFunc_80024108Struct sp48;
    s32 sp44;
    Object* player;
    ObjectShadow* temp_v0_3;
    f32 temp_fv0_3;

    DLL502Data = self->data;
    sp44 = 0x19;
    player = get_player();
    self->unkB0 |= 0x6000;
    if (main_get_bits((s32) setup->unk1A) != 0) {
        DLL502Data->unk136 = 8;
        self->objhitInfo->unk58 &= 0xFFFE;
        self->srt.flags |= 0x4000;
    }
    self->modelInstIdx = setup->unk20;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    temp_v0_3 = self->shadow;
    temp_v0_3->maxDistScale = 2.0f * temp_v0_3->scale;
    temp_v0_3 = self->shadow;
    temp_v0_3->flags |= 0x810;
    DLL502Data->unk110 = (f32) ( setup->unk1D / 255.0f);
    DLL502Data->unk114 = (( setup->unk1C / 255.0f) * 0.2f);
    self->srt.scale = self->def->scale;
    func_80023D30(self, 1, 0.0f, 0U);
    func_80024108(self, 1.0f, 1.0f, &sp48);
    DLL502Data->unk118 = sp48.unk0[0];
    if (DLL502Data->unk118 < 0.0f) {
        DLL502Data->unk118 = (f32) -DLL502Data->unk118;
    }
    DLL502Data->unk118 = (f32) (DLL502Data->unk118 * DLL502Data->unk110);
    DLL502Data->unk118 = (f32) (DLL502Data->unk118 + 20.0f);
    func_80023D30(self, 4, 0.0f, 0U);
    func_80024108(self, 1.0f, 1.0f, &sp48);
    DLL502Data->unk11C = sp48.unk0[2];
    if (DLL502Data->unk11C < 0.0f) {
        DLL502Data->unk11C = (f32) -DLL502Data->unk11C;
    }
    DLL502Data->unk11C = (f32) (DLL502Data->unk11C + 20.0f);
    obj_init_mesg_queue(self, 1U);
    if ((setup->unk18 == 4) || (setup->unk18 == 5)) {
        DLL502Data->unk137 = (u8) (DLL502Data->unk137 | 2);
        gDLL_26_Curves->vtbl->func_4288((UnkCurvesStruct* ) DLL502Data, self, 1000.0f, &sp44, -1);
        self->srt.transl.f[0] = DLL502Data->unk0.unk68.x;
        self->srt.transl.f[2] = DLL502Data->unk0.unk68.z; //unk70
    }
    DLL502Data->unk120 = 5.0f;
    if (player != NULL) {
        temp_fv0_3 = vec3_distance(&player->positionMirror, &self->positionMirror);
        DLL502Data->unk10C = temp_fv0_3;
        DLL502Data->unk108 = temp_fv0_3;
    } else {
        DLL502Data->unk108 = 200.0f;
        DLL502Data->unk10C = 200.0f;
    }
    obj_add_object_type(self, 0x33);
    if (self->modelInstIdx == 0) {
        DLL502Data->unk134 = 0xC1;
    } else {
        DLL502Data->unk134 = 0x66D;
    }
    func_80023D08(self, self->modelInstIdx & 0xFFFF);
}

// offset: 0x3A8 | func: 1 | export: 1
void dll_502_control(Object* self) {
    Object* temp_v0;
    DLL502_Setup* spD8;
    s32 i;
    Object* sidekick;
    Object* spCC;
    f32 var_fv0;
    f32 spC4;
    s32 temp_v0_2;
    Func_80057F1C_Struct** spBC;
    DLL502_Data* temp_s3;
    f32 temp_fv0;
    Object* player;
    u32 spAC;
    Func_80059C40_Struct sp58;
    s32 temp_v0_3;

    temp_s3 = self->data;
    spD8 = (DLL502_Setup*)self->setup;
    player = get_player();
    sidekick = get_sidekick();
    if (temp_s3->unk136 == 8) {
        if (obj_recv_mesg(self, &spAC, NULL, NULL) != 0) {
            do {
                if (spAC == 0x7000B) {
                    if (self->modelInstIdx == 0) {
                        temp_v0 = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0x10); // had two args, second was 0x10?
                        if (((DLL_315_SideFoodbag*)temp_v0->dll)->vtbl->is_obtained(temp_v0) != 0) {
                        ((DLL_315_SideFoodbag*)temp_v0->dll)->vtbl->collect_food(temp_v0, 1);
                        } else {
                        main_increment_bits((s32) temp_s3->unk134);
                        }
                    } else {
                        main_increment_bits((s32) temp_s3->unk134);
                    }
                }
            } while (obj_recv_mesg(self, &spAC, NULL, NULL) != 0);
        }
    } else {
        temp_s3->unk10C = (f32) temp_s3->unk108;
        spC4 = vec3_distance_squared(&player->positionMirror, &self->positionMirror);
        if (sidekick == NULL) {
            temp_s3->unk108 = sqrtf(spC4);
        } else {
            spC4 = spC4;
            temp_fv0 = vec3_distance_squared(&sidekick->positionMirror, &self->positionMirror);
            if (spC4 < temp_fv0) {
                // assign temp_s3->unk108 directly instead of temp-saving it to var_fv0
                temp_s3->unk108 = sqrtf(spC4);
            } else {
                // assign temp_s3->unk108 directly instead of temp-saving it to var_fv0
                temp_s3->unk108 = sqrtf(temp_fv0);
            }
            if (temp_s3->unk108 < spD8->unk1F) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, 1);
            }
        }
        if (func_80025F40(self, &spCC, NULL, NULL) != 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_744, MAX_VOLUME, NULL, NULL, 0, NULL);
            if (spCC->id == 0x416) {
                temp_s3->unk136 = 8U;
                ((DLL_Unknown*)spCC->dll)->vtbl->func[20].withThreeArgs(spCC, 1, self); // func20
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            } else {
                temp_s3->unk137 |= 0x10;
            }
        }
        switch (spD8->unk18) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
        }
        dll_502_func_D74(self, (DLL502_Data* ) temp_s3, (DLL502_Setup* ) spD8); // oh nooo this is a for loop isnt it
        if (temp_s3->unk137 & 8) {
            temp_v0_2 = func_80057F1C(self, self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &spBC, 0, 0);
            for (i = 0; i < temp_v0_2; i++) {
                if (spBC[i]->unk0[0] < (self->srt.transl.f[1] + 10.0f)) {
                    self->srt.transl.f[1] = spBC[i]->unk0[0];
                    break;
                }
            }
            temp_v0_3 = func_80059C40(&self->positionMirror2, &self->srt.transl, 6.0f, 2, &sp58, self, 8, -1, 0xFFU, 0x14);
            if ((spD8->unk18 == 4) && (temp_v0_3 != 0) && (sp58.unk50 == 0xD)) {
                temp_s3->unk137 = (u8) (temp_s3->unk137 | 4);
            }
        }
    }
}

// offset: 0x848 | func: 2 | export: 2
void dll_502_update(Object *self) { }

// offset: 0x854 | func: 3 | export: 3
void dll_502_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x86C | func: 4 | export: 4
void dll_502_free(Object* self, s32 a1) {
    DLL502_Data* DLL502Data;
    u32 temp_a0;

    DLL502Data = self->data;
    obj_free_object_type(self, OBJTYPE_51);
    temp_a0 = DLL502Data->unk12C;
    if (temp_a0 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(temp_a0);
        DLL502Data->unk12C = 0;
    }
}

// offset: 0x8E4 | func: 5 | export: 5
u32 dll_502_get_model_flags(Object* self) {
    DLL502_Setup* dll502Setup = (DLL502_Setup*)self->setup;
    return (dll502Setup->unk20 << 0xB) | 0x400;
}

// offset: 0x8FC | func: 6 | export: 6
u32 dll_502_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL502_Data);
}

// offset: 0x910 | func: 7 
static s16 dll_502_func_910(Object* arg0, Object* arg1, DLL502_Data* arg2, f32 arg3) {
    s16 spB6;
    s32 pad;
    s16 var_s3;
    s16 var_s4; 
    s32 i;
    f32 var_fv1;
    f32 var_fv0;
    f32 sp90[4];
    f32 sp80[4];
    Vec3f sp74;
    s32 pad2;
    s32 pad3;
    s32 pad4;

    var_fv0 = (arg0->srt.transl.f[0] - arg1->srt.transl.f[0]);
    var_fv1 = (arg0->srt.transl.f[2] - arg1->srt.transl.f[2]); 
    spB6 = arctan2_f(-var_fv0, -var_fv1);
    var_fv1 = fsin16_precise(spB6);
    var_fv0 = fcos16_precise(spB6);
    sp74.f[0] = arg0->srt.transl.f[0] - (arg3 * var_fv1);
    sp74.f[1] = arg0->srt.transl.f[1];
    sp74.f[2] = arg0->srt.transl.f[2] - (arg3 * var_fv0);
    if (func_80059C40(&arg0->srt.transl, &sp74, 0.1f, 3, NULL, arg0, 8, -1, 0xFFU, 0) != 0) {
        var_s3 = spB6;
        var_s4 = spB6;
        sp90[2] = sp90[3] = var_fv1;
        sp80[2] = fsin16_precise(0xE38);
        sp80[3] = fsin16_precise(-0xE38);
        sp90[0] = sp90[1] = var_fv0;
        sp80[0] = fcos16_precise(0xE38);
        sp80[1] = fcos16_precise(-0xE38);
        
    for (i = 0; i < 8;  i ++){
        dummy:
        var_fv1 = (sp90[2] * sp80[0]) + (sp90[0] * sp80[2]);
        sp90[0] = (sp90[0] * sp80[0]) - (sp90[2] * sp80[2]);
        sp90[2] = var_fv1; 
        sp74.f[0] = arg0->srt.transl.f[0] - (sp90[2] * arg3);
        sp74.f[2] = arg0->srt.transl.f[2] - (sp90[0] * arg3);
        var_s3 += 0xE38;
        if (func_80059C40(&arg0->srt.transl, &sp74, 0.1f, 1, NULL, arg0, 8, -1, 0xFFU, 0) == 0) {
            return var_s3;
        }
        var_fv1 = (sp90[3] * sp80[1]) + (sp90[1] * sp80[3]);
        sp90[1] = (sp90[1] * sp80[1]) - (sp90[3] * sp80[3]);
        sp90[3] = var_fv1;
        sp74.f[0] = arg0->srt.transl.f[0] - (sp90[3] * arg3);
        sp74.f[2] = arg0->srt.transl.f[2] - (sp90[1] * arg3);
        var_s4 -= 0xE38;
        if (func_80059C40(&arg0->srt.transl, &sp74, 0.1f, 1, NULL, arg0, 8, -1, 0xFFU, 0) == 0) {
            return var_s4;
        }
    }
    }
    return spB6;
}


// offset: 0xC70 | func: 8 
static s16 dll_502_func_C70(Object* arg0, Object* arg1, DLL502_Data* arg2, f32 arg3) {
    f32 temp_fv0;
    f32 temp_fv1;

loop_1:
    temp_fv0 = arg2->unk0.unk68.f[0] - arg0->srt.transl.f[0];
    temp_fv1 = arg2->unk0.unk68.f[2] - arg0->srt.transl.f[2];
    if ((SQ(temp_fv0) + SQ(temp_fv1)) < SQ(arg3)) {
        if ((func_800053B0(&arg2->unk0, arg2->unk120) != 0 || arg2->unk0.unk10 != 0) && gDLL_26_Curves->vtbl->func_4704(&arg2->unk0) != 0) {
        }
        goto loop_1;
    }

    return (s32)arctan2_f(-temp_fv0, -temp_fv1);
}

// offset: 0xD74 | func: 9
static void dll_502_func_D74(Object* arg0, DLL502_Data* DLL502Data, DLL502_Setup* DLL502Setup) { 
    f32 sp84;
    f32 var_fv0;
    f32 var_fv1;
    UnkFunc_80024108Struct sp60;
    SRT sp48;
    Object* sp44;

    sp44 = get_player();
    DLL502Data->unk137 = DLL502Data->unk137 & 0xFFF7;
    if (DLL502Data->unk137 & 4) {
        DLL502Data->unk136 = 6U;
    }
    sp84 = (DLL502Data->unk10C - DLL502Data->unk108) / gUpdateRateF;
    arg0->unkAF |= 8;
    switch (DLL502Data->unk136) {
    case 0:
        if (DLL502Data->unk137 & 0x10) {
            DLL502Data->unk136 = 9U;
        } else {
            if (DLL502Data->unk108 < DLL502Setup->unk19) {
                if (DLL502Data->unk137 & 2) {
                    DLL502Data->unk130 = dll_502_func_C70(arg0, sp44, DLL502Data, DLL502Data->unk118);
                } else {
                    DLL502Data->unk130 = dll_502_func_910(arg0, sp44, DLL502Data, DLL502Data->unk118);
                }
                DLL502Data->unk136 = 1U;
                gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_53C, MAX_VOLUME, NULL, NULL, 0, NULL);
                arg0->srt.yaw = DLL502Data->unk130 - 0x4000;
            } else {
                if (DLL502Data->unk108 < DLL502Setup->unk1F) { 
                    DLL502Data->unk136 = 3U;
                }
            }
        }
        break;
    case 1:
        if (DLL502Data->unk137 & 0x10) {
            DLL502Data->unk136 = 9U;
        } else {
            DLL502Data->unk137 = DLL502Data->unk137 | 8;
            if (DLL502Data->unk137 & 1) {
                DLL502Data->unk136 = 0U;
            }
        }
        break;
    case 3:
    case 7:
        if (DLL502Data->unk137 & 0x10) {
            DLL502Data->unk136 = 9U;
            break;
        } else if (DLL502Data->unk137 & 1) {
            if (DLL502Data->unk136 == 3) {
                DLL502Data->unk136 = 4U;
            } else {
                DLL502Data->unk136 = 0U;
            }
            break;
        } 
    case 4:
        if (DLL502Data->unk137 & 0x10) {
            DLL502Data->unk136 = 9U;
        } else {
            var_fv0 =arg0->srt.transl.f[0] - sp44->srt.transl.f[0];
            var_fv1 = arg0->srt.transl.f[2] - sp44->srt.transl.f[2];
            arg0->srt.yaw = arctan2_f(-(var_fv0),-(var_fv1));
            if ((DLL502Setup->unk1F + 10.0f) < DLL502Data->unk108) {
                DLL502Data->unk136 = 7U;
            } else {
                if (DLL502Data->unk108 < DLL502Setup->unk19) {
                    if (sp84 >= 0.54f) {
                        if (DLL502Data->unk137 & 2) {
                            DLL502Data->unk130 = dll_502_func_C70(arg0, sp44, DLL502Data, DLL502Data->unk118);
                        } else {
                            DLL502Data->unk130 = dll_502_func_910(arg0, sp44, DLL502Data, DLL502Data->unk118);
                        }
                        DLL502Data->unk136 = 1U;
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_53C, MAX_VOLUME, NULL, NULL, 0, NULL);
                        arg0->srt.yaw = DLL502Data->unk130 - 0x4000;
                    } else {
                        if (DLL502Data->unk137 & 2) {
                            DLL502Data->unk130 = dll_502_func_C70(arg0, sp44, DLL502Data, DLL502Data->unk11C);
                        } else {
                            DLL502Data->unk130 = dll_502_func_910(arg0, sp44, DLL502Data, DLL502Data->unk11C);
                        }
                        DLL502Data->unk136 = 5U;
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_53C, MAX_VOLUME, NULL, NULL, 0, NULL);
                        arg0->srt.yaw = DLL502Data->unk130;
                    }
                }
            }
        }
    
        break;
    case 5:
        if ((DLL502Data->unk137 & 0x11) == 0x11) {
            DLL502Data->unk136 = 9U;
        }
        DLL502Data->unk137 = (u8) (DLL502Data->unk137 | 8);
        if (((DLL502Setup->unk19 + 10.0f) < DLL502Data->unk108) && (DLL502Data->unk137 & 1)) {
            DLL502Data->unk136 = 4U;
        } else if (sp84 >= 0.54f) {
            if (DLL502Data->unk137 & 2) {
                DLL502Data->unk130 = dll_502_func_C70(arg0, sp44, DLL502Data, DLL502Data->unk118);
            } else {
                DLL502Data->unk130 = dll_502_func_910(arg0, sp44, DLL502Data, DLL502Data->unk118);
            }
            DLL502Data->unk136 = 1U;
            gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_53C, MAX_VOLUME, NULL, NULL, 0, NULL);
            arg0->srt.yaw = DLL502Data->unk130 - 0x4000;
        }
        break;
    case 2:
        if (DLL502Data->unk137 & 1) {
            DLL502Data->unk136 = 8U;
            arg0->srt.flags |= 0x4000;
            func_800267A4(arg0);
        }
        break;
    case 9:
        if (DLL502Data->unk124 <= 0) {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_745, MAX_VOLUME, &DLL502Data->unk12C, NULL, 0, NULL);
            DLL502Data->unk124 = (f32) rand_next(0xF0, 0x12C);
        }
        DLL502Data->unk124 -= gUpdateRateF;
        if (DLL502Data->unk124 <= 0) {
            gDLL_13_Expgfx->vtbl->func4(arg0);
            DLL502Data->unk136 = 0U;
            DLL502Data->unk137 = (u8) (DLL502Data->unk137 & 0xFFEF);
            if (DLL502Data->unk12C != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(DLL502Data->unk12C);
                DLL502Data->unk12C = 0U;
            }
        } else {
            DLL502Data->unk128 -= gUpdateRateF;
            if (DLL502Data->unk128 <= 0) {
                sp48.transl.f[0] = 10.0f;
                sp48.transl.f[1] = 12.0f;
                gDLL_17_partfx->vtbl->spawn(arg0, PARTICLE_51D, &sp48, PARTFXFLAG_2, -1, NULL);
                DLL502Data->unk128 = 20.0f;
            }
            arg0->unkAF &= 0xFFF7;
        }
        break;
    case 6:
        if (DLL502Data->unk137 & 0x10) {
            DLL502Data->unk136 = 9U;
        }
        arg0->unkAF &= 0xFFF7;
        break;
    }
    if (arg0->unkAF & 1) {
        if ((arg0->modelInstIdx == 0) && (main_get_bits(BIT_911) == 0)) {
            gDLL_3_Animation->vtbl->func30(0x169, NULL, 0);
            main_set_bits(BIT_911, 1U);
        }
        obj_send_mesg(sp44, 0x7000AU, arg0, (void* )0x119);
        if (DLL502Setup->unk1A != -1) {
            main_set_bits((s32) DLL502Setup->unk1A, 1U);
        }
        arg0->unkAF |= 8;
        DLL502Data->unk136 = 2U;
        if (DLL502Data->unk12C != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(DLL502Data->unk12C);
            DLL502Data->unk12C = 0U;
        }
    } 
    if (arg0->curModAnimId != _data_0[DLL502Data->unk136]) {
        func_80023D30(arg0, _data_0[DLL502Data->unk136], 0.25f, 0U);
    }
    if (func_80024108(arg0, _data_14[DLL502Data->unk136], gUpdateRateF, &sp60) != 0) {
        DLL502Data->unk137 = (u8) (DLL502Data->unk137 | 1);
    } else {
        DLL502Data->unk137 = (u8) (DLL502Data->unk137 & 0xFFFE);
    }
   
    if (DLL502Data->unk136 == 1) {
        sp84 = ( sp60.unk0[0] / gUpdateRateF) * DLL502Data->unk110;
    } else if (DLL502Data->unk136 == 5) {
        sp84 = sp60.unk0[2] / gUpdateRateF;
    } else {
        sp84 = 0.0f;
    }
    arg0->speed.f[0] = fsin16_precise(DLL502Data->unk130) * sp84;
    arg0->speed.f[2] = fcos16_precise(DLL502Data->unk130) * sp84;
    obj_integrate_speed(arg0, arg0->speed.f[0] * gUpdateRateF, 0.0f, arg0->speed.f[2] * gUpdateRateF);
}

/*0x0*/ static const char str_0[] = "MUSHROOM: trapped!!!!\n";
/*0x18*/ static const char str_18[] = "MUSHROOM ERROR: no node found\n";
