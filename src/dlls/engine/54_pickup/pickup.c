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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/54_pickup/dll_54_func_64.s")

// offset: 0x5DC | func: 2 | export: 2
int dll_54_func_5DC(Object* obj, s32 arg1) {
    ObjectStruct64* shadowData;

    if (obj->def->shadowType == 2) {
        if (obj->unkB4 == -1) {
            shadowData = obj->unk64;
            shadowData->flags &= ~0x1000;
        } else {
            shadowData = obj->unk64;
            shadowData->flags |= 0x1000;
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
