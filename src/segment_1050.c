#include "common.h"
#include "sys/fs.h"

s32 func_80000450(Object* arg0, Object* arg1, u16 arg2, s8 arg3, s32 arg4, s32 arg5) {
    NewLfxStruct* temp_v0;

    temp_v0 = mmAlloc(sizeof(NewLfxStruct), -1, 0);
    queue_load_file_region_to_ptr((void *)temp_v0, LACTIONS_BIN, arg2 * sizeof(NewLfxStruct), sizeof(NewLfxStruct));
    if (temp_v0 != NULL) {
        if (arg5 != 0) {
            temp_v0->unk12.asByte |= 0x40;
            if (temp_v0->unk1b == 2) {
                gDLL_57->vtbl->func1(temp_v0);
            } else if (temp_v0->unk1b == 5) {
                gDLL_58->vtbl->func1(temp_v0);
            } else {
                gDLL_11_Newlfx->vtbl->func0(NULL, arg1, temp_v0, arg3, arg4, arg5);
            }
        } else {
            if (temp_v0->unk1b == 2) {
                gDLL_57->vtbl->func1(temp_v0);
            } else if (temp_v0->unk1b == 5) {
                gDLL_58->vtbl->func1(temp_v0);
            } else {
                gDLL_11_Newlfx->vtbl->func0(arg0, arg1, temp_v0, arg3, arg4, 0);
            }
        }
    }
    mmFree(temp_v0);
    return 0;
}

s32 func_80000608(Object* arg0, Object* arg1, u16 arg2, s8 arg3, s32 arg4, s32 arg5) {
    NewLfxStruct* temp_v0;

    temp_v0 = mmAlloc(sizeof(NewLfxStruct), -1, NULL);
    queue_load_file_region_to_ptr((void *)temp_v0, LACTIONS_BIN, arg2 * sizeof(NewLfxStruct), sizeof(NewLfxStruct));
    if (temp_v0 != NULL) {
        temp_v0->unk0 = 0xC;
        if (arg5 != 0) {
            temp_v0->unk12.asByte |= 0x40;
            if (temp_v0->unk1b == 2) {
                gDLL_57->vtbl->func1(temp_v0);
            } else if (temp_v0->unk1b == 5) {
                gDLL_58->vtbl->func1(temp_v0);
            } else {
                gDLL_11_Newlfx->vtbl->func0(NULL, arg1, temp_v0, arg3, arg4, arg5);
            }
        } else {
            if (temp_v0->unk1b == 2) {
                gDLL_57->vtbl->func1(temp_v0);
            } else if (temp_v0->unk1b == 5) {
                gDLL_58->vtbl->func1(temp_v0);
            } else {
                gDLL_11_Newlfx->vtbl->func0(arg0, arg1, temp_v0, arg3, arg4, 0);
            }
        }
    }
    mmFree(temp_v0);
    return 0;
}

