.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.set fp=64

.section .text, "ax"

glabel entrypoint
    lui        $t0, %hi(core_BSS_START)
    lui        $t1, %hi(core_BSS_SIZE)
    addiu      $t0, $t0, %lo(core_BSS_START)
    ori        $t1, $t1, %lo(core_BSS_SIZE)
  .clear_bytes:
    addi       $t1, $t1, -0x8
    sw         $zero, 0x0($t0)
    sw         $zero, 0x4($t0)
    bnez       $t1, .clear_bytes
     addi      $t0, $t0, 0x8
    lui        $t2, %hi(boot)
    /* Note: The stack size implied here is way bigger than what is actually allocated. This code
             starts the stack somewhere in the camera.c bss! */
    lui        $sp, %hi(gEntrypointThreadStack + 0x1000)
    addiu      $t2, $t2, %lo(boot)
    jr         $t2
     addiu     $sp, $sp, %lo(gEntrypointThreadStack + 0x1000)
endlabel entrypoint
    nop
    nop
    nop
    nop
    nop
    nop
