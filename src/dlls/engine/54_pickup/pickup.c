#include "common.h"
#include "dlls/objects/210_player.h"
#include "sys/objtype.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s8 unk4;
    s8 unk5;
    s8 unk6;
    u8 unk7;
    u8 unk8;
} DLL54Unk_Func798;

// offset: 0x0 | ctor
void dll_54_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_54_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_54_func_18(Object* arg0, Object* arg1, s16 arg2) {
    obj_add_object_type(arg0, 0x12);
    arg1->srt.pitch = arg2;
}

// offset: 0x64 | func: 1 | export: 1
#if 0
s8 dll_54_func_64(Object *arg0) {
    Object *sp6C;
    Func_80057F1C_Struct **sp58;
    DLL54Unk_Func798 *temp_s0;
    Func_80057F1C_Struct **var_a1;
    Func_80057F1C_Struct *temp_a0;
    Object *temp_a1;
    Object *var_t2;
    ObjectPolyhits *temp_v0_5;
    ObjectPolyhits *temp_v0_6;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 temp_fv1_2;
    f32 var_fv0;
    f32 var_fv0_2;
    f32 var_fv0_3;
    f32 var_fv0_4;
    f32 var_fv0_5;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a3;
    s8 temp_v1_2;
    s8 temp_v1_3;
    s8 temp_v1_4;
    s8 temp_v1_5;
    s8 temp_v1_6;
    void **var_a1_2;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;

    temp_s0 = arg0->data;
    temp_s0->unk8 = 0;
    temp_s0->unk7 &= 0xFFFE;
    temp_a1 = get_player();
    if (temp_s0->unk5 == 0) {
        temp_s0->unk5 = dll_54_func_838(arg0, (s32) temp_a1, temp_s0);
        if (temp_s0->unk5 != 0) {
            temp_s0->unk6 = 1;
            temp_s0->unk7 |= 1;
        }
        if (arg0->unkE0 == 0) {
            func_800267C4(arg0);
            arg0->unkAF &= 0xFFF7;
            if (!(temp_s0->unk7 & 2)) {
                arg0->speed.f[1] -= 0.1f * gUpdateRateF;
                arg0->srt.transl.f[1] += arg0->speed.f[1] * gUpdateRateF;
            }
            temp_v0 = func_80057F1C(arg0, arg0->srt.transl.f[0], arg0->srt.transl.f[1], arg0->srt.transl.f[2], &sp58, 0, 1);
            var_t2 = NULL;
            var_a3 = 0;
            if (temp_v0 > 0) {
                var_a1 = sp58;
loop_8:
                temp_a0 = *var_a1;
                var_a3 += 1;
                if ((temp_a0->unk14 != 0xE) && (temp_fa0 = arg0->srt.transl.f[1], temp_fv0 = temp_a0->unk0[0], (temp_fa0 < temp_fv0)) && ((temp_fv0 - 40.0f) < temp_fa0)) {
                    var_t2 = temp_a0->unk10;
                    arg0->srt.transl.f[1] = temp_fv0;
                    var_a3 = 0;
                    arg0->speed.f[1] = 0.0f;
                } else {
                    var_a1 += 4;
                    if (var_a3 >= temp_v0) {
                        var_a3 = 0;
                    } else {
                        goto loop_8;
                    }
                }
            }
            temp_v1 = temp_v0 & 3;
            if (temp_v0 > 0) {
                if (temp_v1 != 0) {
                    var_a2 = 0 * 4;
                    do {
                        var_a3 += 1;
                        temp_a0_2 = *(sp58 + var_a2);
                        temp_fv1 = arg0->srt.transl.f[1] - temp_a0_2->unk0;
                        var_fv0 = temp_fv1;
                        if (temp_fv1 < 0.0f) {
                            var_fv0 = -temp_fv1;
                        }
                        if (var_fv0 < 5.0f) {
                            temp_v1_2 = temp_a0_2->unk14;
                            if ((s32) temp_s0->unk8 < temp_v1_2) {
                                temp_s0->unk8 = (u8) temp_v1_2;
                            }
                        }
                        var_a2 += 4;
                    } while (temp_v1 != var_a3);
                    if (var_a3 != temp_v0) {
                        goto block_24;
                    }
                } else {
block_24:
                    var_a2_2 = var_a3 * 4;
                    do {
                        var_a1_2 = sp58 + var_a2_2;
                        temp_a0_3 = *var_a1_2;
                        temp_fv1_2 = arg0->srt.transl.f[1] - temp_a0_3->unk0;
                        var_fv0_2 = temp_fv1_2;
                        if (temp_fv1_2 < 0.0f) {
                            var_fv0_2 = -temp_fv1_2;
                        }
                        if (var_fv0_2 < 5.0f) {
                            temp_v1_3 = temp_a0_3->unk14;
                            if ((s32) temp_s0->unk8 < temp_v1_3) {
                                temp_s0->unk8 = (u8) temp_v1_3;
                                var_a1_2 = sp58 + var_a2_2;
                            }
                        }
                        temp_v0_2 = var_a1_2->unk4;
                        var_fv0_3 = arg0->srt.transl.f[1] - temp_v0_2->unk0;
                        if (var_fv0_3 < 0.0f) {
                            var_fv0_3 = -var_fv0_3;
                        }
                        if (var_fv0_3 < 5.0f) {
                            temp_v1_4 = temp_v0_2->unk14;
                            if ((s32) temp_s0->unk8 < temp_v1_4) {
                                temp_s0->unk8 = (u8) temp_v1_4;
                                var_a1_2 = sp58 + var_a2_2;
                            }
                        }
                        temp_v0_3 = var_a1_2->unk8;
                        var_fv0_4 = arg0->srt.transl.f[1] - temp_v0_3->unk0;
                        if (var_fv0_4 < 0.0f) {
                            var_fv0_4 = -var_fv0_4;
                        }
                        if (var_fv0_4 < 5.0f) {
                            temp_v1_5 = temp_v0_3->unk14;
                            if ((s32) temp_s0->unk8 < temp_v1_5) {
                                temp_s0->unk8 = (u8) temp_v1_5;
                                var_a1_2 = sp58 + var_a2_2;
                            }
                        }
                        temp_v0_4 = var_a1_2->unkC;
                        var_a2_2 += 0x10;
                        var_fv0_5 = arg0->srt.transl.f[1] - temp_v0_4->unk0;
                        if (var_fv0_5 < 0.0f) {
                            var_fv0_5 = -var_fv0_5;
                        }
                        if (var_fv0_5 < 5.0f) {
                            temp_v1_6 = temp_v0_4->unk14;
                            if ((s32) temp_s0->unk8 < temp_v1_6) {
                                temp_s0->unk8 = (u8) temp_v1_6;
                            }
                        }
                    } while (var_a2_2 != (temp_v0 * 4));
                }
            }
            if (var_t2 != NULL) {
                temp_v0_5 = var_t2->polyhits;
                temp_v0_5->unk100[temp_v0_5->unk10F] = arg0;
                temp_v0_6 = var_t2->polyhits;
                temp_v0_6->unk10F += 1;
            }
        }
    } else {
        sp6C = temp_a1;
        func_8002681C(arg0);
        arg0->unkAF |= 8;
        if (joy_get_pressed(0) & 0x8000) {
            joy_set_button_mask(0, 0x8000U);
            if ((temp_s0->unk7 & 4) || (sp6C->dll->vtbl->unkC8(sp6C) == 0)) {
                gDLL_6_AMSFX->vtbl->play_sound(sp6C, 0x912U, 0x7FU, NULL, NULL, 0, NULL);
            } else {
                temp_s0->unk6 = 0;
            }
        }
        if (arg0->unkE0 == 1) {
            temp_s0->unk5 = 2;
        }
        if ((temp_s0->unk5 == 2) && (arg0->unkE0 == 0)) {
            temp_s0->unk5 = 0;
            temp_s0->unk6 = 0;
            if (!(temp_s0->unk7 & 8)) {
                map_save_object(arg0->setup, (s32) arg0->mapID, arg0->srt.transl.f[0], arg0->srt.transl.f[1] + 10.0f, arg0->srt.transl.f[2]);
            }
        }
        if (temp_s0->unk6 != 0) {
            obj_send_mesg(sp6C, 0x100008U, arg0, (void *) ((temp_s0->unk2 << 0x10) | (temp_s0->unk0 & 0xFFFF)));
        }
    }
    return temp_s0->unk5;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/54_pickup/dll_54_func_64.s")
#endif

// offset: 0x5DC | func: 2 | export: 2
int dll_54_func_5DC(Object* obj, s32 arg1) {
    ObjectShadow* shadowData;

    if (obj->def->shadowType == OBJ_SHADOW_GEOM) {
        if (obj->unkB4 == -1) {
            shadowData = obj->shadow;
            shadowData->flags &= ~OBJ_SHADOW_FLAG_FADE_OUT;
        } else {
            shadowData = obj->shadow;
            shadowData->flags |= OBJ_SHADOW_FLAG_FADE_OUT;
        }
    }

    if (obj->unkE0) {
        if (arg1 != -1) {
            return FALSE;
        }
    } else if (arg1 == 0) {
        return FALSE;
    }
    return TRUE;
}

// offset: 0x664 | func: 3 | export: 3
void dll_54_func_664(Object* arg0) {
    obj_free_object_type(arg0, 0x12);
}

// offset: 0x6A0 | func: 4 | export: 4
s8 dll_54_func_6A0(DLL54Unk_Func798* arg0) {
    return arg0->unk5;
}

// offset: 0x6AC | func: 5 | export: 5
s32 dll_54_func_6AC(DLL54Unk_Func798* arg0) {
    return arg0->unk7 & 1;
}

// offset: 0x6C0 | func: 6 | export: 6
u8 dll_54_func_6C0(DLL54Unk_Func798* arg0) {
    return arg0->unk8;
}

// offset: 0x6CC | func: 7 | export: 7
void dll_54_func_6CC(DLL54Unk_Func798* arg0, u8 arg1) {
    u32 temp = arg1;
    if (temp) {
        arg0->unk7 &= ~2;
    } else {
        arg0->unk7 |= 2;
    }
}

// offset: 0x704 | func: 8 | export: 8
void dll_54_func_704(DLL54Unk_Func798* arg0, u8 arg1) {
    u32 temp = arg1;

    if (temp) {
        arg0->unk7 |= 4;
    } else {
        arg0->unk7 &= 0xFFFB;
    };
}

// offset: 0x73C | func: 9 | export: 9
s32 dll_54_func_73C(DLL54Unk_Func798* arg0) {
    if (arg0->unk7 & 4) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x760 | func: 10 | export: 10
void dll_54_func_760(DLL54Unk_Func798 *arg0, u8 arg1){
    u32 temp = arg1;

    if (temp){
        arg0->unk7 |= 8;
    } else {
        arg0->unk7 &= 0xFFF7;
    }
}

// offset: 0x798 | func: 11 | export: 11
void dll_54_func_798(Object* arg0, DLL54Unk_Func798* arg1) {
    Object* player;
    Object* sp20;

    player = get_player();
    arg1->unk5 = 0;

    ((DLL_210_Player*)player->dll)->vtbl->func10(player, &sp20);
    if (sp20 == arg0) {
        ((DLL_210_Player*)player->dll)->vtbl->func11(player, 0);
    }
}

// offset: 0x838 | func: 12
s32 dll_54_func_838(Object* arg0, s32 arg1, DLL54Unk_Func798* arg2) {
    s32 returnVal;

    returnVal = 0;
    if (((arg0->unk78[arg0->unkD4].colourIndex & 0xF) == 6) && (arg0->unkAF & 1) && (arg0->unkE0 == 0)) {
        arg2->unk0 = 0;
        joy_set_button_mask(0, A_BUTTON);
        returnVal = 1;
    }
    return returnVal;
}
