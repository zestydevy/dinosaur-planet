[[← back]](README.md)

# Recompilation
The goal of this document is the explain the full process of extracting code and data from the original ROM and then recompiling it back into a new ROM.


## Contents
- [Overview](#overview)
- [ROM Extraction](#rom-extraction)
- [Building](#building)


## Overview
A high level overview of the extraction and build process is as follows:

1. "Split" out readable assembly code and binary data from the original ROM using tools such as [splat](https://github.com/ethteck/splat).
2. Generate a linker script capable of stitching all code and assets back together into a new ROM at the exact same addresses they were extracted from.
3. Reverse extracted assembly code into C.
4. Recompile C code using IDO 5.3 (IRIS Development Option, the original compiler used for the game), reassemble leftover assembly, and convert binary data all into individual object files.
5. Link all resulting object files into a new ROM.

Through this process, code can be incrementally decompiled into C and then constantly recompiled into a full ROM to be compared against the original ROM. If everything was decompiled accurately, the ROMs will be byte-for-byte identical.


## ROM Extraction

### splat
Code and data are extracted from the original ROM primarily using a tool called splat. At the root of the repository is the file `splat.yaml`, which tells splat exactly what address ranges should be extracted and to which files.

With the exception of DLLs, each C file under `src` and binary file under `bin` are mapped to an address range in `splat.yaml`. Additionally, each file has a 'type' that tells splat what kind of data it is (ex. assembly code, C code, binary data, .data, .rodata, etc.).

**Note:** Running `./dino.py extract` will re-run splat, fully rewriting the `asm` and `bin` directories and creating new `.c` stubs under `src` if necessary.

Please see [splat's full documentation for more information](https://github.com/ethteck/splat).

### DLLs
Dinosaur Planet DLLs are in a special format that splat isn't able to simply extract. Instead, splat is configured (by `splat.yaml`) to extract all DLLs to a single `DLLS.bin` file (found under `bin/assets`). This file is then unpacked by the tool `dino_dll.py` (found under `tools`). Each unpacked DLL can be found under `bin/assets/dlls`.

While splat is used to extract the DLL binaries, it does not extract assembly and data from them. Instead, this is done with the `tools/dll_split.py` script which provides somewhat similar functionality. For each DLL directory under `src/dlls` there is a `<dll number>.yaml` file that allows extraction to be configured.

### Linker Script
In addition to extracting code and data, splat is also responsible for generating a linker script that is capable of re-linking all extracted files back into a ROM in the exact same order as the original. After running `./dino.py extract`, this can be found at `dino.ld` in the repository root.

The linker script is vital for creating matching ROMs because it places data from each object file (`.o`) at its original address in the new ROM.

As DLLs are linked separately, they use a different linker script found at `src/dlls/dll.ld`. Some DLLs override this with their own linker script found in their `src` directory.

### Symbols
While extracting assembly code, splat will also generate files containing symbols for global variables and functions that it comes across. These symbols will initially have auto-generated names like `func_<address>` and `D_<address>`. Automatically detected variables are placed in `undefined_syms_auto.txt` and functions in `undefined_funcs_auto.txt` (found in the repository root after splat is ran).

To give these symbols actual names, the file `symbol_addrs.txt` is used. Splat reads this file while extracting and will use the names given here for addresses it thinks are variables or functions.

Sometimes, splat won't be able to detect an address as a proper symbol. This usually happens when the address isn't referenced explicitly in assembly. For addresses that should have a symbol, the `undefined_syms.txt` and `undefined_funcs.txt` files can be used to define things like variables and functions respectively.

DLLs however, do not use any of the above files for symbols. Instead, each DLL has their own `syms.txt` file found in their `src/dlls/<dll number>` directory. This file follows the same syntax as the above symbol files. These DLL symbol files will often contain entries for functions/globals actually defined in the core code. In those cases, the address in the DLL symbols file will not be the absolute address of that symbol, and instead will be a 1-based index into the DLLSIMPORT.tab file. The index will also have the 32nd bit set (0x80000000).


## Building
Rebuilding all of the extracted and decompiled code/data back into a ROM is done generally in two steps:
1. Compiling all source and binary files into object files (`.o`).
2. Linking all object files into a final ROM.

### Build System
[Ninja](https://ninja-build.org/) is the build system currently used by the project. It's responsible for executing every command necessary to build the full ROM. Instead of maintaining a complex build script for the project, one is generated by the custom script `tools/configure.py`. This script scans every directory for source files, binary files, and linker scripts and decides each step needed to convert them into a single ROM. These steps are then written to a Ninja build script (`build.ninja`), which can be found in the repository root after running `./dino.py configure` (or `build` for the first time).

### C Files
C files are compiled in a three-part process:

1. For partially decompiled files, [asm-processor](https://github.com/simonlindholm/asm-processor) pre-processes the file looking for `GLOBAL_ASM` pragmas. Temporary stubs are created for each block of included assembly.
2. The `.c` file is compiled using IDO into an object file (`.o`).
    - Note: IDO (IRIS Development Option) is a set of compiler tools developed for the IRIX operating system and do not run natively on x86. This project uses a [recompiled version of each program](https://github.com/Emill/ido-static-recomp), which can be found under `tools/ido_recomp`.
3. Finally, asm-processor post-processes the resulting object file and patches in the actual included assembly code.

### DLLs
DLLs are a special exception. Instead of being linked directly into the final ROM, each DLL must be linked individually, converted to Dinosaur Planet's DLL format (`.dll`), and finally packed into `DLLS.bin` (before `DLLS.bin` is converted to an object file). The process is as follows:

1. Compile the C source and `exports.s` file into object files.
2. Using a special linker script (found at `src/dlls/dll.ld`), link each object file into an ELF file.
3. Using `elf2dll` (found at `tools/elf2dll`), convert the ELF file to Dinosaur Planet's DLL format (`.dll`).
4. Finally, after all decompiled DLLs have been recompiled, repack all DLLs into a new `DLLS.bin` file using `dino_dll.py` (found at `tools/dino_dll.py`).

After all that, `DLLS.bin` is linked into the file ROM like a normal binary file.

#### DLL Exports
Despite the name, the `exports.s` file actually contains both the DLL's export table **and** the names of the DLL's constructor and destructor. The order of entries in this file must be: the constructor, the destructor, and then each export table entry in the same order as in the original DLL file.

### Assembly Files
All remaining assembly files (`.s`, those not found under `asm/nonmatchings`) are simply assembled using a modern assembler into individual object files.

### Binary Files
All binary files are simply linked into individual object files for the sake of being able to link them into the final ROM.

### Linking
The very final step of building the ROM is linking every object file together into a resulting ELF (the linker script can be found at `dino.ld` in the repository root). The ELF file is then converted to a generic binary file (removing ELF headers), which is the final ROM. 
