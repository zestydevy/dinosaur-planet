#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_800683E0.s")
#else
void _func_800683E0(s16 arg0);
void _func_800683E0(s16 arg0)
{
/* 68FE0 800683E0 27BDFFF8 */  addiu      $sp, $sp, -8
/* 68FE4 800683E4 308400FF */  andi       $a0, $a0, 0xff
a0 = arg0
/* 68FE8 800683E8 3C01800A */  lui        $at, %hi(D_8009B9C8)
/* 68FEC 800683EC C424B9C8 */  lwc1       $f4, %lo(D_8009B9C8)($at)
f4 = D_8009B9C8
/* 68FF0 800683F0 E7A40004 */  swc1       $f4, 4($sp)
sp0004 = D_8009B9C8
/* 68FF4 800683F4 3C013F80 */  lui        $at, 0x3f80
/* 68FF8 800683F8 44813000 */  mtc1       $at, $f6
/* 68FFC 800683FC 00000000 */  nop
f6 = 1.0f
/* 69000 80068400 E7A60000 */  swc1       $f6, ($sp)
sp0000 = 1.0f
/* 69004 80068404 1080000F */  beqz       $a0, .L80068444
/* 69008 80068408 00000000 */   nop
if (arg0 != 0) {
    .L8006840C:
    /* 6900C 8006840C 308E0001 */  andi       $t6, $a0, 1
    t6 = arg0 & 1
    /* 69010 80068410 11C00005 */  beqz       $t6, .L80068428
    /* 69014 80068414 00000000 */   nop
    if (arg0 & 1) != 0) {
        /* 69018 80068418 C7A80000 */  lwc1       $f8, ($sp)
        f8 = sp0000
        /* 6901C 8006841C C7AA0004 */  lwc1       $f10, 4($sp)
        f10 = sp0004
        /* 69020 80068420 460A4402 */  mul.s      $f16, $f8, $f10
        f16 = sp0000 * sp0004
        /* 69024 80068424 E7B00000 */  swc1       $f16, ($sp)
        sp0000 = sp0000 * sp0004

        .L80068428:
    }
    /* 69028 80068428 C7B20004 */  lwc1       $f18, 4($sp)
    f18 = sp0004
    /* 6902C 8006842C 46129102 */  mul.s      $f4, $f18, $f18
    f4 = sp0004 * sp0004
    /* 69030 80068430 E7A40004 */  swc1       $f4, 4($sp)
    sp0004 *= sp0004
    /* 69034 80068434 00042042 */  srl        $a0, $a0, 1
    a0 = arg0 >> 1
    /* 69038 80068438 308400FF */  andi       $a0, $a0, 0xff
    a0 = arg0 & 0xFF
    /* 6903C 8006843C 1480FFF3 */  bnez       $a0, .L8006840C
    /* 69040 80068440 00000000 */   nop
    if (((arg0 >> 1) & 0xFF) == 0) goto L8006840C
    .L80068444:
}
/* 69044 80068444 10000003 */  b          .L80068454
/* 69048 80068448 C7A00000 */   lwc1      $f0, ($sp)
/* 6904C 8006844C 10000001 */  b          .L80068454
/* 69050 80068450 00000000 */   nop
.L80068454:
/* 69054 80068454 03E00008 */  jr         $ra
/* 69058 80068458 27BD0008 */   addiu     $sp, $sp, 8
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_8006845C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_8006863C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_800688B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_80068924.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/amAudio.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_80068E30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_80069378.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_80069394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_68FE0/func_8006939C.s")
