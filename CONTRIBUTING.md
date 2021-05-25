# Contributing to the Dinosaur Planet Decompilation
Welcome to the Dinosaur Planet Decompilation and thanks for taking the time to contribute!

Please read on to learn about what the decompilation involves and how you can help.


### Discord
https://discord.gg/H6WGkznZBc

If you're interested in contributing, please consider joining us on Discord over at the [Dinosaur Planet Community server](https://discord.gg/H6WGkznZBc) in the `#decompilation` channel! The Discord server is the best place to get your questions answered and is where a lot of the project's discussions take place.


## Table of Contents
- [How does decompilation work](#how-does-decompilation-work)
- [Things to know before contributing](#things-to-know-before-contributing)
- [How can I contribute?](#how-can-i-contribute)
    - [Documenting code](#documenting-code)
    - [Decompiling functions](#decompiling-functions)
- [Style guide](#style-guide)
- [Opening a pull request](#opening-a-pull-request)
- [Resources](#resources)


## How does decompilation work
The goal behind the decompilation is to extract and convert the game's code from MIPS machine code to human readable MIPS assembly and ideally C code. Once this code has been converted, the game can then be recompiled using the same compiler and assembler as the original game to produce an identical ROM. A identical recompiled ROM is important because it verifies that the converted code is, at least, functionally 100% the same as the original ROM.

At a high level, the process goes like this:
1. Disassemble the ROM's executable machine code into MIPS assembly code (found under `asm/`).
    - This is done in part by using a tool called [splat](tools/splat/README.md), which splits the code into separate files. See `splat.yaml` for the configuration.
2. Import assembly code in C source files on a function-by-function basis using `#pragma GLOBAL_ASM(...)` (found under `src/`).
    - How splat splits code into different files also affects this part. You'll notice that for each folder under `asm/nonmatchings/` there is a counterpart C source file.
3. Replace each `#pragma GLOBAL_ASM(...)` by decompiling the included assembly code into a C function.
4. Recompile the ROM using IDO 5.3. If the recompiled ROM matches the original ROM, the function decompiled in step 3 is correct!
    - Note: IDO (IRIS Development Option) is a set of compiler tools developed for the IRIX operating system and do not run natively on x86. This project uses a [recompiled version of each program](https://github.com/Emill/ido-static-recomp), which you can find under `tools/ido_recomp/`.


## Things to know before contributing
Before working on the decompilation, you should:
- Have a decent understanding of the C89 programming language.
- Be at least a little familiar with the MIPS instruction set and/or assembly programming.
- Understand the basics, purpose, and relationship of compilers and assemblers.
- Have some familiarity with compiler optimizations.

It's perfectly okay if you don't know all of this right away, but you will need to get familiar with these topics to create matching code.

Some documents to help:
- [MIPS IV Instruction Set](http://math-atlas.sourceforge.net/devel/assembly/mips-iv.pdf) 
- [C89 Draft](https://port70.net/~nsz/c/c89/c89-draft.html)


### Compiler requirements


#### Line endings
If you plan on building the ROM on your machine, it's very important that the repository is cloned with Unix line endings as the compiler will not recognize Windows line endings.


#### Compiling on Windows
The compiler will not run natively on Windows and will need to be ran using something like [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10).


## How can I contribute?
There's different ways you can contribute to the decompilation, some don't involve directly decompiling code at all!

- You could help [document the codebase](#documenting-code) by giving useful names to variables and functions, defining data structures, and writing in-code documentation.
- If you're comfortable with or interested in the [decompilation process](#how-does-decompilation-work), you could help [decompile and match functions](#decompiling-functions).


### Documenting code
Code documentation is an ongoing effort and isn't necessarily complete even for matching code. Using an emulator with debugging support (such as [Project 64](https://www.pj64-emu.com/)) you can play around with functions and variables to determine how they work and what their purpose is.

There's two ways to find the addresses of functions and global variables:
- The `symbol_addrs.txt` and `undefined_syms.txt` files.
- The `dino.map` file produced by building a ROM.


#### Naming symbols
If you've found a good name for a function, global variable, or are looking to label a new memory address:

1. Make sure the name follows the project's [style guide](#style-guide).
2. If this is an existing symbol, be sure to change all references in C code to the new name.
3. Add the new name/address to `symbol_addrs.txt` for new symbols, otherwise rename the existing one. **IMPORTANT:** If the symbol is not referenced statically in assembly code, you will need to define the symbol in `undefined_syms.txt` instead.
4. For new global variables, if you're confident about the data type, consider defining it in `include/variables.h`.
5. For functions that haven't been decompiled yet, be sure to update its `GLOBAL_ASM` include.
6. Rebuild the ROM to verify nothing broke.


#### Defining structs
If you've discovered a new struct or more about an existing one:

- For new structs, be sure to update the global variable's type that uses it. Also consider defining the struct in `include/variables.h` if there isn't a more appropriate header file for it.
- For existing structs, update the existing definition. **IMPORTANT:** Be wary of updates to structs already in use. You must make sure your change doesn't break existing decompiled code, and if it does, you'll need to update that as well.
- Like always, be sure to follow the project's [style guide](#style-guide).


#### Writing in-code documentation
If you would like to document an explanation or bits of information for a function or variable:

1. Make sure the symbol of the function/variable is in code (see [Naming symbols](#naming-symbols)).
2. Add a C comment to the function/variable with any information you would like to add. **Be sure to follow the [style guide](#style-guide)**, this project prefers [doxygen](https://www.doxygen.nl/index.html) styled block comments.


### Decompiling functions
> Be sure to read the [How does decompilation work](#how-does-decompilation-work) section first to understand the process at a high level.

Decompiling MIPS assembly into C code is where most of the project's development time will be spent and where help is needed the most. There's many different methods for decompiling code that really comes down to preference. For an overview of how to do this:

1. Find a nonmatching function that you would like to decompile.
    - This could be a function that is still just a `GLOBAL_ASM` include, or one that has already been decompiled but isn't producing matching code when recompiled.
2. Replace the `GLOBAL_ASM` include (if necessary) with a C function that works just like the original assembly code.
    - For an in depth decompilation guide, please see [-O2 decompilation (for IDO 5.3 and 7.1)](https://hackmd.io/vPmcgdaFSlq4R2mfkq4bJg).
    - There's many tools that can assist in this process like a decompiler such as [mips2c](https://simonsoftware.se/other/mips_to_c.py) or [Ghidra](https://ghidra-sre.org/) (Ghidra needs a [custom loader for the ROM](https://github.com/HugoPeters/N64LoaderWV-DinoPlanet)). However, a decompiler isn't required. If preferred, you could decompile functions by hand. Decompilers usually don't result in matching code and will require additional work. Sometimes decompiling by hand can be easier.
3. Tweak the C implementation until it produces a matching ROM or is at least functionally equivalent (ideally with minimal differences).
    - Use `./diff.py` to compare the recompiled ROM to the original and see which instructions are different.
4. Make sure the code adheres to the project's [style guide](#style-guide).


## Style guide
The Dinosaur Planet Decompilation follows the same [style guide as the SM64 Decompilation](https://docs.google.com/document/d/1T_6dUj32QOscrMOMDsr7bY0esohswEAstKCO4gFn6t0).

In addition:
- Use Unix line endings (`\n`).
- Ensure files end with a newline.
- Strip trailing whitespace.


## Opening a pull request
Before opening a pull request, you should make sure that:

1. Any code additions/changes follow the project's [style guide](#style-guide).
2. The ROM builds and matches the original ROM (only for non-draft pull requests).
    - If you have a decompiled function that doesn't match but is worth committing, make sure to use an `#if` macro to let the compiler ignore the decompiled implementation and include the original assembly instead.


## Resources
- Tools
    - [mips2c (online)](https://simonsoftware.se/other/mips_to_c.py) - Online MIPS to C decompiler.
    - [mips2c (offline)](https://github.com/matt-kempster/mips_to_c) - Source of mips2c.
    - [Ghidra](https://ghidra-sre.org/) - Reverse engineering suite (including a decompiler).
    - [Ghidra N64LoaderWV-DinoPlanet](https://github.com/HugoPeters/N64LoaderWV-DinoPlanet) - A Ghidra plugin for loading the Dinosaur Planet ROM.
    - [f32 converter](https://www.h-schmidt.net/FloatConverter/IEEE754.html) - Convert between 32-bit floating point numbers and binary/hexadecimal representations.
    - [f64 converter](https://babbage.cs.qc.cuny.edu/IEEE-754.old/64bit.html) - Convert between 64-bit floating point numbers and binary/hexadecimal representations.
    - [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) - Randomly permute C files to better match a target binary.
    - [IDO Compiler Explorer](https://compiler.queueram.com/) - Online compiler instances for different versions of IDO.
- Documentation
    - [MIPS IV Instruction Set](http://math-atlas.sourceforge.net/devel/assembly/mips-iv.pdf) - Contains documentation for each MIPS instruction as well as documentation for MIPS CPUs in general.
    - [-O2 decompilation (for IDO 5.3 and 7.1)](https://hackmd.io/vPmcgdaFSlq4R2mfkq4bJg) - Guide for decompiling code compiled with IDO 5.3/7.1 with the -O2 flag enabled (the majority of the code here).
    - [N64 Developers' Manual, Version 5.2](https://jrra.zone/n64/doc/) - An online N64 developer manual (contains documentation for things like operating system functions and more).
    - [N64 Gfx Decompilation (F3DEX & S2DEX)](https://hackmd.io/m3E7g2YaSxiwUv9QKBLYyA) - Guide for decompiling N64 graphics code.
    - [IDO `cc` Manual](https://cocky-wescoff-177c47.netlify.app/cc_manual.html) - Manual for the IDO C compiler.
    - [C89 Draft](https://port70.net/~nsz/c/c89/c89-draft.html) - Useful to understand the subset of C required by IDO 5.3 compared to modern C.
- Other
    - [libreultra](https://github.com/n64decomp/libreultra/) - A matching decompilation of libultra.
    - [N64 development tools](https://github.com/glankk/n64) - Collection of tools including a F3DEX2 display list disassembler/decompiler.
