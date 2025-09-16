#include "common.h"
#include "dll.h"
#include "dlls/objects/210_player.h"

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80031AA0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80031BBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80031DD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80031EBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80031F6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80032170.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_800321E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80032238.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_8003234C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_800323C4.s")

s32 func_80032538(Object* arg0) {
    Object* player;

    if (arg0->unk0xaf & 1 && !gDLL_1_UI->vtbl->func6()) {
        player = get_player();
        if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == -1) {
            set_button_mask(0, 0x8000);
            return 1;
        }
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_800325C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80032690.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_8003273C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_326A0/func_80032804.s")
