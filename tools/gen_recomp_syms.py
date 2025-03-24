import argparse
import shutil
from subprocess import check_output
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from typing import TextIO
import math
import os
from pathlib import Path
from pycparser import c_ast, c_generator, CParser

from dino.dll import DLL
from dino.dll_analysis import get_all_dll_functions
from dino.dll_tab import DLLTab
from dino.dlls_txt import DLLsTxt

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_PATH = Path("bin")
BUILD_PATH = Path("build")
SRC_DLLS_PATH = Path("src/dlls")

# vram -> function def
MANUAL_FUNCTION_DEFS = {
    # Static functions
    0x800813BC: { "name": "proutSyncPrintf", "size": 0x64 },

    # These functions are supposed to fallthrough to the next function
    0x80016178: { "name": "cos16_precise", "size": 0x60 },
    0x800161d8: { "name": "cos16", "size": 0x44 },
    0x800162f4: { "name": "fcos16", "size": 0x54 },

    # 0x8001B4F0-0x8001C8D4 is handwritten assembly that trips up recomp, split it into a bunch of
    # functions that don't technically exist to get recomp working. This will basically make problematic
    # conditional branches turn into conditional tail calls into these fake functions
    # Note: Some of the fake functions must overlap fake functions below them
    0x8001B4F0: { "name": "func_8001B4F0", "size": 0x200 },
    0x8001B6F0: { "name": "func_8001B6F0", "size": 0x510 },
    0x8001BC00: { "name": "func_8001BC00", "size": 0xF0 },
    0x8001BCF0: { "name": "func_8001BCF0", "size": 0x280 },
    0x8001BF70: { "name": "func_8001BF70", "size": 0x268 },
    0x8001C1D8: { "name": "func_8001C1D8", "size": 0x440 }, # synthetic
    0x8001C5A0: { "name": "func_8001C5A0", "size": 0x78 }, # synthetic
    0x8001C618: { "name": "func_8001C618", "size": 0x7C }, # synthetic
    0x8001C660: { "name": "func_8001C660", "size": 0x34 }, # synthetic
    0x8001C694: { "name": "func_8001C694", "size": 0x20C }, # synthetic
    0x8001C6C8: { "name": "func_8001C6C8", "size": 0x1D8 }, # synthetic
    0x8001C834: { "name": "func_8001C834", "size": 0x6C }, # synthetic
    0x8001C83C: { "name": "func_8001C83C", "size": 0x64 }, # synthetic
    0x8001C8A0: { "name": "func_8001C8A0", "size": 0x34 }, # synthetic
}

def gen_core_syms(syms_toml: TextIO, datasyms_toml: TextIO):
    text_start = 0x80000400
    text_size = 0x89350
    text_end = text_start + text_size
    segment_start = text_start
    segment_size = 0xA3AA0

    funcs = []
    vrams: dict[int, int] = {}
    data_globals = []

    # Grab functions/data from ROM .elf
    with open(BUILD_PATH.joinpath("dino.elf"), "rb") as file, \
         ELFFile(file) as elf:
        syms = elf.get_section_by_name(".symtab")
        assert isinstance(syms, SymbolTableSection)

        for sym in syms.iter_symbols():
            value = sym.entry["st_value"]
            size = sym.entry["st_size"]
            st_shndx = sym.entry["st_shndx"]
            st_info_type = sym.entry["st_info"]["type"]

            if sym.name.startswith("L8"):
                continue

            if value >= text_start and value < text_end:
                # Function

                # Apply size overrides
                manual_def = MANUAL_FUNCTION_DEFS.get(value)
                if manual_def != None:
                    assert manual_def["name"] == sym.name
                    size = manual_def["size"]
                    print("Overriding function {} definition to 0x{:X}".format(sym.name, size))
            
                func = { "name": sym.name, "vram": value, "size": size if size != 0 else None }

                idx = vrams.get(value)
                if idx == None:
                    vrams[value] = len(funcs)
                    funcs.append(func)
                elif funcs[idx]["name"].startswith("func_") or funcs[idx]["name"] == "":
                    funcs[idx] = func
                pass
            elif value >= text_end:
                # Data
                if (st_info_type == "STT_NOTYPE" and st_shndx == "SHN_ABS") or (st_info_type == "STT_OBJECT"):
                    data_global = { "name": sym.name, "vram": value }
                    data_globals.append(data_global)
    
    # Add any functions defined in the manual funcs table that weren't found in the .elf
    for (vram, mdef) in MANUAL_FUNCTION_DEFS.items():
        if vrams.get(vram) == None:
            func = { "name": mdef["name"], "vram": vram, "size": mdef["size"] }
            funcs.append(func)
            print("Adding manual function definition {} @ 0x{:X} with size 0x{:X}".format(mdef["name"], vram, mdef["size"]))
    
    # Sort
    funcs.sort(key=lambda f : f["vram"])
    data_globals.sort(key=lambda f : f["vram"])
    
    # Write function symbols
    syms_toml.write("[[section]]\n")
    syms_toml.write("name = \".segment\"\n")
    syms_toml.write("rom = 0x00001000\n")
    syms_toml.write("vram = 0x{:X}\n".format(segment_start))
    syms_toml.write("size = 0x{:X}\n".format(segment_size))
    syms_toml.write("\n")

    syms_toml.write("functions = [\n")
    i = 0
    for func in funcs:
        name = func["name"]
        vram = func["vram"]
        size = func["size"]

        if size == None:
            print(f"Inferring size for {name}")
            if i < len(funcs) - 1:
                size = funcs[i + 1]["vram"] - vram
            else:
                size = text_end - vram
        
        syms_toml.write("    {{ name = \"{}\", vram = 0x{:X}, size = 0x{:X} }},\n"
            .format(name, vram, size))

        i += 1
    syms_toml.write("]\n")

    # Write data symbols
    datasyms_toml.write("[[section]]\n")
    datasyms_toml.write("name = \".segment\"\n")
    datasyms_toml.write("rom = 0x00001000\n")
    datasyms_toml.write("vram = 0x{:X}\n".format(segment_start))
    datasyms_toml.write("size = 0x{:X}\n".format(segment_size))
    datasyms_toml.write("\n")

    datasyms_toml.write("symbols = [\n")
    for data_global in data_globals:
        name = data_global["name"]
        vram = data_global["vram"]
        
        datasyms_toml.write("    {{ name = \"{}\", vram = 0x{:X} }},\n"
            .format(name, vram))
    
    datasyms_toml.write("]\n")

GCC_PREPROCESS_CMD = [
    "gcc",
    "-nostdinc",
    "-E",
    "-D_LANGUAGE_C",
    "-D_MIPS_SZLONG=32",
    "-DF3DEX_GBI_2",
    "-Iinclude"
]

c_gen = c_generator.CGenerator()
c_parser = CParser()

def gen_dll_recomp_header(header: TextIO,
                          c_source_paths: list[Path],
                          symbol_renames: "dict[str, str]"):
    # Parse C source
    includes: list[str] = []
    declarations: list[tuple[str, str]] = []
    found_symbols: "set[str]" = set()

    for c_source_path in c_source_paths:
        # Get includes
        with open(c_source_path, "r", encoding="utf-8") as c_source:
            for line in c_source.readlines():
                line = line.strip()
                if line.startswith("#include"):
                    includes.append(line[line.index(" ") + 1:])

        # Get declarations
        preprocessed_text = check_output(GCC_PREPROCESS_CMD + [str(c_source_path)], universal_newlines=True)
        ast: c_ast.FileAST = c_parser.parse(preprocessed_text)

        for child in ast:
            if isinstance(child, c_ast.Decl):
                original = child.name
                if original in found_symbols:
                    continue
                rename = symbol_renames.get(original)
                if rename != None:
                    # Turn data decl into an extern
                    type = child.type
                    if isinstance(type, c_ast.ArrayDecl) or isinstance(type, c_ast.PtrDecl) or isinstance(type, c_ast.FuncDecl):
                        type = type.type
                    if not isinstance(type, c_ast.TypeDecl):
                        continue
                    child.name = rename
                    type.declname = child.name

                    child.storage = ["extern"]
                    child.init = None

                    decl = "{};".format(c_gen.visit(child))
                    alias = "#define {} {}".format(original, rename)

                    declarations.append((decl, alias))
                    found_symbols.add(original)
            elif isinstance(child, c_ast.FuncDef):
                original = child.decl.name
                if original in found_symbols:
                    continue
                rename = symbol_renames.get(original)
                if rename != None:
                    # Turn func def into an extern decl
                    type = child.decl.type.type
                    if isinstance(type, c_ast.ArrayDecl) or isinstance(type, c_ast.PtrDecl):
                        type = type.type
                    type.declname = rename

                    child.decl.storage = ["extern"]

                    decl = "{};".format(c_gen.visit(child.decl))
                    alias = "#define {} {}".format(original, rename)

                    declarations.append((decl, alias))
                    found_symbols.add(original)

    # Write
    header.write("#ifndef _DLL_29_INTERNAL_H\n")
    header.write("#define _DLL_29_INTERNAL_H\n\n")

    for include in includes:
        header.write(f"#include {include}\n")

    for decl, alias in declarations:
        header.write(f"\n{decl}\n")
        header.write(f"{alias}\n")

    header.write("\n#endif //_DLL_29_INTERNAL_H\n")

def scan_dll_elf(
        elf: ELFFile, 
        dll: DLL, 
        dll_vram: int,
        vrams_to_funcs: "dict[int, dict]",
        data_globals: list,
        symbol_renames: "dict[str, str]",
        dll_prefix: str):
    syms = elf.get_section_by_name(".symtab")
    assert isinstance(syms, SymbolTableSection)

    for sym in syms.iter_symbols():
        value = sym.entry["st_value"]
        size = sym.entry["st_size"]
        st_shndx = sym.entry["st_shndx"]
        st_info_type = sym.entry["st_info"]["type"]
        st_info_bind = sym.entry["st_info"]["bind"]

        maybe_unref_static_func = st_info_type == "STT_NOTYPE" and st_shndx == "SHN_ABS"

        if st_info_type != "STT_FUNC" and st_info_type != "STT_OBJECT" and not maybe_unref_static_func:
            continue
        if st_shndx == "SHN_UNDEF":
            continue
        if st_info_bind != "STB_LOCAL" and size == 0 and not maybe_unref_static_func:
            continue
        if value & 0x80000000:
            # External symbol reference, already covered by core syms
            continue

        if st_shndx == "SHN_ABS":
            sec_offset = dll.header.size
        else:
            section = elf.get_section(st_shndx).name
            if section == ".text":
                sec_offset = dll.header.size
            elif section == ".rodata":
                sec_offset = dll.header.rodata_offset
            elif section == ".data":
                sec_offset = dll.header.data_offset
            elif section == ".bss":
                sec_offset = dll.get_bss_offset()
            else:
                continue

        vram = dll_vram + sec_offset + value

        if st_info_type == "STT_FUNC" or maybe_unref_static_func:
            # Function
            func_info = vrams_to_funcs.get(vram)
            if func_info == None:
                print("Failed to find DLL {} func {} at {:0X} ({:0X})"
                    .format(dll.number, sym.name, vram, value))
            else:
                rename = dll_prefix + sym.name
                func_info["name"] = rename
                func_info["symbol"] = sym.name
                symbol_renames[sym.name] = rename
        elif st_info_type == "STT_OBJECT":
            # Data
            rename = dll_prefix + sym.name
            data_global = { 
                "name": rename, 
                "symbol": sym.name,
                "vram": vram
            }
            data_globals.append(data_global)
            symbol_renames[sym.name] = rename

def gen_dll_syms(syms_toml: TextIO, datasyms_toml: TextIO, dino_dlls_txt: TextIO, include_dir: Path):
    with open(BIN_PATH.joinpath("assets/DLLS_tab.bin"), "rb") as tab_file:
        tab = DLLTab.parse(tab_file.read())

    with open(BIN_PATH.joinpath("assets/DLLS.bin"), "rb") as dlls_file:
        dlls_data = bytearray(dlls_file.read())

    dlls_txt_path = SRC_DLLS_PATH.joinpath("dlls.txt")
    assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
    
    with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
        dlls_txt = DLLsTxt.parse(dlls_txt_file)

    dlls_rom_base = 0x38317CC
    dlls_vram_base = 0x81000000

    number = 1
    dll_rom = dlls_rom_base
    dll_vram = dlls_vram_base

    for entry in tab.entries:
        print(f"Generating DLL {number} symbols...")

        dll_data = dlls_data[entry.start_offset:entry.end_offset]
        assert len(dll_data) == entry.size

        funcs = []
        vrams_to_funcs: dict[int, dict] = {}
        data_globals = []
        symbol_renames: "dict[str, str]" = {}

        # Make sure the symbol names are globally unique
        # This isn't required in the decomp since DLLs are not linked with each other,
        # but this is the case for recomp.
        dll_prefix = f"__dll{number}_"
        
        # Grab functions by parsing the DLL contents
        dll = DLL.parse(dll_data, str(number))
        dll_functions = get_all_dll_functions(dll_data, dll)
        for func in dll_functions:
            func_info = { 
                "name": "{}func_{:X}".format(dll_prefix, func.address), 
                "symbol": None,
                "vram": dll_vram + dll.header.size + func.address, 
                "size": len(func.insts) * 4
            }
            funcs.append(func_info)
            vrams_to_funcs[func_info["vram"]] = func_info

        # If we have decomp set up for this DLL, grab function and data symbols from the .elf
        dll_dir = dlls_txt.path_map.get(number, None)
        if dll_dir != None:
            dll_elf_path = BUILD_PATH.joinpath(f"src/dlls/{dll_dir}/{number}.elf")
            if dll_elf_path.exists():
                with open(dll_elf_path, "rb") as file:
                    elf = ELFFile(file)
                    scan_dll_elf(
                        elf,
                        dll,
                        dll_vram,
                        vrams_to_funcs,
                        data_globals,
                        symbol_renames,
                        dll_prefix
                    )
                
                # Emit recomp header for DLL internals
                dll_src_path = SRC_DLLS_PATH.joinpath(dll_dir)
                c_source_paths = [p for p in dll_src_path.rglob("*.c")]
                if len(c_source_paths) > 0:
                    header_path = include_dir.joinpath(f"dlls/{dll_dir}_internal.h")
                    header_path.parent.mkdir(parents=True, exist_ok=True)
                    with open(header_path, "w", encoding="utf-8") as header:
                        gen_dll_recomp_header(
                            header,
                            c_source_paths,
                            symbol_renames
                        )

        
        funcs.sort(key=lambda f : f["vram"])
        data_globals.sort(key=lambda f : f["vram"])

        # Write dll .txt entry
        dino_dlls_txt.write(f".dll{number}\n")

        # Write functions
        syms_toml.write("\n")
        syms_toml.write("[[section]]\n")
        syms_toml.write(f"name = \".dll{number}\"\n")
        syms_toml.write("rom = 0x{:X}\n".format(dll_rom + dll.header.size))
        syms_toml.write("vram = 0x{:X}\n".format(dll_vram + dll.header.size))
        syms_toml.write("size = 0x{:X}\n".format(entry.size))
        syms_toml.write("got_address = 0x{:X}\n".format(dll_vram + dll.header.rodata_offset))
        syms_toml.write("\n")

        syms_toml.write("relocs = [\n")
        for reloc in dll.reloc_table.gp_relocations:
            syms_toml.write("    {{ type = \"R_MIPS_HI16\", vram = 0x{:X}, target_vram = 0x{:X} }},\n"
                .format(dll_vram + dll.header.size + reloc, dll_vram + dll.header.rodata_offset))
            syms_toml.write("    {{ type = \"R_MIPS_LO16\", vram = 0x{:X}, target_vram = 0x{:X} }},\n"
                .format(dll_vram + dll.header.size + reloc + 4, dll_vram + dll.header.rodata_offset))
        syms_toml.write("]\n")
        syms_toml.write("\n")

        syms_toml.write("functions = [\n")
        i = 0
        for func in funcs:
            name = func["name"]
            vram = func["vram"]
            size = func["size"]

            syms_toml.write("    {{ name = \"{}\", vram = 0x{:X}, size = 0x{:X} }},\n"
                .format(name, vram, size))
            i += 1
        syms_toml.write("]\n")

        # Write data symbols
        datasyms_toml.write("\n")
        datasyms_toml.write("[[section]]\n")
        datasyms_toml.write(f"name = \".dll{number}\"\n")
        datasyms_toml.write("rom = 0x{:X}\n".format(dll_rom + dll.header.size))
        datasyms_toml.write("vram = 0x{:X}\n".format(dll_vram + dll.header.size))
        datasyms_toml.write("size = 0x{:X}\n".format(entry.size))
        datasyms_toml.write("\n")

        datasyms_toml.write("symbols = [\n")
        for data_global in data_globals:
            name = data_global["name"]
            vram = data_global["vram"]
        
            datasyms_toml.write("    {{ name = \"{}\", vram = 0x{:X} }},\n"
                .format(name, vram))
    
        datasyms_toml.write("]\n")
        
        dll_rom += entry.size
        dll_vram += math.ceil((entry.size + entry.bss_size) / 4096) * 4096
        number += 1

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("-o", "--output", type=str, help="Directory to output the symbol and header files.", default=".")
    args = parser.parse_args()

    out_path = Path(args.output)

    out_syms_path = out_path.joinpath("dino.syms.toml").absolute()
    out_datasyms_path = out_path.joinpath("dino.datasyms.toml").absolute()
    out_dlls_txt_path = out_path.joinpath("dino.dlls.txt").absolute()
    out_include_path = out_path.joinpath("include/recomp").absolute()

    if out_include_path.exists():
        shutil.rmtree(out_include_path)

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    # Gen syms
    with open(out_syms_path, "w", encoding="utf-8") as syms_toml, \
         open(out_datasyms_path, "w", encoding="utf-8") as datasyms_toml, \
         open(out_dlls_txt_path, "w", encoding="utf-8") as dlls_txt:
        print("Generating core symbols...")
        gen_core_syms(syms_toml, datasyms_toml)
        print("Generating DLL symbols...")
        gen_dll_syms(syms_toml, datasyms_toml, dlls_txt, out_include_path)
    print("Done. Wrote symbols to {}".format(out_syms_path))

if __name__ == "__main__":
    main()
