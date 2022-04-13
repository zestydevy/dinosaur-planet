# Decompilation Guide
This document is intended to be an introductory guide to decomp (tailored specifically for Dinosaur Planet). If this is your first time with decomp then please read on! If you're already familiar with decomp then you may still find some useful information here, as this document is a little specific to Dinosaur Planet.


## Contents
- [1. Concepts](#1-concepts)
- [2. Decompiling a Function](#2-decompiling-a-function)
    - [2a. Assembly Extraction](#2a-assembly-extraction)
    - [2b. Reversing](#2b-reversing)
- [3. Symbols and Sections](#3-symbols-and-sections)


## 1. Concepts
It's important to first understand exactly what this project is attempting to accomplish. The goal is to create a matching decompilation. This means that when all reversed code is recompiled, the re-created ROM is byte-for-byte identical to the original.

This process is typically done on a per-function basis. The raw machine code for a function is extracted/disassembled from the original ROM into readable MIPS assembly code. Then, that function is decompiled into C code that when recompiled results in the exact same machine code that it started from.

Before continuing, please consider reading the [overview of the project](./Overview.md) to get an idea of where everything is located. Don't worry if not everything makes sense yet, this document will attempt to explain more in detail. 


## 2. Decompiling a Function
Let's go over the full process of decompiling a single function. This section will be using the already decompiled function `vec3_normalize` as an example (you can find the final source in `src/vec3.c`).

### 2a. Assembly Extraction
Before we can begin decompiling a function, we need to extract its assembly code from the ROM. This is done automatically using a tool called [splat](https://github.com/ethteck/splat). Currently, splat is already configured to extract most functions. Splat's configuration for Dinosaur Planet can be found at `splat.yaml` in the repository root.

Splat works by mapping an address range of the ROM to individual files. For example, `vec3_normalize` is currently part of the `vec3.c` file. You can see this mapping in `splat.yaml`:
```yaml
# From 0x17180 until the next entry is mapped to src/vec3.c
- [0x17180, c, vec3]
```

With this configuration, splat will take the recompiled contents of `vec3.c` and stitch them into the recompiled ROM starting at that exact address.

The final step is to temporarily include the original assembly of each function that is part of the file. This is done using the special pragma `GLOBAL_ASM`:
```c
#pragma GLOBAL_ASM("asm/nonmatchings/vec3/vec3_normalize.s")
```

Splat extracts each function it finds into the `asm/nonmatchings` directory as individual `.s` files grouped by the name of the mapped file they are a part of. The `GLOBAL_ASM` pragma is used by another tool [asm-processor](https://github.com/simonlindholm/asm-processor), which is responsible for actually including the referenced `.s` file at that position in the C source.

Great! Now, we can actually decompile this function.

### 2b. Reversing
Firstly, let's create some space to start decompiling and reversing `vec3_normalize`. We'll use an `#if` directive to temporarily replace the `GLOBAL_ASM` pragma with an actual C function:
```c
#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/vec3/vec3_normalize.s")
#else
// We don't know anything about the function signature yet, so we'll 
// just make it void for now.
void vec3_normalize() {

}
#endif
```

Next, let's finally take a look at the actual assembly code for the function:
```
glabel vec3_normalize
addiu      $sp, $sp, -0x18
sw         $ra, 0x14($sp)
lwc1       $f2, ($a0)
lwc1       $f14, 4($a0)
lwc1       $f0, 8($a0)
mul.s      $f4, $f2, $f2
sw         $a0, 0x18($sp)
mul.s      $f6, $f14, $f14
add.s      $f8, $f4, $f6
mul.s      $f10, $f0, $f0
jal        sqrtf
 add.s     $f12, $f10, $f8
mtc1       $zero, $f16
lw         $a0, 0x18($sp)
mov.s      $f12, $f0
c.eq.s     $f0, $f16
lui        $at, 0x3f80
bc1tl      .L8001678C
 mov.s     $f0, $f12
mtc1       $at, $f18
lwc1       $f4, ($a0)
lwc1       $f10, 4($a0)
div.s      $f2, $f18, $f0
lwc1       $f16, 8($a0)
mul.s      $f6, $f4, $f2
nop
mul.s      $f8, $f10, $f2
nop
mul.s      $f18, $f16, $f2
swc1       $f6, ($a0)
swc1       $f8, 4($a0)
swc1       $f18, 8($a0)
mov.s      $f0, $f12
.L8001678C:
lw         $ra, 0x14($sp)
addiu      $sp, $sp, 0x18
jr         $ra
 nop
```

At this point, it's very helpful to have a [MIPS assembly manual](https://www.cs.cornell.edu/courses/cs3410/2008fa/MIPS_Vol2.pdf) on hand so we can look up the meaning of each instruction. Each line in the above disassembly is a single instruction in the format `mnemonic [operand1,operand2,...]`. For example the first instruction is the mnemonic 'addiu' with the operands 'sp,sp,-0x18'.

For the first step of decompilation, we'll convert each individual instruction into the equivalent C code:
```c
void vec3_normalize(f32 *a0) {
    // addiu      $sp, $sp, -0x18   ; We'll skip the first two instructions,
    // sw         $ra, 0x14($sp)    ; which are just part of the stack setup
    
    //                              ; First is some float value loads from $a0,
    // lwc1       $f2, ($a0)        ; the first parameter of the function
    f32 f2 = *a0;
    // lwc1       $f14, 4($a0)
    f32 f14 = *(a0+4)
    // lwc1       $f0, 8($a0)
    f32 f0 = *(a0+8)
    // mul.s      $f4, $f2, $f2     ; Next, is some arithmetic
    f32 f4 = f2 * f2;
    // sw         $a0, 0x18($sp)    ; There's an upcoming function call that
    //                              ; doesn't use the $a0 register, so the code
    //                              ; must save it to the stack first
    *(sp+0x18) = a0;
    // mul.s      $f6, $f14, $f14
    f32 f6 = f14 * f14;
    // add.s      $f8, $f4, $f6
    f32 f8 = f4 + f6;
    // mul.s      $f10, $f0, $f0
    f32 f10 = f0 * f0;
    // jal        sqrtf             ; Here we have a function call with a
    //  add.s     $f12, $f10, $f8   ; branch delay slot. The delay slot is
    //                              ; executed first in this case.
    f32 f12 = f10 + f8;
    //                              ; Note: Normally function calls will use
    //                              ; the a0-a3 registers but sqrtf is
    //                              ; implemented in assembly. Looking at its
    //                              ; code shows it takes $f12 as its input
    //                              ; and outputs to $f0
    f32 f0 = sqrtf(f12);                     
    // mtc1       $zero, $f16
    f32 f16 = 0;
    // lw         $a0, 0x18($sp)    ; $a0 is restored from the stack
    // mov.s      $f12, $f0
    f32 f12 = f0;
    // c.eq.s     $f0, $f16         ; Here we have a conditional branch with
    // lui        $at, 0x3f80       ; a 'likely' delay slot. The delay slot
    // bc1tl      .L8001678C        ; in this case is only executed if the
    //  mov.s     $f0, $f12         ; condition is true
    int at = 0x3f800000;
    if (f0 == f16) {
        f0 = f12;
        goto L8001678C;
    }
    // mtc1       $at, $f18         ; More arithmetic
    f32 f18 = at;
    // lwc1       $f4, ($a0)
    f32 f4 = *a0;
    // lwc1       $f10, 4($a0)
    f32 f10 = *(a0+4);
    // div.s      $f2, $f18, $f0
    f32 f2 = f18 / f0;
    // lwc1       $f16, 8($a0)
    f32 f16 = *(a0+8);
    // mul.s      $f6, $f4, $f2
    f32 f6 = f4 * f2;
    // nop
    // mul.s      $f8, $f10, $f2
    f32 f8 = f10 * f2;
    // nop
    // mul.s      $f18, $f16, $f2
    f32 f18 = f16 * f2;
    // swc1       $f6, ($a0)
    *a0 = f6;
    // swc1       $f8, 4($a0)
    *(a0+4) = f8;
    // swc1       $f18, 8($a0)
    *(a0+8) = f18;
    // mov.s      $f0, $f12
    f32 f0 = f12;
    L8001678C:
    // lw         $ra, 0x14($sp)    ; Finally, the stack is restored from
    // addiu      $sp, $sp, 0x18    ; the previous frame
    
    // jr         $ra               ; Note: Returning a float uses the $f0
    //                              ; register instead of $v0 like normal.
    //                              ; We can infer that a float is returned
    //                              ; since $f0 is set at the end of the
    //                              ; function and not used.
    return f0;
    //  nop
}
```

> Whew! That was a bit of code to go through and annotate. It's worth mentioning at this point that auto-decompilers can be used instead of manually translating each instruction, such as [mips2c](https://simonsoftware.se/other/mips_to_c.py) and [Ghidra](https://ghidra-sre.org/).
>
> It's unlikely that a tool will decompile a perfectly matching function, but it can save a lot of time getting started with one.

Now that we have each instruction translated, we can start analyzing the function and start reducing code into readable C (currently the code won't even compile).

The first interesting code we can see is the use of `$a0`:
```c
f32 f2 = *a0;
f32 f14 = *(a0+4)
f32 f0 = *(a0+8)
// ...
*a0 = f6;
*(a0+4) = f8;
*(a0+8) = f18;
```

This looks a lot like the access of struct fields. From this, we could assume that `$a0` holds a pointer to a structure with three 4-byte wide fields. Since this function is known to be working with 3-dimensional vectors, lets use the following definition:
```c
typedef struct {
    f32 x, y, z;
} Vec3f;
```

Additionally, we were able to infer that this function returns a single float. Now that the return value and parameters of the function are known, let's update the signature:
```c
f32 vec3_normalize(Vec3f *a0)
```

Next up, let's start reducing and cleaning up the function implementation. Many of the local variables derived from `$f` registers aren't individual variables, but are part of larger expressions.
```c
f32 vec3_normalize(Vec3f *a0) {
    f32 f18, f2;
    f32 f12 = sqrtf((a0->z * a0->z) + ((a0->x * a0->x) + (a0->y * a0->y)));
    if (f12 == 0.0f) {
        goto L8001678C;
    }
    
    f18 = 0x3f800000;
    f2 = f18 / f12;
    a0->x = a0->x * f2;
    a0->y = a0->y * f2;
    a0->z = a0->z * f2;
    
    L8001678C:
    return f12;
}
```

This looks significantly better already (and actually compiles)! Let's check our progress so far by diffing our implementation against the original ROM. We can do this by building the ROM `./dino.py build` and then running diff on the function's symbol `./dino.py diff vec3_normalize`. Looks like everything matches except for one instruction: `lui at,0x3f80`, which from our implementation is currently `lui at,0x4e7e`. If we trace this instruction back to the C code it's currently the line `f18 = 0x3f800000;` Let's fix that.

Looking back at the assembly, we can see these two instructions:
```
lui     at,0x3f80
mtc1    at,$f18
```

These two instructions are actually 'bitwise' casting the integer `0x3f800000` into a float, rather than simply assigning 16256 (0x3f80) to `f18`. Let's use [a floating point converter](https://www.h-schmidt.net/FloatConverter/IEEE754.html) to see what `0x3f800000` is bit-for-bit as a float. Turns out it is actually `1.0`! If we replace the statement with `f18 = 1.0f`, re-build, and re-run the diff, we can now see that the function matches!

Before we call this function done however, let's clean it up by getting rid of the `goto` and using better variable names based on what we know about vector normalization.
> Note: Most functions will not use `goto` at all. Instead, branches should be translated into `if` and sometimes simple `switch` statements.
```c
f32 vec3_normalize(Vec3f *v) {
    f32 length, lengthInv;
    
    length = sqrtf((v->z * v->z) + ((v->x * v->x) + (v->y * v->y)));
    
    if (length != 0.0f) {
        lengthInv = 1.0f / length;
        v->x = v->x * lengthInv;
        v->y = v->y * lengthInv;
        v->z = v->z * lengthInv;
    }
    
    return length;
}
```

Congratulations 🎉, you've gone through the full decompilation process of a function!

There is still so much more to reversing assembly, but this is the general process of breaking it down one step at a time. For much more information on decompiling assembly from IDO, please see the ever-evolving [-O2 decompilation (for IDO 5.3 and 7.1)](https://hackmd.io/vPmcgdaFSlq4R2mfkq4bJg).


## 3. Symbols and Sections


