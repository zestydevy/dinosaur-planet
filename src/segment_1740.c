#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1740/func_80000B40.s")
#else
void * _func_80000B40(u8 arg0, u8 arg1, s32 arg2, u8 arg3, s32 arg4);
void * _func_80000B40(u8 arg0, u8 arg1, s32 arg2, u8 arg3, s32 arg4)
{
/* 1740 80000B40 27BDFFB0 */  addiu      $sp, $sp, -0x50
/* 1744 80000B44 AFB00018 */  sw         $s0, 0x18($sp)
/* 1748 80000B48 30D000FF */  andi       $s0, $a2, 0xff
/* 174C 80000B4C AFBF001C */  sw         $ra, 0x1c($sp)
/* 1750 80000B50 AFA40050 */  sw         $a0, 0x50($sp)
/* 1754 80000B54 AFA50054 */  sw         $a1, 0x54($sp)
/* 1758 80000B58 AFA60058 */  sw         $a2, 0x58($sp)
/* 175C 80000B5C AFA7005C */  sw         $a3, 0x5c($sp)

void * mem;
s32 sp0034;
s32 sp0038;
s32 sp003c;
s32 sp0040;
s32 i;

/* 1760 80000B60 0C005E69 */  jal        mmAlign16
/* 1764 80000B64 2404001C */   addiu     $a0, $zero, 0x1c
mmAlign16(0x1C)
/* 1768 80000B68 93AE0057 */  lbu        $t6, 0x57($sp)
t6 = arg1
/* 176C 80000B6C 93A9005F */  lbu        $t1, 0x5f($sp)
t1 = arg3
/* 1770 80000B70 3C057F7F */  lui        $a1, 0x7f7f
a1 = 0x7F7F
/* 1774 80000B74 01D00019 */  multu      $t6, $s0
arg1 * arg2
/* 1778 80000B78 0009C840 */  sll        $t9, $t1, 1
t9 = arg3 << 1
/* 177C 80000B7C 00096880 */  sll        $t5, $t1, 2
t5 = arg3 << 2
/* 1780 80000B80 34A57FFF */  ori        $a1, $a1, 0x7fff
a1 = 0x7F7F7FFF
/* 1784 80000B84 00003025 */  or         $a2, $zero, $zero
a2 = 0
/* 1788 80000B88 AFA20034 */  sw         $v0, 0x34($sp)
sp0034 = mmAlign16(0x1C)
/* 178C 80000B8C AFB00028 */  sw         $s0, 0x28($sp)
sp0028 = arg2
/* 1790 80000B90 AFA90024 */  sw         $t1, 0x24($sp)
sp0024 = arg3
/* 1794 80000B94 00007812 */  mflo       $t7
t7 = arg1 * arg2
/* 1798 80000B98 004F5021 */  addu       $t2, $v0, $t7
t2 = (mmAlign16(0x1C)) + (arg1 * arg2)
/* 179C 80000B9C AFAA0038 */  sw         $t2, 0x38($sp)
sp0038 = (mmAlign16(0x1C)) + (arg1 * arg2)
/* 17A0 80000BA0 01300019 */  multu      $t1, $s0
arg3 * arg2
/* 17A4 80000BA4 0000C012 */  mflo       $t8
t8 = arg3 * arg2
/* 17A8 80000BA8 01585821 */  addu       $t3, $t2, $t8
t3 = ((mmAlign16(0x1C)) + (arg1 * arg2)) + (arg3 * arg2)
/* 17AC 80000BAC 01796021 */  addu       $t4, $t3, $t9
t4 = (((mmAlign16(0x1C)) + (arg1 * arg2)) + (arg3 * arg2)) + (arg3 << 1)
/* 17B0 80000BB0 018D2021 */  addu       $a0, $t4, $t5
a0 = ((((mmAlign16(0x1C)) + (arg1 * arg2)) + (arg3 * arg2)) + (arg3 << 1)) + arg3 << 2
/* 17B4 80000BB4 AFA40044 */  sw         $a0, 0x44($sp)
sp0044 = ((((mmAlign16(0x1C)) + (arg1 * arg2)) + (arg3 * arg2)) + (arg3 << 1)) + arg3 << 2
/* 17B8 80000BB8 AFAC0040 */  sw         $t4, 0x40($sp)
sp0040 = t4
/* 17BC 80000BBC 0C005AAA */  jal        malloc
mem = malloc(
    ((((mmAlign16(0x1C)) + (arg1 * arg2)) + (arg3 * arg2)) + (arg3 << 1)) + arg3 << 2,
    0x7F7F7FFF,
    NULL
);
/* 17C0 80000BC0 AFAB003C */   sw        $t3, 0x3c($sp)
sp003c = t3
/* 17C4 80000BC4 8FA80028 */  lw         $t0, 0x28($sp)
t0 = arg2
/* 17C8 80000BC8 8FA90024 */  lw         $t1, 0x24($sp)
t1 = arg3
/* 17D0 80000BD0 00405025 */   or        $t2, $v0, $zero
t2 = mem
/* 17CC 80000BCC 10400052 */  beqz       $v0, .L80000D18
if (mem != NULL) {
    /* 17D4 80000BD4 93AE0053 */  lbu        $t6, 0x53($sp)
    t6 = arg0
    /* 17D8 80000BD8 00003825 */  or         $a3, $zero, $zero
    a3 = 0
    /* 17DC 80000BDC 31260003 */  andi       $a2, $t1, 3
    a2 = (arg3 & 3)
    /* 17E0 80000BE0 A04E0000 */  sb         $t6, ($v0)
    mem->m0000 = arg0
    /* 17E4 80000BE4 93AF0057 */  lbu        $t7, 0x57($sp)
    t7 = arg1
    /* 17E8 80000BE8 A0500002 */  sb         $s0, 2($v0)
    mem->m0002 = arg2
    /* 17EC 80000BEC A04F0001 */  sb         $t7, 1($v0)
    mem->m0001 = arg1
    /* 17F0 80000BF0 93B8005F */  lbu        $t8, 0x5f($sp)
    t8 = arg3
    /* 17F4 80000BF4 A0580003 */  sb         $t8, 3($v0)
    mem->m0003 = arg3
    /* 17F8 80000BF8 8FB90060 */  lw         $t9, 0x60($sp)
    t9 = arg4
    /* 17FC 80000BFC AC590018 */  sw         $t9, 0x18($v0)
    mem->m0018 = arg4
    /* 1800 80000C00 8FAD0034 */  lw         $t5, 0x34($sp)
    t5 = sp0034
    /* 1804 80000C04 01A27021 */  addu       $t6, $t5, $v0
    t6 = sp0034 + (u32)mem
    /* 1808 80000C08 AC4E0008 */  sw         $t6, 8($v0)
    mem->m0008 = sp0034 + (u32)mem
    /* 180C 80000C0C 8FAF0038 */  lw         $t7, 0x38($sp)
    t7 = sp0038
    /* 1810 80000C10 01E2C021 */  addu       $t8, $t7, $v0
    t8 = sp0038 + (u32)mem
    /* 1814 80000C14 AC58000C */  sw         $t8, 0xc($v0)
    mem->m000C = sp0038 + (u32)mem
    /* 1818 80000C18 8FB9003C */  lw         $t9, 0x3c($sp)
    t9 = sp003C
    /* 181C 80000C1C 2418FFFF */  addiu      $t8, $zero, -1
    t8 = -1
    /* 1820 80000C20 8C44000C */  lw         $a0, 0xc($v0)
    a0 = mem->m000C
    /* 1824 80000C24 03226821 */  addu       $t5, $t9, $v0
    t5 = sp003C = (u32)mem
    /* 1828 80000C28 AC4D0010 */  sw         $t5, 0x10($v0)
    mem->m0010 = sp003C = (u32)mem
    /* 182C 80000C2C 8FAE0040 */  lw         $t6, 0x40($sp)
    t6 = sp0040
    /* 1830 80000C30 0308C823 */  subu       $t9, $t8, $t0
    t9 = -1 - arg2
    /* 1834 80000C34 A4590006 */  sh         $t9, 6($v0)
    mem->m0006 = -1 - arg2
    /* 1838 80000C38 01C27821 */  addu       $t7, $t6, $v0
    t7 = sp0040 + (u32)mem
    /* 183C 80000C3C AC4F0014 */  sw         $t7, 0x14($v0)
    mem->m0014 = sp0040 + (u32)mem
    /* 1844 80000C44 A0400004 */   sb        $zero, 4($v0)
    mem->m0004 = 0
    /* 1840 80000C40 19200035 */  blez       $t1, .L80000D18
    if (arg3 > 0) {
        /* 184C 80000C4C 00C04025 */   or        $t0, $a2, $zero
        t0 = arg3 & 3
        /* 1848 80000C48 10C00010 */  beqz       $a2, .L80000C8C
        for (i = 0; i < (arg3 & 3); ++i) {
        /* 1850 80000C50 00002840 */  sll        $a1, $zero, 1
        /* 1854 80000C54 00001880 */  sll        $v1, $zero, 2
        /* 1858 80000C58 2406FFFF */  addiu      $a2, $zero, -1
        .L80000C5C:
        /* 185C 80000C5C 8C4D0010 */  lw         $t5, 0x10($v0)
        t5 = mem->m0010
        /* 1860 80000C60 24E70001 */  addiu      $a3, $a3, 1
        ++a3
        /* 1864 80000C64 01A57021 */  addu       $t6, $t5, $a1
        t6 = &mem->m0010 + 1
        /* 1868 80000C68 A5C60000 */  sh         $a2, ($t6)
        (mem->m0010 + 1) = -1
        /* 186C 80000C6C 8C4F0014 */  lw         $t7, 0x14($v0)
        t7 = mem->m0014
        /* 1870 80000C70 24A50002 */  addiu      $a1, $a1, 2
        a1 += 2
        /* 1874 80000C74 01E3C021 */  addu       $t8, $t7, $v1
        t8 = mem->m0014 + 2
        /* 1878 80000C78 AF040000 */  sw         $a0, ($t8)
        (mem->m0014 + 2) = mem->m000C
        /* 187C 80000C7C 00902021 */  addu       $a0, $a0, $s0
        a0 = mem->m000C + arg2
        /* 1880 80000C80 1507FFF6 */  bne        $t0, $a3, .L80000C5C
        /* 1884 80000C84 24630004 */   addiu     $v1, $v1, 4
        /* 1888 80000C88 10E90023 */  beq        $a3, $t1, .L80000D18
        .L80000C8C:
        }
        /* 188C 80000C8C 2406FFFF */   addiu     $a2, $zero, -1
        /* 1890 80000C90 00072840 */  sll        $a1, $a3, 1
        /* 1894 80000C94 00071880 */  sll        $v1, $a3, 2
        /* 1898 80000C98 00094080 */  sll        $t0, $t1, 2
        .L80000C9C:
        /* 189C 80000C9C 8C590010 */  lw         $t9, 0x10($v0)
        /* 18A0 80000CA0 03256821 */  addu       $t5, $t9, $a1
        /* 18A4 80000CA4 A5A60000 */  sh         $a2, ($t5)
        /* 18A8 80000CA8 8C4E0014 */  lw         $t6, 0x14($v0)
        /* 18AC 80000CAC 01C37821 */  addu       $t7, $t6, $v1
        /* 18B0 80000CB0 ADE40000 */  sw         $a0, ($t7)
        /* 18B4 80000CB4 8C580010 */  lw         $t8, 0x10($v0)
        /* 18B8 80000CB8 00902021 */  addu       $a0, $a0, $s0
        /* 18BC 80000CBC 0305C821 */  addu       $t9, $t8, $a1
        /* 18C0 80000CC0 A7260002 */  sh         $a2, 2($t9)
        /* 18C4 80000CC4 8C4D0014 */  lw         $t5, 0x14($v0)
        /* 18C8 80000CC8 01A37021 */  addu       $t6, $t5, $v1
        /* 18CC 80000CCC ADC40004 */  sw         $a0, 4($t6)
        /* 18D0 80000CD0 8C4F0010 */  lw         $t7, 0x10($v0)
        /* 18D4 80000CD4 00902021 */  addu       $a0, $a0, $s0
        /* 18D8 80000CD8 01E5C021 */  addu       $t8, $t7, $a1
        /* 18DC 80000CDC A7060004 */  sh         $a2, 4($t8)
        /* 18E0 80000CE0 8C590014 */  lw         $t9, 0x14($v0)
        /* 18E4 80000CE4 03236821 */  addu       $t5, $t9, $v1
        /* 18E8 80000CE8 ADA40008 */  sw         $a0, 8($t5)
        /* 18EC 80000CEC 8C4E0010 */  lw         $t6, 0x10($v0)
        /* 18F0 80000CF0 00902021 */  addu       $a0, $a0, $s0
        /* 18F4 80000CF4 01C57821 */  addu       $t7, $t6, $a1
        /* 18F8 80000CF8 A5E60006 */  sh         $a2, 6($t7)
        /* 18FC 80000CFC 8C580014 */  lw         $t8, 0x14($v0)
        /* 1900 80000D00 24A50008 */  addiu      $a1, $a1, 8
        /* 1904 80000D04 0303C821 */  addu       $t9, $t8, $v1
        /* 1908 80000D08 AF24000C */  sw         $a0, 0xc($t9)
        /* 190C 80000D0C 24630010 */  addiu      $v1, $v1, 0x10
        /* 1910 80000D10 1468FFE2 */  bne        $v1, $t0, .L80000C9C
        /* 1914 80000D14 00902021 */   addu      $a0, $a0, $s0
    }
}
.L80000D18:
/* 1918 80000D18 01401025 */  or         $v0, $t2, $zero
/* 191C 80000D1C 8FBF001C */  lw         $ra, 0x1c($sp)
/* 1920 80000D20 8FB00018 */  lw         $s0, 0x18($sp)
/* 1924 80000D24 27BD0050 */  addiu      $sp, $sp, 0x50
/* 1928 80000D28 03E00008 */  jr         $ra
/* 192C 80000D2C 00000000 */   nop
}
#endif

void func_80000D30(void *arg0) {
    free(arg0);
}

typedef struct {
/*0000*/ u16 m0000;
/*0002*/ u8 m0002;
/*0003*/ u8 m0003;
/*0004*/ u8 m0004;
/*0005*/ u8 m0005;
/*0006*/ u16 m0006;
/*0008*/ u32 m0008;
/*000C*/ u32 m000C;
/*0010*/ u16 * m0010;
/*0014*/ u32 * m0014;
} UnkSegment1740Struct;

void func_80000D50(UnkSegment1740Struct * arg0);
void func_80000D50(UnkSegment1740Struct * arg0)
{
    s32 i;
    s16 clear = -1;

    arg0->m0006 = -1 - arg0->m0002;
    arg0->m0004 = 0;

    for (i = 0; i < arg0->m0003; ++i) {
       arg0->m0010[i] = clear;
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_1740/func_80000DA4.s")
#else
s32 _func_80000DA4(void * arg0, s16 arg1)
{
s32 i;
s32 sp0034 = 0;
/* 19A4 80000DA4 27BDFFC8 */  addiu      $sp, $sp, -0x38
/* 19A8 80000DA8 AFB50028 */  sw         $s5, 0x28($sp)
/* 19AC 80000DAC AFB40024 */  sw         $s4, 0x24($sp)
/* 19B0 80000DB0 0080A025 */  or         $s4, $a0, $zero
s4 = arg0
/* 19B4 80000DB4 00A0A825 */  or         $s5, $a1, $zero
s5 = arg1
/* 19B8 80000DB8 AFBF002C */  sw         $ra, 0x2c($sp)
/* 19BC 80000DBC AFB30020 */  sw         $s3, 0x20($sp)
/* 19C0 80000DC0 AFB2001C */  sw         $s2, 0x1c($sp)
/* 19C4 80000DC4 AFB10018 */  sw         $s1, 0x18($sp)
/* 19C8 80000DC8 AFB00014 */  sw         $s0, 0x14($sp)
/* 19CC 80000DCC AFA00034 */  sw         $zero, 0x34($sp)

/* 19D0 80000DD0 828E0004 */  lb         $t6, 4($s4)
t6 = arg0->m0004
/* 19D4 80000DD4 25CFFFFF */  addiu      $t7, $t6, -1
t7 = arg0->m0004 - 1
/* 19D8 80000DD8 A28F0004 */  sb         $t7, 4($s4)
--arg0->m0004
/* 19DC 80000DDC 82880004 */  lb         $t0, 4($s4)
t0 = arg0->m0004
/* 19E4 80000DE4 92870003 */   lbu       $a3, 3($s4)
a3 = arg0->m0003
if (arg0->m0004 < 0) {
    /* 19E0 80000DE0 05030006 */  bgezl      $t0, .L80000DFC

    /* 19E8 80000DE8 92980003 */  lbu        $t8, 3($s4)
    t8 = arg0->m0003
    /* 19EC 80000DEC 2719FFFF */  addiu      $t9, $t8, -1
    t9 = arg0->m0003 - 1
    /* 19F0 80000DF0 A2990004 */  sb         $t9, 4($s4)
    arg0->m0004 = arg0->m0003 - 1
    /* 19F4 80000DF4 82880004 */  lb         $t0, 4($s4)
    t0 = arg0->m0004
    /* 19F8 80000DF8 92870003 */  lbu        $a3, 3($s4)
    a3 = arg0->m0003
.L80000DFC:
}
/* 19FC 80000DFC 00009025 */  or         $s2, $zero, $zero
s2 = 0
/* 1A04 80000E04 8FAE0034 */   lw        $t6, 0x34($sp)
t6 = 0
/* 1A00 80000E00 58E00010 */  blezl      $a3, .L80000E44

/* 1A08 80000E08 8E820010 */  lw         $v0, 0x10($s4)
v0 = arg0->m0010
if (arg0->m0003 > 0) {
    .L80000E0C:
    /* 1A0C 80000E0C 84490000 */  lh         $t1, ($v0)
    t1 = arg0->m0010[i]
    /* 1A14 80000E14 26520001 */   addiu     $s2, $s2, 1
    ++s2
    /* 1A10 80000E10 56A90008 */  bnel       $s5, $t1, .L80000E34
    if (arg1 == arg0->m0010[i]) {
        /* 1A18 80000E18 8E8A0014 */  lw         $t2, 0x14($s4)
        t2 = arg0->m0014
        /* 1A1C 80000E1C 00125880 */  sll        $t3, $s2, 2
        t3 = i * 4
        /* 1A20 80000E20 014B6021 */  addu       $t4, $t2, $t3
        t4 = arg0->m0014 + (i * 4)
        /* 1A24 80000E24 8D8D0000 */  lw         $t5, ($t4)
        t5 = *(arg0->m0014 + (i * 4))
        /* 1A2C 80000E2C AFAD0034 */   sw        $t5, 0x34($sp)
        sp0034 = *(arg0->m0014 + (i * 4))
        /* 1A28 80000E28 10000005 */  b          .L80000E40
        .L80000E34:
    }
    /* 1A34 80000E34 0247082A */  slt        $at, $s2, $a3
    /* 1A38 80000E38 1420FFF4 */  bnez       $at, .L80000E0C
    /* 1A3C 80000E3C 24420002 */   addiu     $v0, $v0, 2
    .L80000E40:
    /* 1A40 80000E40 8FAE0034 */  lw         $t6, 0x34($sp)
    t6 = sp0034
    .L80000E44:
}
/* 1A44 80000E44 02408025 */  or         $s0, $s2, $zero
s0 = i
/* 1A48 80000E48 51C0001F */  beql       $t6, $zero, .L80000EC8
/* 1A4C 80000E4C 8E8E0014 */   lw        $t6, 0x14($s4)

if (sp0034 != 0) {

    /* 1A54 80000E54 26430001 */   addiu     $v1, $s2, 1
    v1 = s2 + 1
    /* 1A50 80000E50 1248001A */  beq        $s2, $t0, .L80000EBC
    if (v1 != arg0->m0004) {
        /* 1A58 80000E58 8E860010 */  lw         $a2, 0x10($s4)
        a2 = arg0->m0010
        .L80000E5C:
        /* 1A5C 80000E5C 00102040 */  sll        $a0, $s0, 1
        a0 = i << 1
        /* 1A64 80000E64 00102880 */   sll       $a1, $s0, 2
        a1 = i << 2
        /* 1A60 80000E60 14670002 */  bne        $v1, $a3, .L80000E6C
        if (v1 == arg0->m0003) {
            /* 1A68 80000E68 00001825 */  or         $v1, $zero, $zero
            v1 = 0
            .L80000E6C:
        }
        /* 1A6C 80000E6C 00037840 */  sll        $t7, $v1, 1
        t7 = v1 << 1
        /* 1A70 80000E70 00CFC021 */  addu       $t8, $a2, $t7
        t8 = arg0->m0010 + (v1 << 1)
        /* 1A74 80000E74 87190000 */  lh         $t9, ($t8)
        t9 = *(arg0->m0010 + (v1 << 1))
        /* 1A78 80000E78 00C44821 */  addu       $t1, $a2, $a0
        t1 = arg0->m0010 + (i << 1)
        /* 1A7C 80000E7C 00035080 */  sll        $t2, $v1, 2
        t2 = v1 << 2
        /* 1A80 80000E80 A5390000 */  sh         $t9, ($t1)
        *(arg0->m0010 + (i << 1)) = *(arg0->m0010 + (v1 << 1))
        /* 1A84 80000E84 8E820014 */  lw         $v0, 0x14($s4)
        v0 = arg0->m0014
        /* 1A88 80000E88 26100001 */  addiu      $s0, $s0, 1
        ++i
        /* 1A8C 80000E8C 24630001 */  addiu      $v1, $v1, 1
        ++v1
        /* 1A90 80000E90 004A5821 */  addu       $t3, $v0, $t2
        t3 = arg0->m0014 + (v1 << 2)
        /* 1A94 80000E94 8D6C0000 */  lw         $t4, ($t3)
        t4 = *(arg0->m0014 + (v1 << 2))
        /* 1A98 80000E98 00456821 */  addu       $t5, $v0, $a1
        t5 = arg0->m0014 + (i << 2)
        /* 1A9C 80000E9C ADAC0000 */  sw         $t4, ($t5)
        *(arg0->m0014 + (i << 2)) = *(arg0->m0014 + (v1 << 2))
        /* 1AA0 80000EA0 92870003 */  lbu        $a3, 3($s4)
        a3 = arg0->m0003
        /* 1AA8 80000EA8 82880004 */   lb        $t0, 4($s4)
        t0 = arg0->m0004
        /* 1AA4 80000EA4 56070003 */  bnel       $s0, $a3, .L80000EB4
        if (i == arg0->m0003) {
            /* 1AAC 80000EAC 00008025 */  or         $s0, $zero, $zero
            i = 0
            /* 1AB0 80000EB0 82880004 */  lb         $t0, 4($s4)
            t0 = arg0->m0004
        .L80000EB4:
        }
        /* 1AB8 80000EB8 8E860010 */   lw        $a2, 0x10($s4)
        a2 = arg0->m0010
        /* 1AB4 80000EB4 5608FFE9 */  bnel       $s0, $t0, .L80000E5C
        if (i != arg0->m0004) goto L80000E5C
        
        .L80000EBC:
    }
    /* 1ABC 80000EBC 1000004D */  b          .L80000FF4
    /* 1AC0 80000EC0 8E860010 */   lw        $a2, 0x10($s4)
    /* 1AC4 80000EC4 8E8E0014 */  lw         $t6, 0x14($s4)
    .L80000EC8:
} else {
/* 1AC8 80000EC8 00087880 */  sll        $t7, $t0, 2
/* 1ACC 80000ECC 01CFC021 */  addu       $t8, $t6, $t7
/* 1AD0 80000ED0 8F190000 */  lw         $t9, ($t8)
/* 1AD4 80000ED4 AFB90034 */  sw         $t9, 0x34($sp)
/* 1AD8 80000ED8 86890006 */  lh         $t1, 6($s4)
/* 1ADC 80000EDC 02A99023 */  subu       $s2, $s5, $t1
/* 1AE0 80000EE0 06420006 */  bltzl      $s2, .L80000EFC
/* 1AE4 80000EE4 928B0001 */   lbu       $t3, 1($s4)
/* 1AE8 80000EE8 928A0001 */  lbu        $t2, 1($s4)
/* 1AEC 80000EEC 024A082A */  slt        $at, $s2, $t2
/* 1AF0 80000EF0 54200036 */  bnel       $at, $zero, .L80000FCC
/* 1AF4 80000EF4 92910002 */   lbu       $s1, 2($s4)
/* 1AF8 80000EF8 928B0001 */  lbu        $t3, 1($s4)
.L80000EFC:
/* 1AFC 80000EFC 000B6043 */  sra        $t4, $t3, 1
/* 1B00 80000F00 02AC9023 */  subu       $s2, $s5, $t4
/* 1B04 80000F04 06430003 */  bgezl      $s2, .L80000F14
/* 1B08 80000F08 8E8D0018 */   lw        $t5, 0x18($s4)
/* 1B0C 80000F0C 00009025 */  or         $s2, $zero, $zero
/* 1B10 80000F10 8E8D0018 */  lw         $t5, 0x18($s4)
.L80000F14:
/* 1B14 80000F14 A6920006 */  sh         $s2, 6($s4)
/* 1B18 80000F18 51A0001E */  beql       $t5, $zero, .L80000F94
/* 1B1C 80000F1C 92910002 */   lbu       $s1, 2($s4)
/* 1B20 80000F20 928E0001 */  lbu        $t6, 1($s4)
/* 1B24 80000F24 00008025 */  or         $s0, $zero, $zero
/* 1B28 80000F28 59C00026 */  blezl      $t6, .L80000FC4
/* 1B2C 80000F2C 868B0006 */   lh        $t3, 6($s4)
/* 1B30 80000F30 8E990018 */  lw         $t9, 0x18($s4)
.L80000F34:
/* 1B34 80000F34 3244FFFF */  andi       $a0, $s2, 0xffff
/* 1B38 80000F38 0320F809 */  jalr       $t9
/* 1B3C 80000F3C 00000000 */   nop
/* 1B40 80000F40 92910002 */  lbu        $s1, 2($s4)
/* 1B44 80000F44 8E8F0008 */  lw         $t7, 8($s4)
/* 1B48 80000F48 00409825 */  or         $s3, $v0, $zero
/* 1B4C 80000F4C 02110019 */  multu      $s0, $s1
/* 1B50 80000F50 00402025 */  or         $a0, $v0, $zero
/* 1B54 80000F54 02203025 */  or         $a2, $s1, $zero
/* 1B58 80000F58 0000C012 */  mflo       $t8
/* 1B5C 80000F5C 01F82821 */  addu       $a1, $t7, $t8
/* 1B60 80000F60 0C01F140 */  jal        bcopy
/* 1B64 80000F64 00000000 */   nop
/* 1B68 80000F68 0C005CBA */  jal        free
/* 1B6C 80000F6C 02602025 */   or        $a0, $s3, $zero
/* 1B70 80000F70 92890001 */  lbu        $t1, 1($s4)
/* 1B74 80000F74 26100001 */  addiu      $s0, $s0, 1
/* 1B78 80000F78 26520001 */  addiu      $s2, $s2, 1
/* 1B7C 80000F7C 0209082A */  slt        $at, $s0, $t1
/* 1B80 80000F80 5420FFEC */  bnel       $at, $zero, .L80000F34
/* 1B84 80000F84 8E990018 */   lw        $t9, 0x18($s4)
/* 1B88 80000F88 1000000E */  b          .L80000FC4
/* 1B8C 80000F8C 868B0006 */   lh        $t3, 6($s4)
/* 1B90 80000F90 92910002 */  lbu        $s1, 2($s4)
.L80000F94:
/* 1B94 80000F94 928A0001 */  lbu        $t2, 1($s4)
/* 1B98 80000F98 8E840008 */  lw         $a0, 8($s4)
/* 1B9C 80000F9C 02320019 */  multu      $s1, $s2
/* 1BA0 80000FA0 92850000 */  lbu        $a1, ($s4)
/* 1BA4 80000FA4 00003012 */  mflo       $a2
/* 1BA8 80000FA8 00000000 */  nop
/* 1BAC 80000FAC 00000000 */  nop
/* 1BB0 80000FB0 01510019 */  multu      $t2, $s1
/* 1BB4 80000FB4 00003812 */  mflo       $a3
/* 1BB8 80000FB8 0C0049B8 */  jal        queue_load_file_region_to_ptr
/* 1BBC 80000FBC 00000000 */   nop
/* 1BC0 80000FC0 868B0006 */  lh         $t3, 6($s4)
.L80000FC4:
/* 1BC4 80000FC4 02AB9023 */  subu       $s2, $s5, $t3
/* 1BC8 80000FC8 92910002 */  lbu        $s1, 2($s4)
.L80000FCC:
/* 1BCC 80000FCC 8E8C0008 */  lw         $t4, 8($s4)
/* 1BD0 80000FD0 8FA50034 */  lw         $a1, 0x34($sp)
/* 1BD4 80000FD4 02510019 */  multu      $s2, $s1
/* 1BD8 80000FD8 02203025 */  or         $a2, $s1, $zero
/* 1BDC 80000FDC 00006812 */  mflo       $t5
/* 1BE0 80000FE0 018D2021 */  addu       $a0, $t4, $t5
/* 1BE4 80000FE4 0C01F140 */  jal        bcopy
/* 1BE8 80000FE8 00000000 */   nop
/* 1BEC 80000FEC 82880004 */  lb         $t0, 4($s4)
/* 1BF0 80000FF0 8E860010 */  lw         $a2, 0x10($s4)
.L80000FF4:
/* 1BF4 80000FF4 00087040 */  sll        $t6, $t0, 1
/* 1BF8 80000FF8 00CEC821 */  addu       $t9, $a2, $t6
/* 1BFC 80000FFC A7350000 */  sh         $s5, ($t9)
/* 1C00 80001000 82890004 */  lb         $t1, 4($s4)
/* 1C04 80001004 8E980014 */  lw         $t8, 0x14($s4)
/* 1C08 80001008 8FAF0034 */  lw         $t7, 0x34($sp)
/* 1C0C 8000100C 00095080 */  sll        $t2, $t1, 2
/* 1C10 80001010 030A5821 */  addu       $t3, $t8, $t2
/* 1C14 80001014 AD6F0000 */  sw         $t7, ($t3)
/* 1C18 80001018 8FA20034 */  lw         $v0, 0x34($sp)
/* 1C1C 8000101C 8FBF002C */  lw         $ra, 0x2c($sp)
/* 1C20 80001020 8FB00014 */  lw         $s0, 0x14($sp)
/* 1C24 80001024 8FB10018 */  lw         $s1, 0x18($sp)
/* 1C28 80001028 8FB2001C */  lw         $s2, 0x1c($sp)
/* 1C2C 8000102C 8FB30020 */  lw         $s3, 0x20($sp)
/* 1C30 80001030 8FB40024 */  lw         $s4, 0x24($sp)
/* 1C34 80001034 8FB50028 */  lw         $s5, 0x28($sp)
/* 1C38 80001038 03E00008 */  jr         $ra
/* 1C3C 8000103C 27BD0038 */   addiu     $sp, $sp, 0x38
}
#endif
