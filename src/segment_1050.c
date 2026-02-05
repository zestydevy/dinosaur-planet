#include "common.h"
#include "sys/fs.h"

s32 func_80000450(Object* arg0, Object* arg1, u16 actionIndex, s8 arg3, s32 arg4, s32 arg5) {
    LightAction* lAction;

    lAction = mmAlloc(sizeof(LightAction), -1, ALLOC_NAME("lfx:action1"));
    queue_load_file_region_to_ptr((void *)lAction, LACTIONS_BIN, actionIndex * sizeof(LightAction), sizeof(LightAction));
    if (lAction) {
        if (arg5) {
            lAction->unk12.asByte |= 0x40;
            if (lAction->unk1b == 2) {
                gDLL_57->vtbl->func1(lAction);
            } else if (lAction->unk1b == 5) {
                gDLL_58->vtbl->func1(lAction);
            } else {
                gDLL_11_Newlfx->vtbl->func0(NULL, arg1, lAction, arg3, arg4, arg5);
            }
        } else {
            if (lAction->unk1b == 2) {
                gDLL_57->vtbl->func1(lAction);
            } else if (lAction->unk1b == 5) {
                gDLL_58->vtbl->func1(lAction);
            } else {
                gDLL_11_Newlfx->vtbl->func0(arg0, arg1, lAction, arg3, arg4, 0);
            }
        }
    }
    mmFree(lAction);
    return 0;
}

s32 func_80000608(Object* arg0, Object* arg1, u16 actionIndex, s8 arg3, s32 arg4, s32 arg5) {
    LightAction* lAction;

    lAction = mmAlloc(sizeof(LightAction), -1, ALLOC_NAME("lfx:action1"));
    queue_load_file_region_to_ptr((void *)lAction, LACTIONS_BIN, actionIndex * sizeof(LightAction), sizeof(LightAction));
    if (lAction != NULL) {
        lAction->unk0 = 0xC;
        if (arg5 != 0) {
            lAction->unk12.asByte |= 0x40;
            if (lAction->unk1b == 2) {
                gDLL_57->vtbl->func1(lAction);
            } else if (lAction->unk1b == 5) {
                gDLL_58->vtbl->func1(lAction);
            } else {
                gDLL_11_Newlfx->vtbl->func0(NULL, arg1, lAction, arg3, arg4, arg5);
            }
        } else {
            if (lAction->unk1b == 2) {
                gDLL_57->vtbl->func1(lAction);
            } else if (lAction->unk1b == 5) {
                gDLL_58->vtbl->func1(lAction);
            } else {
                gDLL_11_Newlfx->vtbl->func0(arg0, arg1, lAction, arg3, arg4, 0);
            }
        }
    }
    mmFree(lAction);
    return 0;
}

