#include "sys/envfx.h"

#include "sys/asset.h"
#include "sys/memory.h"
#include "sys/pi.h"
#include "dll.h"

s32 envfxAction(Object *calledBy, Object *target, u16 actionIndex, s32 arg3) {
    EnvFxAction *envFx;

    envFx = mmAlloc(sizeof(EnvFxAction), COLOUR_TAG_WHITE, ALLOC_NAME("envfx:action1"));
    assetRomLoadSection ((void **)envFx, ENVFXACT_BIN, actionIndex * sizeof(EnvFxAction), sizeof(EnvFxAction));
    if (envFx != NULL) {
        if ((envFx->unk5C < 3) || (envFx->unk5C == 4)) {
            gDLL_9_Newclouds->vtbl->func0(calledBy, target, envFx, arg3);
        } else if (envFx->unk5C == 3) {
            gDLL_8->vtbl->func0(calledBy, target, envFx, arg3, actionIndex);
        } else if (envFx->unk5C == 5)	{
            gDLL_7_Newday->vtbl->func0(calledBy, target, envFx, arg3);
		} else if (envFx->unk5C == 6)	{
            gDLL_12_Minic->vtbl->func0(calledBy, target, envFx, arg3, actionIndex);
        }
    }

    mmFree(envFx);
    return 0;
}

s32 envfxRestoreAction(Object *calledBy, Object *target, u16 actionIndex, s32 arg3) {
    EnvFxAction *envFx;

    envFx = mmAlloc(sizeof(EnvFxAction), COLOUR_TAG_WHITE, ALLOC_NAME("envfx:action1"));
    assetRomLoadSection((void **)envFx, ENVFXACT_BIN, actionIndex * sizeof(EnvFxAction), sizeof(EnvFxAction));
    if (envFx != NULL) {
        if ((envFx->unk5C < 3) || (envFx->unk5C == 4)) {
            gDLL_9_Newclouds->vtbl->func0(calledBy, target, envFx, arg3);
        } else if (envFx->unk5C == 3)	{
            envFx->unk2A = 0;
            gDLL_8->vtbl->func0(calledBy, target, envFx, arg3, actionIndex);
        } else if (envFx->unk5C == 5)	{
            gDLL_7_Newday->vtbl->func0(calledBy, target, envFx, arg3);
        } else if (envFx->unk5C == 6)	{
            gDLL_12_Minic->vtbl->func0(calledBy, target, envFx, arg3, actionIndex);
        }
    }

    mmFree(envFx);
    return 0;
}
