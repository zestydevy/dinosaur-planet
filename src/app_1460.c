#include "common.h"


typedef struct {
    u8  pad0[0x2A];
    u16 unk2A;
    u8  pad2C[0x30];
    u8  unk5C;
} UnknownStruct000;


// Deprecated

// typedef struct {
//     u8 pad0[0x4];
//     void (*unk4)(s32, s32, s32, s32);
// } UnknownStruct001;

// typedef struct {
//     u8 pad0[0x4];
//     void (*unk4)(s32, s32, s32, s32, u16);
// } UnknownStruct002;

// typedef struct {
//   UnknownStruct001* unk0;
// } UnknownStruct003;

// typedef struct {
//   UnknownStruct002* unk0;
// } UnknownStruct004;


s32 func_80000860(s32 arg0, s32 arg1, u16 arg2, s32 arg3) {
    UnknownStruct000 *tmp;

    tmp = malloc(0x60, -1, 0);
    func_800126E0(tmp, 0x49, arg2 * 0x60, 0x60);
    if (tmp != NULL) {
        if ((tmp->unk5C < 3) || (tmp->unk5C == 4)) {
            (*gDLL_newclouds)->unk4.withFourArgs(arg0, arg1, tmp, arg3);
        } else if (tmp->unk5C == 3) {
            (*D_8008C984)->unk4.withFiveArgs(arg0, arg1, tmp, arg3, arg2);
        } else if (tmp->unk5C == 5) {
            (*D_8008C980)->unk4.withFourArgs(arg0, arg1, tmp, arg3);
        } else if (tmp->unk5C == 6) {
            (*gDLL_minic)->unk4.withFiveArgs(arg0, arg1, tmp, arg3, arg2);
        }
    }
    free(tmp);
    return 0;
}

s32 func_800009C8(s32 arg0, s32 arg1, u16 arg2, s32 arg3) {
    UnknownStruct000 *tmp;

    tmp = malloc(0x60, -1, 0);
    func_800126E0(tmp, 0x49, arg2 * 0x60, 0x60);
    if (tmp != NULL) {
        if ((tmp->unk5C < 3) || (tmp->unk5C == 4)) {
            (*gDLL_newclouds)->unk4.withFourArgs(arg0, arg1, tmp, arg3);
        } else if (tmp->unk5C == 3) {
            tmp->unk2A = 0;
            (*D_8008C984)->unk4.withFiveArgs(arg0, arg1, tmp, arg3, arg2);
        } else if (tmp->unk5C == 5) {
            (*D_8008C980)->unk4.withFourArgs(arg0, arg1, tmp, arg3);
        } else if (tmp->unk5C == 6) {
            (*gDLL_minic)->unk4.withFiveArgs(arg0, arg1, tmp, arg3, arg2);
        }
    }
    free(tmp);
    return 0;
}
