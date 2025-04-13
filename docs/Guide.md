[[← back]](README.md)

# Decompilation Guide
This document is intended to be an introductory guide to decomp (tailored specifically for Dinosaur Planet). If this is your first time with decomp then please read on! If you're already familiar with decomp then you may still find some useful information here, as this document is a little specific to Dinosaur Planet.


## Contents
- [1. Introduction](#1-introduction)
- [2. Decompiling a Function](#2-decompiling-a-function)
    - [2a. Assembly Extraction](#2a-assembly-extraction)
    - [2b. Reversing](#2b-reversing)
- [3. Symbols](#3-symbols)
- [4. Data Sections](#4-data-sections)
- [5. Optimization Levels and Debug Symbols](#5-optimization-levels-and-debug-symbols)
- [6. DLL Decompilation](#6-dll-decompilation)
- [7. Decompilation Tools](#7-decompilation-tools)


## 1. Introduction
It's important to first understand exactly what this project is attempting to accomplish. The goal is to create a matching decompilation. This means that when all reversed code is recompiled, the re-created ROM is byte-for-byte identical to the original.

This process is typically done on a per-function basis. The raw machine code for a function is extracted/disassembled from the original ROM into readable MIPS assembly code. Then, that function is decompiled into C code that when recompiled results in the exact same machine code that it started from.

Before continuing, please consider reading the [overview of the project](./Overview.md) to get an idea of where everything is located. Don't worry if not everything makes sense yet, this document will attempt to explain more in detail. 


## 2. Decompiling a Function
Let's go over the full process of decompiling a single function. This section will be using the already decompiled function `vec3_normalize` as an example (you can find the final source in `src/vec3.c`).

### 2a. Assembly Extraction
Before we can begin decompiling a function, we need to extract its assembly code from the ROM. This is done automatically using a tool called [splat](https://github.com/ethteck/splat). Currently, splat is already configured to extract most functions. Splat's configuration for Dinosaur Planet can be found at `splat.yaml` in the repository root.

Splat works by mapping an address range of the ROM to individual files. For example, `vec3_normalize` is currently part of the `vec3.c` file. You can see this mapping in `splat.yaml`:
```yaml
# From ROM 0x17180 until the next entry is mapped to src/vec3.c
- [0x17180, c, vec3]
```

With this configuration, splat will take the recompiled contents of `vec3.c` and stitch them into the recompiled ROM starting at that exact address.

The final step is to temporarily include the original assembly of each function that is part of the file. This is done using the special pragma `GLOBAL_ASM`:
```c
#pragma GLOBAL_ASM("asm/nonmatchings/vec3/vec3_normalize.s")
```

Splat extracts each function it finds into the `asm/nonmatchings` directory as individual `.s` files grouped by the name of the mapped file they are a part of. The `GLOBAL_ASM` pragma is used by another tool, [asm-processor](https://github.com/simonlindholm/asm-processor), which is responsible for actually including the referenced `.s` file at that position in the C source.

Great! Now, we can start to decompile this function.

### 2b. Reversing
First, let's create some space to start decompiling and reversing `vec3_normalize`. We'll use an `#if` directive to temporarily replace the `GLOBAL_ASM` pragma with an actual C function:
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

Next, let's look at the full assembly code of the function:
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

At this point, it's very helpful to have a [MIPS assembly manual](https://www.cs.cornell.edu/courses/cs3410/2008fa/MIPS_Vol2.pdf) on hand so we can look up the meaning of each instruction. Each line in the above disassembly is a single instruction in the format `mnemonic [operand1,operand2,...]`. For example, the first instruction is the mnemonic 'addiu' with the operands 'sp,sp,-0x18'.

For the first step of decompilation, we'll convert each individual instruction into the equivalent C code:
```c
void vec3_normalize(f32 *a0) {
    // addiu      $sp, $sp, -0x18   ; We'll skip the first two instructions,
    // sw         $ra, 0x14($sp)    ; which are just part of the stack setup
    
    //                              ; First is some float value loads from $a0,
    // lwc1       $f2, ($a0)        ; the first parameter of the function
    f32 f2 = *(a0+0);
    // lwc1       $f14, 4($a0)
    f32 f14 = *(a0+1)
    // lwc1       $f0, 8($a0)
    f32 f0 = *(a0+2)
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
    //                              ; Note: Normally, function calls will use
    //                              ; the $a0-$a3 registers for arguments and
    //                              ; $v0 for returns but sqrtf takes and
    //                              ; returns a floating point value so,
    //                              ; according to the MIPS O32 ABI, it uses
    //                              ; $f12 for the argument and $f0 as the
    //                              ; return value.
    f32 f0 = sqrtf(f12);
    // mtc1       $zero, $f16
    f32 f16 = 0;
    // lw         $a0, 0x18($sp)    ; $a0 is restored from the stack
    a0 = *(sp+0x18);
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
    f32 f4 = *(a0+0);
    // lwc1       $f10, 4($a0)
    f32 f10 = *(a0+1);
    // div.s      $f2, $f18, $f0
    f32 f2 = f18 / f0;
    // lwc1       $f16, 8($a0)
    f32 f16 = *(a0+2);
    // mul.s      $f6, $f4, $f2
    f32 f6 = f4 * f2;
    // nop
    // mul.s      $f8, $f10, $f2
    f32 f8 = f10 * f2;
    // nop
    // mul.s      $f18, $f16, $f2
    f32 f18 = f16 * f2;
    // swc1       $f6, ($a0)
    *(a0+0) = f6;
    // swc1       $f8, 4($a0)
    *(a0+1) = f8;
    // swc1       $f18, 8($a0)
    *(a0+2) = f18;
    // mov.s      $f0, $f12
    f32 f0 = f12;
    L8001678C:
    // lw         $ra, 0x14($sp)    ; Finally, the stack is restored from
    // addiu      $sp, $sp, 0x18    ; the previous frame
    
    // jr         $ra               ; Note: Returning a float uses the $f0
    //                              ; register instead of $v0 like normal.
    //                              ; We can infer that a float is returned
    //                              ; since $f0 is set at the end of the
    //                              ; function and not used after.
    return f0;
    //  nop
}
```

> Whew! That was a bit of code to go through and annotate. It's worth mentioning at this point that auto-decompilers can be used instead of manually translating each instruction, such as [decomp.me](https://decomp.me/) and [mips2c](https://simonsoftware.se/other/mips_to_c.py).
>
> It's unlikely that a tool will decompile assembly into a perfectly matching function, but it can still save a lot of time.

Now that we have each instruction translated, we can start analyzing the function and start reducing code into readable C (currently the code won't even compile).

The first interesting code we can see is the use of `$a0`:
```c
f32 f2 = *(a0+0);
f32 f14 = *(a0+1);
f32 f0 = *(a0+2);
// ...
*(a0+0) = f6;
*(a0+1) = f8;
*(a0+2) = f18;
```

This looks a lot like the access of struct fields. From this, we could assume that `$a0` holds a pointer to a structure with three four-byte wide fields. Since this function is known to be working with three-dimensional vectors, let's use the following definition:
```c
typedef struct {
    f32 x, y, z;
} Vec3f;
```

Additionally, we were able to infer that this function returns a single float. Now that the return value and parameters of the function are known, let's update the signature:
```c
f32 vec3_normalize(Vec3f *a0)
```

Next up, let's start reducing and cleaning up the function implementation. Many of the local variables derived from `$f` registers aren't individual variables but are part of larger expressions.
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

This looks significantly better already (and compiles)! Let's check our progress so far by diffing our implementation against the original ROM. We can do this by building the ROM (`./dino.py build`) and then running diff on the function's symbol (`./dino.py diff vec3_normalize`). Looks like everything matches except for one instruction: `lui at,0x3f80`, which from our implementation is currently `lui at,0x4e7e`. If we trace this instruction back to the C code, we can see it's from the line `f18 = 0x3f800000;` Let's fix it.

> Tip: When running `diff` consider passing the flags `-m -o -w` (or just `-mow`). This will tell diff to run build first, compare object files (to display symbol names), and watch the source file for changes and auto re-build, respectively.

Looking back at the assembly, we can see these two instructions:
```
lui     at,0x3f80
mtc1    at,$f18
```

These two instructions are actually 'bitwise' casting the integer `0x3f800000` into a float, rather than simply assigning the float 1,065,353,216.0 to `f18`. Let's use [a floating point converter](https://www.h-schmidt.net/FloatConverter/IEEE754.html) to see what `0x3f800000` is bit-for-bit as a float. Turns out it's actually `1.0`! If we replace the statement with `f18 = 1.0f;`, re-build, and re-run the diff, we can now see that the function matches!

Before we call this function done however, let's clean it up by getting rid of the `goto` and using better variable names based on what we know about vector normalization:
> Note: Most functions will not use `goto` at all and usually won't match with them still in place. Instead, branches should be translated into `if` and sometimes simple `switch` statements.
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


## 3. Symbols
An important part of decomp is giving names to memory addresses, known as symbols. This includes global/local static variables and functions.

Part of splat's job is to automatically generate symbols for addresses that it sees referenced. The full list of these generated symbols can be found (after running `./dino.py extract`) in the `undefined_funcs_auto.txt` and `undefined_syms_auto.txt` files in the repository root. Initially, splat will also generate a name for each symbol, such as `func_<address>` and `D_<address>` for functions and variables respectively. These names can be overwritten using the `symbol_addrs.txt` file (also in the repository root).

The format of each line in `symbol_addrs.txt` is as follows:
```
<symbol name> = <VRAM address in hex>; // <attributes...>
```

For example:
```
gObjectCount = 0x800b2934; // size:0x2
texture_load = 0x8003cda8; // type:func
```

Attributes are separated by spaces and are optional unless splat is incorrectly detecting the symbol type. Although optional, the size attribute is useful for data symbols as it allows splat to provide additional analysis such as warnings about symbol overlaps.

Sometimes, there will be an address that should have a symbol that splat was unable to detect. This usually happens when the address is not explicitly referenced in the final assembly code. In this case, the symbol cannot be added to `symbol_addrs.txt` and instead must be added to `undefined_funcs.txt` or `undefined_syms.txt` for functions and variables respectively (found in the repository root). These files are fed directly into the linker and as such should not contain the `type` or `size` attributes (just the format `<name> = <VRAM address in hex>;`).

> Note: When modifying `symbol_addrs.txt`, you will need to re-run `./dino.py extract` to re-generate the linker the script with the new symbols. If you just modify `undefined_funcs.txt` or `undefined_syms.txt`, then this is not required.


## 4. Data Sections
Each compilation unit (`.c` files, `.s`, etc.) contributes code and data to various sections of its resulting object file (and ultimately the final linked ELF file). The common sections are:
- `.text` - Executable machine code, each compiled function ends up here.
- `.data` - *Initialized* global/local static variables.
- `.rodata` - *Initialized* global/local static constants (also includes things like jump tables and string literals).
- `.bss` - *Uninitialized* global/local static variables and constants.

The `.data`, `.rodata`, and `.bss` sections are known as data sections (or data segments). Over the course of decomp, these sections need to be defined in the `splat.yaml` configuration for each file. Data sections are defined very similarly to code in splat, for example:
```yaml
# From 0x92370 until the next entry is the .data section (in the ROM)
# for the resulting object file of src/crash.c
- [0x92370, .data, crash]
```

The preceding example will tell splat to generate the linker script such that the `.data` section of `crash.c`'s object file (`build/src/crash.o`) will be inserted at the address `0x92370` in the final ROM.

> Note: Modifying anything in `splat.yaml` will require you to re-run `./dino.py extract` to re-extract code/data and, more importantly, to re-generate the linker script.

### Example: A `switch` jump table
As an example, let's take a look at the function `func_8005D3A4`, which happens to be the first function in `video.c` with a `switch` statement that compiles into a jump table.

If we look through the first few lines of assembly, we can see that the function references a jump table at the VRAM address `0x8009ACC0`. In order to get this function to match, the `.rodata` section for the file (`video.c`) needs to marked in `splat.yaml` such that the `switch` statement, when compiled, places its jump table at `0x8009ACC0`. To do this, we'll first need to convert the VRAM address of the jump table to its ROM address.

To convert the address, we need to first look at the current splat config. The configured segment for code contains:
```yaml
start: 0x1000       # ROM offset
vram: 0x80000400    # VRAM offset
```

With this, we can convert the VRAM address to the ROM address as follows:
```
(0x8009ACC0 - 0x80000400) + 0x1000 = 0x9B8C0
```

Finally, we tell splat about the `.rodata` section for `video.c`:
```yaml
- [0x9B8C0, .rodata, video]
```

> Note: This works in `video.c`'s case because this jump table is the **first** piece of data contributed to `.rodata` for the file. If there was, for example, another jump table further up in the file, then splat would need the ROM address of that jump table.


## 5. Optimization Levels and Debug Symbols
Just about all game code in Dinosaur Planet was originally compiled with the same compiler flags. However, the game statically links other libraries that were compiled differently. The two [IDO `cc` flags](https://cocky-wescoff-177c47.netlify.app/cc_manual.html) that primarily influence assembly generation are:
- `-O[0-3]` - Defines the optimization level to compile with (`-O0` is off and `-O3` is all optimizations).
- `-g[0-3]` - Defines the symbol tables to be generated for debugging (`-g0` is off and `-g3` is for full debugging).

The flag combinations currently known for the decomp are:
- `-O2 -g3` - The default. Used for almost everything.
- `-O2 -g0` - Used for compiling *modified* libultra code.
- `-O1 -g0` - Used for compiling *unmodified* libultra code.
- `-O1 -g2` - Used for libnaudio code.

Sometimes, to get a function to match you will need to change the optimization and debugging levels that the containing file is compiled with. This can be done by adding the comment `// @DECOMP_OPT_FLAGS=<opt flags>` to the top of the file. For example, `// @DECOMP_OPT_FLAGS=-O1` would compile the file with the optimization flag `-O1` instead of the default.

Additionally, you may need a different version of IDO. This can be set by adding the comment `// @DECOMP_IDO_VERSION=<version>` to the top of the file. Only versions `5.3` (default) and `7.1` are available currently.

> Note: The build script must be re-configured for the new optimization flags/compiler to take effect (`./dino.py configure`).


## 6. DLL Decompilation
Dinosaur Planet DLLs goes through a separate extraction and compilation process to the core code. Instead of using `splat`, the `tools/dll_split.py` script is used to extract assembly/data, stub C code, and automatically discover symbols. This script does not need to be used directly. If a DLL's directory under `src/dlls` doesn't exist yet, you can use `./dino.py setup-dll` to create it. Once created, running the usual `./dino.py extract` command or `./dino.py extract-dll <dll number>` will re-extract the DLL as necessary.

For the C source of the DLL, the same decompilation process is used as described in the above sections.

### DLL Exports
An important part of DLL recompilation is the `exports.s` file that you will find in each DLL's `src` directory. This defines both the names of the DLL's constructor and destructor as well as each entry of the DLL's export table. The `./dino.py setup-dll` command will set this file up for you, but the function names in this file will need to be updated if they are changed.

### DLL Symbols
Just like the core code uses files like `symbol_addrs.txt` to define symbols, each DLL has their own `syms.txt` file instead. This file follows the same syntax as described before.

DLLs work with three types of symbols:
- External symbols (non-static, defined in the DLL)
- Local symbols (static, defined in the DLL)
- Import symbols (defined in the core code)

Import symbols are special and are the same for all DLLs. These symbols don't define actual addresses and instead are indexes into the DLLSIMPORT.tab file, which contains the real address. The indexes also have their 32nd bit set (0x80000000). For simplicity, all import symbols are defined in the `export_symbol_addrs.txt` file found in the repository root and are linked with every DLL.

### DLL Build Configuration
Each DLL comes with a `dll.yaml` file that defines whether the DLL should be compiled and linked.


## 7. Decompilation Tools
There are many tools out there to assist with decomp. You can find [a full list of tools and other resources in the contribution guide](../CONTRIBUTING.md#resources).

Here are some recommended tools:

### decomp.me
[decomp.me](https://decomp.me/) is an online collaborative space for decomp. Here, you can create a 'scratch' for a function and get a live diff of your current C implementation and the expected assembly code. You can also share a link to your function for others to play around with. The site has a preset for the Dinosaur Planet decomp that will automatically set up the correct compiler version and default flags.

> Note: To use decomp.me, you'll need a 'context' file for the function you're working on. This is essentially just a C header file that contains all of the declarations of types, functions, and macros that are used by your function. You can generate this file by running `./dino.py context <path to your file.c>` (you may need to remove function implementations at the bottom of the generated file).

### mips2c
mips2c can be used to automatically decompile a function from assembly into C. There is an [online](https://simonsoftware.se/other/mips_to_c.py) and [offline](https://github.com/matt-kempster/mips_to_c) version of the tool. This tool can save a lot of time by producing an initial C implementation to work with that you can then tweak until the function matches.

### Ghidra
[Ghidra](https://ghidra-sre.org/) is a full reverse engineering tool suite. It can be used for many things such as exploring the full assembly of the ROM, getting instant C decompilations of each function, tracing references between addresses, and testing function and structure definitions. Ghidra can be a bit overwhelming at first, but once learned it can be extremely valuable for analyzing and understanding functions and data structures.

> Note: To use Ghidra with the Dinosaur Planet ROM, you will need [the custom loader plugin N64LoaderWV-DinoPlanet](https://github.com/HugoPeters/N64LoaderWV-DinoPlanet). This plugin allows Ghidra to read the N64 style ROM and analyze Dinosaur Planet DLL code.

