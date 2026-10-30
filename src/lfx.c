#include "sys/lfx.h"

#include "sys/asset.h"
#include "sys/memory.h"
#include "sys/pi.h"
#include "dll.h"

s32 lfxAction(Object* arg0, Object* arg1, u16 actionIndex, s8 arg3, s32 arg4, SRT* arg5) {
    LightAction* lAction;

    lAction = mmAlloc(sizeof(LightAction), -1, ALLOC_NAME("lfx:action1"));
    assetRomLoadSection((void *)lAction, LACTIONS_BIN, actionIndex * sizeof(LightAction), sizeof(LightAction));
    if (lAction) {
        if (arg5) {
            lAction->flags |= 0x40;
            if (lAction->type == LACTION_TYPE_TRKLIGHT) {
                dll_trklight->DoAction(lAction);
            } else if (lAction->type == LACTION_TYPE_SKYLIGHT) {
                dll_skylight->DoAction(lAction);
            } else {
                dll_newlfx->DoAction(NULL, arg1, lAction, arg3, arg4, arg5);
            }
        } else {
            if (lAction->type == LACTION_TYPE_TRKLIGHT) {
                dll_trklight->DoAction(lAction);
            } else if (lAction->type == LACTION_TYPE_SKYLIGHT) {
                dll_skylight->DoAction(lAction);
            } else {
                dll_newlfx->DoAction(arg0, arg1, lAction, arg3, arg4, NULL);
            }
        }
    }
    mmFree(lAction);
    return 0;
}

s32 lfxRestoreAction(Object* arg0, Object* arg1, u16 actionIndex, s8 arg3, s32 arg4, SRT* arg5) {
    LightAction* lAction;

    lAction = mmAlloc(sizeof(LightAction), -1, ALLOC_NAME("lfx:action1"));
    assetRomLoadSection((void *)lAction, LACTIONS_BIN, actionIndex * sizeof(LightAction), sizeof(LightAction));
    if (lAction != NULL) {
        lAction->timer = 12;
        if (arg5 != 0) {
            lAction->flags |= 0x40;
            if (lAction->type == LACTION_TYPE_TRKLIGHT) {
                dll_trklight->DoAction(lAction);
            } else if (lAction->type == LACTION_TYPE_SKYLIGHT) {
                dll_skylight->DoAction(lAction);
            } else {
                dll_newlfx->DoAction(NULL, arg1, lAction, arg3, arg4, arg5);
            }
        } else {
            if (lAction->type == LACTION_TYPE_TRKLIGHT) {
                dll_trklight->DoAction(lAction);
            } else if (lAction->type == LACTION_TYPE_SKYLIGHT) {
                dll_skylight->DoAction(lAction);
            } else {
                dll_newlfx->DoAction(arg0, arg1, lAction, arg3, arg4, NULL);
            }
        }
    }
    mmFree(lAction);
    return 0;
}

