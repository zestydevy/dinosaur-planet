.include "macro.inc"

# assembler directives
.set noreorder # don't insert nops after branches
.set gp=64     # allow use of 64-bit general purpose registers

.section .text, "ax"

.macro .square reg
    mul.s \reg, \reg
.endm

.macro .load_mat4_entry_f reg, mtx, row, col
    l.s \reg, ((\row * 16) + (\col * 4)) (\mtx)
.endm

.macro .store_mat4_entry_f reg, mtx, row, col
    s.s \reg, ((\row * 16) + (\col * 4)) (\mtx)
.endm

.macro .store_mat4_entry reg, mtx, row, col
    sw \reg, ((\row * 16) + (\col * 4)) (\mtx)
.endm


glabel guMtxXFMF
    mtc1       $a1, $f12
    l.s       $f4, ($a0)
    mtc1       $a2, $f14
    l.s       $f8, 0x10($a0)
    mul.s      $f6, $f4, $f12
    mtc1       $a3, $f16
    l.s       $f4, 0x20($a0)
    mul.s      $f10, $f8, $f14
    lw         $t6, 0x10($sp)
    mul.s      $f8, $f4, $f16
    add.s      $f18, $f6, $f10
    l.s       $f10, 0x30($a0)
    add.s      $f6, $f18, $f8
    add.s      $f4, $f10, $f6
    s.s       $f4, ($t6)
    l.s       $f18, 4($a0)
    l.s       $f10, 0x14($a0)
    lw         $t7, 0x14($sp)
    mul.s      $f8, $f18, $f12
    l.s       $f18, 0x24($a0)
    mul.s      $f6, $f10, $f14
    add.s      $f4, $f8, $f6
    mul.s      $f10, $f18, $f16
    l.s       $f6, 0x34($a0)
    add.s      $f8, $f4, $f10
    add.s      $f18, $f6, $f8
    s.s       $f18, ($t7)
    l.s       $f4, 8($a0)
    l.s       $f6, 0x18($a0)
    lw         $t8, 0x18($sp)
    mul.s      $f10, $f4, $f12
    l.s       $f4, 0x28($a0)
    mul.s      $f8, $f6, $f14
    add.s      $f18, $f10, $f8
    mul.s      $f6, $f4, $f16
    l.s       $f8, 0x38($a0)
    add.s      $f10, $f18, $f6
    add.s      $f4, $f8, $f10
    jr         $ra
     s.s      $f4, ($t8)

glabel func_80014F70
    l.s       $f4, ($a1)
    l.s       $f10, ($a0)
    l.s       $f6, 4($a1)
    l.s       $f12, 0x10($a0)
    mul.s      $f10, $f4, $f10
    l.s       $f8, 8($a1)
    l.s       $f14, 0x20($a0)
    mul.s      $f12, $f6, $f12
    add.s      $f12, $f10, $f12
    mul.s      $f14, $f8, $f14
    l.s       $f10, 4($a0)
    mul.s      $f10, $f4, $f10
    add.s      $f16, $f12, $f14
    l.s       $f12, 0x14($a0)
    l.s       $f14, 0x24($a0)
    mul.s      $f12, $f6, $f12
    s.s       $f16, ($a2)
    mul.s      $f14, $f8, $f14
    add.s      $f12, $f10, $f12
    l.s       $f10, 8($a0)
    add.s      $f16, $f12, $f14
    mul.s      $f10, $f4, $f10
    l.s       $f12, 0x18($a0)
    l.s       $f14, 0x28($a0)
    s.s       $f16, 4($a2)
    mul.s      $f12, $f6, $f12
    add.s      $f12, $f10, $f12
    mul.s      $f14, $f8, $f14
    add.s      $f14, $f12, $f14
    jr         $ra
     s.s      $f14, 8($a2)

glabel func_80014FEC
    addiu      $t0, $a0, 0x40
1:
    l.s       $f5, ($a0)
    l.s       $f10, ($a1)
    l.s       $f4, 4($a0)
    l.s       $f12, 0x10($a1)
    mul.s      $f10, $f5, $f10
    l.s       $f6, 8($a0)
    l.s       $f14, 0x20($a1)
    mul.s      $f12, $f4, $f12
    l.s       $f8, 0xc($a0)
    l.s       $f16, 0x30($a1)
    mul.s      $f14, $f6, $f14
    addiu      $a0, 0x10
    addiu      $a2, 0x10
    mul.s      $f16, $f8, $f16
    add.s      $f14, $f12, $f14
    l.s       $f12, 0x14($a1)
    add.s      $f16, $f10, $f16
    l.s       $f10, 4($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x24($a1)
    mul.s      $f12, $f4, $f12
    l.s       $f16, 0x34($a1)
    s.s       $f18, -0x10($a2)
    mul.s      $f14, $f6, $f14
    nop
    mul.s      $f16, $f8, $f16
    add.s      $f14, $f12, $f14
    l.s       $f12, 0x18($a1)
    add.s      $f16, $f10, $f16
    l.s       $f10, 8($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x28($a1)
    mul.s      $f12, $f4, $f12
    l.s       $f16, 0x38($a1)
    s.s       $f18, -0xc($a2)
    mul.s      $f14, $f6, $f14
    nop
    mul.s      $f16, $f8, $f16
    add.s      $f14, $f12, $f14
    l.s       $f12, 0x1c($a1)
    add.s      $f16, $f10, $f16
    l.s       $f10, 0xc($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x2c($a1)
    mul.s      $f12, $f4, $f12
    l.s       $f16, 0x3c($a1)
    s.s       $f18, -8($a2)
    mul.s      $f14, $f6, $f14
    nop
    mul.s      $f16, $f8, $f16
    add.s      $f14, $f12, $f14
    add.s      $f16, $f10, $f16
    add.s      $f18, $f14, $f16
    s.s       $f18, -4($a2)
    bnel       $t0, $a0, 1b
     nop
    jr         $ra
     nop

glabel func_800150E4
    addiu      $t0, $a0, 0x30
1:
    l.s       $f5, ($a0)
    l.s       $f10, ($a1)
    l.s       $f4, 4($a0)
    l.s       $f12, 0x10($a1)
    mul.s      $f10, $f5, $f10
    l.s       $f6, 8($a0)
    l.s       $f14, 0x20($a1)
    mul.s      $f12, $f4, $f12
    addiu      $a2, 0x10
    addiu      $a0, 0x10
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f12
    l.s       $f10, 4($a1)
    l.s       $f12, 0x14($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x24($a1)
    mul.s      $f12, $f4, $f12
    s.s       $f18, -0x10($a2)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f12
    l.s       $f10, 8($a1)
    l.s       $f12, 0x18($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x28($a1)
    mul.s      $f12, $f4, $f12
    s.s       $f18, -0xc($a2)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f12
    add.s      $f18, $f14, $f16
    s.s       $f18, -8($a2)
    bnel       $t0, $a0, 1b
     nop
    l.s       $f5, ($a0)
    l.s       $f10, ($a1)
    l.s       $f4, 4($a0)
    l.s       $f12, 0x10($a1)
    mul.s      $f10, $f5, $f10
    l.s       $f6, 8($a0)
    l.s       $f14, 0x20($a1)
    mul.s      $f12, $f4, $f12
    l.s       $f16, 0x30($a1)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f16
    l.s       $f10, 4($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f14, $f12, $f14
    l.s       $f12, 0x14($a1)
    add.s      $f18, $f14, $f16
    mul.s      $f12, $f4, $f12
    l.s       $f14, 0x24($a1)
    l.s       $f16, 0x34($a1)
    s.s       $f18, ($a2)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f16
    l.s       $f10, 8($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f14, $f12, $f14
    l.s       $f12, 0x18($a1)
    add.s      $f18, $f14, $f16
    mul.s      $f12, $f4, $f12
    l.s       $f14, 0x28($a1)
    l.s       $f16, 0x38($a1)
    s.s       $f18, 4($a2)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f16
    add.s      $f14, $f12, $f14
    add.s      $f18, $f14, $f16
    jr         $ra
     s.s      $f18, 8($a2)

.set reorder
glabel func_80015204
    addiu      $t0, $a0, 0x30
1:
    l.s       $f5, ($a0)
    l.s       $f10, ($a1)
    l.s       $f4, 4($a0)
    l.s       $f12, 0x10($a1)
    mul.s      $f10, $f5, $f10
    l.s       $f6, 8($a0)
    l.s       $f14, 0x20($a1)
    mul.s      $f12, $f4, $f12
    addiu      $a0, 0x10
    addiu      $a2, 0x10
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f12
    l.s       $f10, 4($a1)
    l.s       $f12, 0x14($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x24($a1)
    mul.s      $f12, $f4, $f12
    s.s       $f18, -0x10($a2)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f12
    l.s       $f10, 8($a1)
    l.s       $f12, 0x18($a1)
    mul.s      $f10, $f5, $f10
    add.s      $f18, $f14, $f16
    l.s       $f14, 0x28($a1)
    mul.s      $f12, $f4, $f12
    s.s       $f18, -0xc($a2)
    mul.s      $f14, $f6, $f14
    add.s      $f16, $f10, $f12
    add.s      $f18, $f14, $f16
    s.s       $f18, -8($a2)
    bnel       $t0, $a0, 1b
    jr         $ra

glabel func_80015298
    li.s $f12, 65536.0
    addiu      $t0, $a0, 0x40
1:
    l.s       $f4, ($a0)
    l.s       $f6, 4($a0)
    l.s       $f8, 8($a0)
    mul.s      $f4, $f12
    l.s       $f10, 0xc($a0)
    addiu      $a0, 0x10
    mul.s      $f6, $f12
    addiu      $a1, 8
    mul.s      $f8, $f12
    nop
    mul.s      $f10, $f12
    cvt.w.s    $f4, $f4
    cvt.w.s    $f6, $f6
    mfc1       $t1, $f4
    cvt.w.s    $f8, $f8
    mfc1       $t2, $f6
    sh         $t1, 0x18($a1)
    cvt.w.s    $f10, $f10
    mfc1       $t3, $f8
    sh         $t2, 0x1a($a1)
    srl        $t1, 0x10
    mfc1       $t4, $f10
    sh         $t3, 0x1c($a1)
    srl        $t2, 0x10
    sh         $t4, 0x1e($a1)
    srl        $t3, 0x10
    srl        $t4, 0x10
    sh         $t1, -8($a1)
    sh         $t2, -6($a1)
    sh         $t3, -4($a1)
    sh         $t4, -2($a1)
    bnel       $t0, $a0, 1b
    jr         $ra
.set noreorder

glabel func_80015330
    li.s $f13, 65536.0
    l.s       $f0, ($a0)
    l.s       $f1, 4($a0)
    l.s       $f2, 8($a0)
    mul.s      $f0, $f13
    l.s       $f3, 0x10($a0)
    l.s       $f4, 0x14($a0)
    l.s       $f5, 0x18($a0)
    l.s       $f6, 0x20($a0)
    l.s       $f7, 0x24($a0)
    l.s       $f8, 0x28($a0)
    cvt.w.s    $f12, $f0
    l.s       $f9, 0x30($a0)
    l.s       $f10, 0x34($a0)
    l.s       $f11, 0x38($a0)
    mul.s      $f1, $f13
    mfc1       $t4, $f12
    addiu      $t1, $zero, 1
    sh         $zero, 0x26($a1)
    sh         $t4, 0x20($a1)
    srl        $t4, 0x10
    sh         $t4, ($a1)
    cvt.w.s    $f12, $f1
    sh         $zero, 6($a1)
    sh         $zero, 0x2e($a1)
    sh         $zero, 0xe($a1)
    mul.s      $f2, $f13
    mfc1       $t4, $f12
    sh         $zero, 0x36($a1)
    sh         $zero, 0x16($a1)
    sh         $t4, 0x22($a1)
    srl        $t4, 0x10
    sh         $t4, 2($a1)
    cvt.w.s    $f12, $f2
    sh         $zero, 0x3e($a1)
    sh         $t1, 0x1e($a1)
    mul.s      $f3, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x24($a1)
    srl        $t4, 0x10
    sh         $t4, 4($a1)
    cvt.w.s    $f12, $f3
    mul.s      $f4, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x28($a1)
    srl        $t4, 0x10
    sh         $t4, 8($a1)
    cvt.w.s    $f12, $f4
    mul.s      $f5, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x2a($a1)
    srl        $t4, 0x10
    sh         $t4, 0xa($a1)
    cvt.w.s    $f12, $f5
    mul.s      $f6, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x2c($a1)
    srl        $t4, 0x10
    sh         $t4, 0xc($a1)
    cvt.w.s    $f12, $f6
    mul.s      $f7, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x30($a1)
    srl        $t4, 0x10
    sh         $t4, 0x10($a1)
    cvt.w.s    $f12, $f7
    mul.s      $f8, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x32($a1)
    srl        $t4, 0x10
    sh         $t4, 0x12($a1)
    cvt.w.s    $f12, $f8
    mul.s      $f9, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x34($a1)
    srl        $t4, 0x10
    sh         $t4, 0x14($a1)
    cvt.w.s    $f12, $f9
    mul.s      $f10, $f13
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x38($a1)
    srl        $t4, 0x10
    sh         $t4, 0x18($a1)
    cvt.w.s    $f12, $f10
    mfc1       $t4, $f12
    mul.s      $f11, $f13
    sh         $t4, 0x3a($a1)
    srl        $t4, 0x10
    sh         $t4, 0x1a($a1)
    cvt.w.s    $f12, $f11
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x3c($a1)
    srl        $t4, 0x10
    jr         $ra
     sh        $t4, 0x1c($a1)

glabel func_800154D4
    li.s $f13, 65536.0
    l.s       $f0, 0x30($a0)
    l.s       $f1, 0x34($a0)
    l.s       $f2, 0x38($a0)
    mul.s      $f0, $f13
    ori        $t1, $zero, 1
    sll        $t2, $t1, 0x10
    sw         $zero, 0x20($a1)
    sh         $t1, ($a1)
    sh         $zero, 2($a1)
    sw         $zero, 0x24($a1)
    cvt.w.s    $f12, $f0
    sw         $zero, 4($a1)
    sw         $zero, 0x28($a1)
    sh         $zero, 8($a1)
    mul.s      $f1, $f13
    mfc1       $t4, $f12
    sh         $t1, 0xa($a1)
    sw         $zero, 0x2c($a1)
    sh         $t4, 0x38($a1)
    srl        $t4, 0x10
    sh         $t4, 0x18($a1)
    cvt.w.s    $f12, $f1
    sw         $zero, 0xc($a1)
    sw         $zero, 0x30($a1)
    sw         $zero, 0x10($a1)
    mul.s      $f2, $f13
    mfc1       $t4, $f12
    sw         $zero, 0x34($a1)
    sh         $t1, 0x14($a1)
    sh         $t4, 0x3a($a1)
    srl        $t4, 0x10
    sh         $t4, 0x1a($a1)
    cvt.w.s    $f12, $f2
    sh         $zero, 0x16($a1)
    sh         $zero, 0x3e($a1)
    sh         $t1, 0x1e($a1)
    mfc1       $t4, $f12
    nop
    sh         $t4, 0x3c($a1)
    srl        $t4, 0x10
    jr         $ra
     sh        $t4, 0x1c($a1)

glabel func_80015584
    lui        $at, %hi(D_8008CA84)
    jr         $ra
     sw        $a0, %lo(D_8008CA84)($at)

glabel func_80015590
    lw         $a0, D_8008CA84
    lui        $at, %hi(D_8008CA88)
    jr         $ra
     sw        $a0, %lo(D_8008CA88)($at)

glabel func_800155A4
    lw         $a0, D_8008CA88
    lui        $at, %hi(D_8008CA84)
    jr         $ra
     sw        $a0, %lo(D_8008CA84)($at)

glabel func_800155B8
    lui        $v0, %hi(D_8008CA84)
    jr         $ra
     lw        $v0, %lo(D_8008CA84)($v0)

glabel func_800155C4
    lw         $t0, D_8008CA84
    lui        $at, %hi(D_8008CA84)
    sub        $a1, $a0
    dsll32     $t1, $t0, 0x1f
    dsll       $t2, $t0, 0x1f
    dsrl       $t1, 0x1f
    dsrl32     $t2, 0
    dsll32     $t3, $t0, 0xc
    or         $t1, $t2
    dsrl32     $t3, 0
    xor        $t1, $t3
    dsrl       $t3, $t1, 0x14
    andi       $t3, 0x0fff
    xor        $t0, $t3, $t1
    sw         $t0, %lo(D_8008CA84)($at)
    addi       $a1, 1
    subu       $t0, $a0
    divu       $zero, $t0, $a1
    mflo       $t0
    mfhi       $v0
    add        $v0, $a0
    bnez       $a1, 1f
     nop
    break      7
1:
     jr        $ra
     nop

glabel func_80015630
    lh         $t0, ($a0)
    lh         $t3, ($a1)
    lh         $t1, 2($a0)
    lh         $t4, 2($a1)
    mult       $t0, $t3
    lh         $t2, 4($a0)
    lh         $t5, 4($a1)
    mflo       $t6
    nop
    nop
    mult       $t1, $t4
    mflo       $t7
    add        $t6, $t7
    nop
    mult       $t2, $t5
    mflo       $t8
    add        $t6, $t8
    sra        $t6, 0xc
    mult       $t6, $t3
    mflo       $t3
    sra        $t3, 0xd
    sub        $t3, $t0
    mult       $t6, $t4
    sh         $t3, 6($a0)
    mflo       $t4
    sra        $t4, 0xd
    sub        $t4, $t1
    mult       $t6, $t5
    sh         $t4, 8($a0)
    mflo       $t5
    sra        $t5, 0xd
    sub        $t5, $t0
    jr         $ra
     sh        $t5, 0xa($a0)

glabel func_800156B8
    addiu      $sp, -8
    lui        $at, 0x3780
    sd         $ra, ($sp)
    or         $a3, $a0, $zero
    mtc1       $at, $f18
    jal        func_8001617C
     lh        $a0, ($a1)
    mtc1       $v0, $f0
    lh         $a0, ($a1)
    cvt.s.w    $f0, $f0
    mul.s      $f0, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f2
    lh         $a0, 2($a1)
    cvt.s.w    $f2, $f2
    mul.s      $f2, $f18
    jal        func_8001617C
     nop
    mtc1       $v0, $f4
    lh         $a0, 2($a1)
    cvt.s.w    $f4, $f4
    mul.s      $f4, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f6
    lh         $a0, 4($a1)
    cvt.s.w    $f6, $f6
    mul.s      $f6, $f18
    jal        func_8001617C
     nop
    mtc1       $v0, $f8
    lh         $a0, 4($a1)
    cvt.s.w    $f8, $f8
    mul.s      $f8, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f10
    lw         $t2, 8($a1)
    sw         $zero, 0xc($a3)
    cvt.s.w    $f10, $f10
    sw         $zero, 0x1c($a3)
    sw         $zero, 0x2c($a3)
    lui        $at, 0x3f80
    mul.s      $f10, $f18
    nop
    mul.s      $f16, $f4, $f0
    nop
    mul.s      $f16, $f8
    nop
    mul.s      $f18, $f10, $f2
    add.s      $f16, $f18
    mtc1       $t2, $f18
    nop
    mul.s      $f16, $f18
    s.s       $f16, ($a3)
    mul.s      $f16, $f8, $f6
    nop
    mul.s      $f16, $f18
    s.s       $f16, 4($a3)
    mul.s      $f16, $f4, $f2
    nop
    mul.s      $f16, $f8
    nop
    mul.s      $f18, $f10, $f0
    sub.s      $f16, $f18
    mtc1       $t2, $f18
    nop
    mul.s      $f16, $f18
    s.s       $f16, 8($a3)
    mul.s      $f16, $f4, $f0
    nop
    mul.s      $f16, $f10
    nop
    mul.s      $f18, $f8, $f2
    sub.s      $f16, $f18
    mtc1       $t2, $f18
    nop
    mul.s      $f16, $f18
    s.s       $f16, 0x10($a3)
    mul.s      $f16, $f10, $f6
    nop
    mul.s      $f16, $f18
    s.s       $f16, 0x14($a3)
    mul.s      $f16, $f4, $f2
    nop
    mul.s      $f16, $f10
    nop
    mul.s      $f18, $f8, $f0
    add.s      $f16, $f18
    mtc1       $t2, $f18
    nop
    mul.s      $f16, $f18
    s.s       $f16, 0x18($a3)
    mul.s      $f16, $f6, $f0
    nop
    mul.s      $f16, $f18
    s.s       $f16, 0x20($a3)
    mul.s      $f16, $f4, $f18
    neg.s      $f16
    s.s       $f16, 0x24($a3)
    mul.s      $f16, $f6, $f2
    nop
    mul.s      $f16, $f18
    mtc1       $at, $f18
    s.s       $f16, 0x28($a3)
    lw         $t0, 0xc($a1)
    sw         $t0, 0x30($a3)
    lw         $t0, 0x10($a1)
    sw         $t0, 0x34($a3)
    lw         $t0, 0x14($a1)
    s.s       $f18, 0x3c($a3)
    sw         $t0, 0x38($a3)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel func_8001588C
    mtc1       $a1, $f18
    l.s       $f16, 0x10($a0)
    mul.s      $f16, $f18
    s.s       $f16, 0x10($a0)
    l.s       $f16, 0x14($a0)
    mul.s      $f16, $f18
    s.s       $f16, 0x14($a0)
    l.s       $f16, 0x18($a0)
    mul.s      $f16, $f18
    jr         $ra
     s.s      $f16, 0x18($a0)

glabel func_800158B8
    mtc1       $a1, $f16
    l.s       $f0, 0x10($a0)
    l.s       $f4, 0x14($a0)
    l.s       $f8, 0x18($a0)
    mul.s      $f0, $f16
    l.s       $f2, 0x30($a0)
    l.s       $f6, 0x34($a0)
    mul.s      $f4, $f16
    l.s       $f10, 0x38($a0)
    mul.s      $f8, $f16
    add.s      $f0, $f2
    add.s      $f4, $f6
    s.s       $f0, 0x30($a0)
    add.s      $f8, $f10
    s.s       $f4, 0x34($a0)
    jr         $ra
     s.s      $f8, 0x38($a0)

glabel func_800158FC
    addiu      $sp, -8
    lui        $at, 0x3780
    sd         $ra, ($sp)
    or         $a3, $a0, $zero
    mtc1       $at, $f18
    jal        func_8001617C
     lh        $a0, ($a1)
    mtc1       $v0, $f0
    lh         $a0, ($a1)
    cvt.s.w    $f0, $f0
    mul.s      $f0, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f2
    lh         $a0, 2($a1)
    cvt.s.w    $f2, $f2
    mul.s      $f2, $f18
    jal        func_8001617C
     nop
    mtc1       $v0, $f4
    lh         $a0, 2($a1)
    cvt.s.w    $f4, $f4
    mul.s      $f4, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f6
    lh         $a0, 4($a1)
    cvt.s.w    $f6, $f6
    mul.s      $f6, $f18
    jal        func_8001617C
     nop
    mtc1       $v0, $f8
    lh         $a0, 4($a1)
    cvt.s.w    $f8, $f8
    mul.s      $f8, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f10
    sw         $zero, 0xc($a3)
    s.s       $f4, 0x18($a3)
    cvt.s.w    $f10, $f10
    sw         $zero, 0x1c($a3)
    sw         $zero, 0x2c($a3)
    li.s        $at, 1.0
    mul.s      $f10, $f18
    nop
    mul.s      $f16, $f4, $f8
    nop
    mul.s      $f16, $f0
    nop
    mul.s      $f18, $f2, $f10
    sub.s      $f16, $f18, $f16
    s.s       $f16, ($a3)
    mul.s      $f16, $f4, $f10
    nop
    mul.s      $f16, $f0
    nop
    mul.s      $f18, $f2, $f8
    add.s      $f16, $f18
    s.s       $f16, 4($a3)
    mul.s      $f16, $f0, $f6
    neg.s      $f16
    s.s       $f16, 8($a3)
    mul.s      $f16, $f6, $f8
    neg.s      $f16
    s.s       $f16, 0x10($a3)
    mul.s      $f16, $f6, $f10
    s.s       $f16, 0x14($a3)
    mul.s      $f16, $f4, $f8
    nop
    mul.s      $f16, $f2
    nop
    mul.s      $f18, $f0, $f10
    add.s      $f16, $f18
    s.s       $f16, 0x20($a3)
    mul.s      $f16, $f4, $f10
    nop
    mul.s      $f16, $f2
    nop
    mul.s      $f18, $f0, $f8
    sub.s      $f16, $f18, $f16
    l.s       $f18, ($a3)
    s.s       $f16, 0x24($a3)
    mul.s      $f16, $f2, $f6
    s.s       $f16, 0x28($a3)
    l.s       $f0, 0xc($a1)
    l.s       $f2, 0x10($a1)
    l.s       $f16, 0x10($a3)
    mul.s      $f18, $f0
    l.s       $f4, 0x14($a1)
    mul.s      $f16, $f2
    add.s      $f16, $f18
    l.s       $f18, 0x20($a3)
    mul.s      $f18, $f4
    add.s      $f16, $f18
    l.s       $f18, 4($a3)
    s.s       $f16, 0x30($a3)
    mul.s      $f18, $f0
    l.s       $f16, 0x14($a3)
    mul.s      $f16, $f2
    add.s      $f16, $f18
    l.s       $f18, 0x24($a3)
    mul.s      $f18, $f4
    add.s      $f16, $f18
    l.s       $f18, 8($a3)
    s.s       $f16, 0x34($a3)
    mul.s      $f18, $f0
    l.s       $f16, 0x18($a3)
    mul.s      $f16, $f2
    add.s      $f16, $f18
    l.s       $f18, 0x28($a3)
    mul.s      $f18, $f4
    add.s      $f16, $f18
    s.s       $f16, 0x38($a3)
    mtc1       $at, $f16
    nop
    s.s       $f16, 0x3c($a3)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel func_80015AE0
    addiu      $sp, -8
    lui        $at, 0x3780
    sd         $ra, ($sp)
    or         $a3, $a0, $zero
    mtc1       $at, $f18
    jal        func_8001617C
     lh        $a0, ($a1)
    mtc1       $v0, $f0
    lh         $a0, ($a1)
    cvt.s.w    $f0, $f0
    mul.s      $f0, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f2
    lh         $a0, 2($a1)
    cvt.s.w    $f2, $f2
    mul.s      $f2, $f18
    jal        func_8001617C
     nop
    mtc1       $v0, $f4
    lh         $a0, 2($a1)
    cvt.s.w    $f4, $f4
    mul.s      $f4, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f6
    lh         $a0, 4($a1)
    cvt.s.w    $f6, $f6
    mul.s      $f6, $f18
    jal        func_8001617C
     nop
    mtc1       $v0, $f8
    lh         $a0, 4($a1)
    cvt.s.w    $f8, $f8
    mul.s      $f8, $f18
    jal        func_80016178
     nop
    mtc1       $v0, $f10
    lui        $at, 0x3f80
    sw         $zero, 0xc($a3)
    cvt.s.w    $f10, $f10
    sw         $zero, 0x1c($a3)
    sw         $zero, 0x2c($a3)
    mul.s      $f10, $f18
    mtc1       $at, $f18
    mul.s      $f12, $f0, $f6
    s.s       $f18, 0x3c($a3)
    ld         $ra, ($sp)
    addiu      $sp, 8
    mul.s      $f14, $f2, $f10
    nop
    mul.s      $f16, $f0, $f8
    jr         $ra
     nop

glabel func_80015BB8
    addiu      $sp, -8
    sd         $ra, ($sp)
    jal        some_getter_from_float_array
     nop
    jal        func_800162F4
     mov.s     $f1, $f0
    l.s       $f2, ($a1)
    l.s       $f5, 0x20($a1)
    l.s       $f3, 4($a1)
    mul.s      $f8, $f2, $f0
    l.s       $f6, 0x24($a1)
    l.s       $f4, 8($a1)
    mul.s      $f9, $f5, $f1
    l.s       $f7, 0x28($a1)
    mul.s      $f10, $f3, $f0
    nop
    mul.s      $f11, $f6, $f1
    nop
    mul.s      $f12, $f4, $f0
    nop
    mul.s      $f13, $f7, $f1
    sub.s      $f10, $f11
    mul.s      $f5, $f0
    nop
    mul.s      $f2, $f1
    s.s       $f10, 4($a1)
    sub.s      $f12, $f13
    mul.s      $f6, $f0
    s.s       $f12, 8($a1)
    mul.s      $f3, $f1
    nop
    mul.s      $f7, $f0
    nop
    mul.s      $f4, $f1
    sub.s      $f8, $f9
    add.s      $f5, $f2
    add.s      $f6, $f3
    s.s       $f5, 0x20($a1)
    add.s      $f7, $f4
    s.s       $f6, 0x24($a1)
    s.s       $f8, ($a1)
    s.s       $f7, 0x28($a1)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel func_80015C70
    addiu      $sp, -8
    sd         $ra, ($sp)
    or         $a2, $a0, $zero
    lh         $t3, ($a1)
    lh         $t4, 2($a1)
    lh         $t5, 4($a1)
    jal        func_8001617C
     lh        $a0, ($a2)
    or         $t6, $v0, $zero
    jal        func_80016178
     lh        $a0, ($a2)
    mult       $t3, $t6
    or         $t7, $v0, $zero
    lh         $a0, 2($a2)
    mflo       $t0
    nop
    nop
    mult       $t4, $t6
    mflo       $t1
    nop
    nop
    mult       $t3, $t7
    mflo       $t3
    sub        $t3, $t1
    sra        $t3, $t3, 0x10
    mult       $t4, $t7
    mflo       $t4
    add        $t4, $t0
    jal        func_8001617C
     sra       $t4, $t4, 0x10
    or         $t6, $v0, $zero
    jal        func_80016178
     lh        $a0, 2($a2)
    mult       $t4, $t6
    or         $t7, $v0, $zero
    lh         $a0, 4($a2)
    mflo       $t0
    nop
    nop
    mult       $t5, $t6
    mflo       $t1
    nop
    nop
    mult       $t4, $t7
    mflo       $t4
    sub        $t4, $t1
    sra        $t4, $t4, 0x10
    mult       $t5, $t7
    mflo       $t5
    add        $t5, $t0
    jal        func_8001617C
     sra       $t5, $t5, 0x10
    or         $t6, $v0, $zero
    jal        func_80016178
     lh        $a0, 4($a2)
    mult       $t3, $t6
    or         $t7, $v0, $zero
    sh         $t4, 2($a1)
    mflo       $t0
    nop
    nop
    mult       $t5, $t6
    mflo       $t1
    nop
    nop
    mult       $t3, $t7
    mflo       $t3
    add        $t3, $t1
    sra        $t3, $t3, 0x10
    mult       $t5, $t7
    sh         $t3, ($a1)
    mflo       $t5
    sub        $t5, $t0
    sra        $t5, $t5, 0x10
    sh         $t5, 4($a1)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel func_80015DAC
    addiu      $sp, -8
    sd         $ra, ($sp)
    or         $a2, $a0, $zero
    l.s       $f4, ($a1)
    l.s       $f6, 4($a1)
    l.s       $f8, 8($a1)
    jal        func_80016110
     lh        $a0, 4($a2)
    mul.s      $f10, $f4, $f0
    lh         $a0, 4($a2)
    mul.s      $f12, $f6, $f0
    jal        func_80016144
     nop
    mul.s      $f4, $f0
    lh         $a0, 2($a2)
    mul.s      $f6, $f0
    sub.s      $f4, $f12
    jal        func_80016110
     add.s     $f6, $f10
    mul.s      $f10, $f6, $f0
    lh         $a0, 2($a2)
    mul.s      $f12, $f8, $f0
    jal        func_80016144
     nop
    mul.s      $f6, $f0
    lh         $a0, ($a2)
    mul.s      $f8, $f0
    sub.s      $f6, $f12
    jal        func_80016110
     add.s     $f8, $f10
    mul.s      $f10, $f4, $f0
    lh         $a0, ($a2)
    mul.s      $f12, $f8, $f0
    jal        func_80016144
     nop
    mul.s      $f4, $f0
    s.s       $f6, 4($a1)
    mul.s      $f8, $f0
    add.s      $f4, $f12
    sub.s      $f8, $f10
    s.s       $f4, ($a1)
    s.s       $f8, 8($a1)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel func_80015E64
    addiu      $sp, -8
    sd         $ra, ($sp)
    or         $a2, $a0, $zero
    l.s       $f4, 0($a1)
    l.s       $f6, 4($a1)
    l.s       $f8, 8($a1)
    jal        func_80016110
     lh        $a0, ($a2)
    mul.s      $f10, $f4, $f0
    lh         $a0, ($a2)
    mul.s      $f12, $f8, $f0
    jal        func_80016144
     nop
    mul.s      $f4, $f0
    lh         $a0, 2($a2)
    mul.s      $f8, $f0
    add.s      $f4, $f12
    jal        func_80016110
     sub.s     $f8, $f10
    mul.s      $f10, $f6, $f0
    lh         $a0, 2($a2)
    mul.s      $f12, $f8, $f0
    jal        func_80016144
     nop
    mul.s      $f6, $f0
    lh         $a0, 4($a2)
    mul.s      $f8, $f0
    sub.s      $f6, $f12
    jal        func_80016110
     add.s     $f8, $f10
    mul.s      $f10, $f4, $f0
    lh         $a0, 4($a2)
    mul.s      $f12, $f6, $f0
    jal        func_80016144
     nop
    mul.s      $f4, $f0
    s.s       $f8, 8($a1)
    mul.s      $f6, $f0
    sub.s      $f4, $f12
    add.s      $f6, $f10
    s.s       $f4, ($a1)
    s.s       $f6, 4($a1)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel func_80015F1C
    addiu      $sp, -8
    sd         $ra, ($sp)
    or         $a2, $a0, $zero
    l.s       $f8, 8($a1)
    jal        func_80016110
     lh        $a0, 2($a2)
    mul.s      $f6, $f8, $f0
    jal        func_80016144
     lh        $a0, 2($a2)
    mul.s      $f8, $f0
    neg.s      $f6
    jal        func_80016110
     lh        $a0, ($a2)
    mul.s      $f4, $f8, $f0
    jal        func_80016144
     lh        $a0, ($a2)
    mul.s      $f8, $f0
    s.s       $f4, ($a1)
    s.s       $f6, 4($a1)
    s.s       $f8, 8($a1)
    ld         $ra, ($sp)
    addiu      $sp, 8
    jr         $ra
     nop

glabel mat4_ident_translate
    move         $t0, $a0
    addiu      $t1, $t0, 0x40
1:
    addiu      $t0, 4
    bne        $t1, $t0, 1b
     sw        $zero, -4($t0)

    li.s $f18, 1.0 
    nop

    .store_mat4_entry_f $f18, $a0, 0, 0
    .store_mat4_entry_f $f18, $a0, 1, 1
    .store_mat4_entry_f $f18, $a0, 2, 2
    .store_mat4_entry_f $f18, $a0, 3, 3

    .store_mat4_entry $a1, $a0, 3, 0
    .store_mat4_entry $a2, $a0, 3, 1
    jr         $ra
    .store_mat4_entry $a3, $a0, 3, 2

glabel func_80015FBC
    lui        $v0, %hi(D_8008CA40)
    jr         $ra
     addiu     $v0, %lo(D_8008CA40)

glabel func_80015FC8
    or         $t0, $a0, $zero
    addiu      $t1, $t0, 0x40
1:
    addiu      $t0, 4
    bne        $t1, $t0, 1b
     sw        $zero, -4($t0)
    li.s $f18, 1.0
    nop
    s.s       $f18, 0x3c($a0)
    sw         $a1, ($a0)
    sw         $a2, 0x14($a0)
    jr         $ra
     sw        $a3, 0x28($a0)

glabel func_80015FFC
    or         $t0, $a0, $a1
    bne        $zero, $t0, 1f
     nop
    jr         $ra
     addiu     $v0, $zero, 0
1:
    bltz       $a0, 2f
     nop
    bltzl      $a1, 4f
     negu      $a1
    j          6f
     addiu     $v0, $zero, 0
2:
    bltz       $a1, 3f
     negu      $a0
    j          5f
     ori       $v0, $zero, 0xc000
3:
    negu       $a1
    j          6f
     ori       $v0, $zero, 0x8000
    negu       $a1
4:
    addiu      $v0, $zero, 0x4000
5:
    xor        $a0, $a1
    xor        $a1, $a0, $a1
    xor        $a0, $a1
6:
    subu       $t0, $a0, $a1
    bltzl      $t0, 8f
     dsll      $t0, $a0, 0xb
    dsll       $t0, $a1, 0xb
    ddivu      $zero, $t0, $a0
    lui        $t1, %hi(D_8008E294)
    addiu      $t1, %lo(D_8008E294)
    addiu      $v0, 0x4000
    bnez       $a0, 7f
     nop
    break      7
7:
     mflo      $t0
    mflo       $t0
    andi       $t0, 0xffe
    addu       $t1, $t0
    lh         $t0, ($t1)
    subu       $v0, $t0
    jr         $ra
     andi      $v0, 0xffff
    dsll       $t0, $a0, 0xb
8:
    ddivu      $zero, $t0, $a1
    lui        $t1, %hi(D_8008E294)
    addiu      $t1, %lo(D_8008E294)
    bnez       $a1, 9f
     nop
    break      7
9:
     mflo      $t0
    mflo       $t0
    andi       $t0, 0xffe
    addu       $t1, $t0
    lh         $t0, ($t1)
    addu       $v0, $t0
    jr         $ra
     andi      $v0, 0xffff

glabel round_2_floats
    li.s $f0, 255.0
    nop
    mul.s      $f12, $f0
    nop
    mul.s      $f14, $f0
    cvt.w.s    $f12, $f12
    cvt.w.s    $f14, $f14
    mfc1       $a0, $f12
    mfc1       $a1, $f14
    j          func_80015FFC
     nop

glabel func_80016110
    addiu      $sp, -8
    sd         $ra, ($sp)
    jal        func_8001617C
     nop
    mtc1       $v0, $f0
    li.s $f2, 0.0000152587890625
    cvt.s.w    $f0, $f0
    ld         $ra, ($sp)
    addiu      $sp, 8
    mul.s      $f0, $f2
    jr         $ra
     nop

glabel func_80016144
    addiu      $sp, -8
    sd         $ra, ($sp)
    jal        func_80016178
     nop
    mtc1       $v0, $f0
    li.s        $f2, 0.0000152587890625
    cvt.s.w    $f0, $f0
    ld         $ra, ($sp)
    addiu      $sp, 8
    mul.s      $f0, $f2
    jr         $ra
     nop

glabel func_80016178
    addiu      $a0, 0x4000

glabel func_8001617C
    sll        $v0, $a0, 0x11
    bgezl      $v0, 1f
     srl       $t2, $a0, 3
    xori       $a0, 0x7fff
    srl        $t2, $a0, 3
1:
    lui        $v0, %hi(D_8008CA8C)
    andi       $t2, 0x7fe
    addiu      $v0, %lo(D_8008CA8C)
    addu       $v0, $t2
    lhu        $t2, 2($v0)
    lhu        $v0, ($v0)
    andi       $t1, $a0, 0xf
    sll        $a0, 0x10
    subu       $t2, $v0
    multu      $t2, $t1
    sll        $v0, 1
    mflo       $t2
    srl        $t2, 3
    bgez       $a0, 2f
     addu      $v0, $t2
    negu       $v0
2:
    jr         $ra
     nop

glabel func_800161D8
    addiu      $a0,0x4000
    sll        $v0, $a0, 0x11
    bgezl      $v0, 1f
     srl       $t2, $a0, 3
    xori       $a0, 0x7fff
    srl        $t2, $a0, 3
1:
    lui        $v0, %hi(D_8008CA8C)
    andi       $t2, 0x7fe
    addiu      $v0, %lo(D_8008CA8C)
    addu       $v0, $t2
    lhu        $v0, ($v0)
    sll        $a0, 0x10
    bgez       $a0, 2f
     sll       $v0, 1
    negu       $v0
2:
    jr         $ra
     nop



glabel func_8001621C
    l.s       $f0, 0($a0)
    l.s       $f1, 0($a1)
    l.s       $f2, 4($a0)
    l.s       $f3, 4($a1)
    sub.s      $f0, $f1
    l.s       $f4, 8($a0)
    l.s       $f5, 8($a1)
    sub.s      $f2, $f3
    .square    $f0
    sub.s      $f4, $f5
    .square    $f2
    add.s      $f0, $f2
    .square    $f4
    add.s      $f0, $f4
    jr         $ra
     sqrt.s    $f0, $f0

glabel func_8001625C
    l.s       $f0, ($a0)
    l.s       $f1, ($a1)
    l.s       $f2, 4($a0)
    l.s       $f3, 4($a1)
    sub.s      $f0, $f1
    l.s       $f4, 8($a0)
    l.s       $f5, 8($a1)
    sub.s      $f2, $f3
    mul.s      $f0, $f0
    sub.s      $f4, $f5
    mul.s      $f2, $f2
    add.s      $f0, $f2
    mul.s      $f4, $f4
    jr         $ra
     add.s     $f0, $f4

glabel func_80016298
    l.s       $f0, ($a0)
    l.s       $f1, ($a1)
    l.s       $f4, 8($a0)
    l.s       $f5, 8($a1)
    sub.s      $f0, $f1
    sub.s      $f4, $f5
    .square    $f0
    nop
    .square    $f4
    add.s      $f0, $f4
    jr         $ra
     sqrt.s    $f0, $f0

glabel func_800162C8
    l.s       $f0, ($a0)
    l.s       $f1, ($a1)
    l.s       $f4, 8($a0)
    l.s       $f5, 8($a1)
    sub.s      $f0, $f1
    sub.s      $f4, $f5
    mul.s      $f0, $f0
    nop
    mul.s      $f4, $f4
    jr         $ra
     add.s     $f0, $f4

glabel func_800162F4
    addiu      $a0, 0x4000

glabel some_getter_from_float_array
    lui        $t2, %hi(some_ascending_float_array)
    srl        $t0, $a0, 4
    andi       $t1, $a0, 0x4000
    addiu      $t2, %lo(some_ascending_float_array)
    beqz       $t1, 2f
     andi      $t0, 0x3ff
    andi       $t1, $a0, 0xf
    beql       $t1, $zero, 1f
     ori       $t1, $zero, 0x400
    addiu      $t0, 1
    ori        $t1, $zero, 0x400
1:
    sub        $t0, $t1, $t0
2:
    sll        $t0, 2
    addu       $t2, $t0
    andi       $t1, $a0, 0x8000
    beqz       $t1, 3f
     l.s      $f0, ($t2)
    neg.s      $f0
3:
    jr         $ra
     nop

glabel func_80016348
    mtc1       $zero, $f0
    addiu      $v0, $zero, 0
    c.eq.s     $f12, $f0
    bc1fl      1f
     c.lt.s    $f12, $f0
    c.eq.s     $f14, $f0
    bc1t       8f
     nop
    c.lt.s     $f12, $f0
1:
    bc1tl      2f
     c.lt.s    $f14, $f0
    c.lt.s     $f14, $f0
    bc1fl      6f
     c.lt.s    $f12, $f14
    neg.s      $f14
    j          3f
     addiu     $v0, $zero, 0x4000
    c.lt.s     $f14, $f0
2:
    neg.s      $f12
    bc1tl      4f
     neg.s     $f14
    ori        $v0, $zero, 0xc000
3:
    mov.s      $f13, $f12
    mov.s      $f12, $f14
    b          5f
     mov.s     $f14, $f13
    neg.s      $f14
4:
    ori        $v0, $zero, 0x8000
5:
    c.lt.s     $f12, $f14
6:
    bc1fl      7f
     div.s     $f10, $f14, $f12
    div.s      $f10, $f12, $f14
    li.s $f1, 2048.0
    lui        $t0, %hi(D_8008E294)
    addiu      $t0, %lo(D_8008E294)
    mul.s      $f10, $f1
    cvt.w.s    $f10, $f10
    mfc1       $t1, $f10
    nop
    andi       $t1, 0xffe
    add        $t0, $t1
    lh         $t0, ($t0)
    addu       $v0, $t0
    jr         $ra
     andi      $v0, 0xffff
    div.s      $f10, $f14, $f12
7:
    li.s $f1, 2048.0
    lui        $t0, %hi(D_8008E294)
    addiu      $t0, %lo(D_8008E294)
    addiu      $v0, 0x4000
    mul.s      $f10, $f1
    cvt.w.s    $f10, $f10
    mfc1       $t1, $f10
    nop
    andi       $t1, 0xffe
    add        $t0, $t1
    lh         $t0, ($t0)
    subu       $v0, $t0
    andi       $v0, 0xffff
8:
    jr         $ra
     nop

glabel func_80016444
    sub        $v0, $a1, $a0
    bge       $v0, 32768, 1f
    li       $t0, 65536
    blt       $v0, -32767, 2f
    nop
    jr         $ra
    nop
1:
    jr         $ra
     sub       $v0, $t0
2:
    jr         $ra
     add       $v0, $t0

glabel floor_f
    floor.w.s  $f14, $f12
    mfc1       $v0, $f14
    jr         $ra
    nop

glabel ceil_f
    ceil.w.s   $f14, $f12
    mfc1       $v0, $f14
    jr         $ra
    nop

.set reorder
glabel func_8001649C
    break      7
     l.s      $f10, 0x10($sp)
    l.s       $f0, D_800991B0
    mul.s      $f11, $f10, $f10
    l.s       $f1, D_800991B4
    l.s       $f2, D_800991B8
    mtc1       $a2, $f16
    lui        $at, %hi(D_800991BC)
    mul.s      $f0, $f12
    l.s       $f3, %lo(D_800991BC)($at)
    mtc1       $a3, $f18
    lui        $at, %hi(D_800991C0)
    l.s       $f4, %lo(D_800991C0)($at)
    lui        $at, %hi(D_800991C4)
    l.s       $f5, %lo(D_800991C4)($at)
    mul.s      $f1, $f14
    l.s       $f6, D_800991C8
    l.s       $f7, D_800991CC
    l.s       $f8, D_800991D0
    mul.s      $f2, $f16
    l.s       $f9, D_800991D4
    add.s      $f0, $f1
    mul.s      $f3, $f18
    nop
    mul.s      $f4, $f12
    add.s      $f2, $f3
    add.s      $f0, $f2
    mul.s      $f5, $f14
    nop
    mul.s      $f6, $f16
    add.s      $f4, $f5
    mul.s      $f7, $f18
    nop
    mul.s      $f15, $f10, $f11
    nop
    mul.s      $f8, $f12
    add.s      $f6, $f7
    add.s      $f4, $f6
    mul.s      $f9, $f16
    add.s      $f8, $f9
    mul.s      $f8, $f10
    nop
    mul.s      $f15, $f0
    nop
    mul.s      $f11, $f4
    add.s      $f14, $f8
    add.s      $f15, $f11
     add.s     $f0, $f14, $f15
    jr         $ra
.set noreorder


