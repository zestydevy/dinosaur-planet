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
- [Style guide](#style-guide)
- [Opening a pull request](#opening-a-pull-request)
- [Resources](#resources)


## How does decompilation work
The goal behind the decompilation is to extract and convert the game's code from MIPS machine code to human readable MIPS assembly and ideally C code. Once this code has been converted, the game can then be recompiled using the same compiler as the original game to produce an identical ROM. A identical recompiled ROM is important because it verifies that the converted code is, at least, functionally 100% the same as the original ROM.

At a high level, the process goes like this:
1. Disassemble the ROM's executable machine code into MIPS assembly code.
2. Import assembly code in C source files on a function-by-function basis using `#pragma GLOBAL_ASM(...)`.
3. Replace each `#pragma GLOBAL_ASM(...)` by decompiling the included assembly code into a C function.
4. Recompile the ROM. If the recompiled ROM matches the original ROM, the function decompiled in step 3 is correct!


## Things to know before contributing
Before working on the decompilation, you should (ideally):
- Have a decent understanding of the C89 programming language.
- Be at least a little familiar with the MIPS instruction set and/or assembly programming in general.
- Understand the basics of compilers and assemblers.
- Have some familiarity with compiler optimizations.

It is perfectly okay if you don't know all of this right away! Decomp can be fairly involved and it's normal to need to learn things as you go.


## How can I contribute?
The most important work that needs to be done currently is decompiling and understanding functions and data structures.

- If you are new to decomp, please see [our full introductory decompilation guide](docs/Guide.md)!
- If you are already familiar with decomp, you can find project documentation under the [docs](./docs) directory.


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
The following is a list of external resources that may be helpful:
- Tools
    - [decomp.me](https://decomp.me/) - Online space for decompiling individual functions with a live diff.
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
    - [Nintendo Ultra64 RSP Programmer’s Guide](https://ultra64.ca/files/documentation/silicon-graphics/SGI_Nintendo_64_RSP_Programmers_Guide.pdf) - Full documentation for the Nintendo 64 Reality Signal Processor.
    - [IDO `cc` Manual](https://cocky-wescoff-177c47.netlify.app/cc_manual.html) - Manual for the IDO C compiler.
    - [C89 Draft](https://port70.net/~nsz/c/c89/c89-draft.html) - Useful to understand the subset of C required by IDO 5.3 compared to modern C.
    - [MIPS System V Application Binary Interface](https://refspecs.linuxfoundation.org/elf/mipsabi.pdf) - Full documentation of the the MIPS ABI.
    - [MIPSpro™ Assembly Language Programmer's Guide](https://techpubs.jurassic.nl/library/manuals/2000/007-2418-001/pdf/007-2418-001.pdf) - IRIX Manual for writing MIPS assembly code.
    - [MIPS Compiling and Performance Tuning Guide](https://techpubs.jurassic.nl/library/manuals/2000/007-2479-001/pdf/007-2479-001.pdf) - IRIX manual for understanding things like assembler flags and internals.
- Other
    - [libreultra](https://github.com/n64decomp/libreultra/) - A matching decompilation of libultra. 
    - [N64 development tools](https://github.com/glankk/n64) - Collection of tools including a F3DEX2 display list disassembler/decompiler.
