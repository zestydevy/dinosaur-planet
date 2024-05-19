import argparse
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
import os
from pathlib import Path

BUILD_PATH = Path("build")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project (default=..).", default="..")
    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    textStart = 0x80000400
    textSize = 0x89350
    textEnd = textStart + textSize

    funcs = []
    vrams: dict[int, int] = {}

    with open(BUILD_PATH.joinpath("dino.elf"), "rb") as file:
        elf = ELFFile(file)
        syms = elf.get_section_by_name(".symtab")
        assert isinstance(syms, SymbolTableSection)

        for sym in syms.iter_symbols():
            value = sym.entry["st_value"]
            size = sym.entry["st_size"]

            if value < textStart or value >= textEnd:
                continue

            if sym.name.startswith("L8"):
                continue
        
            func = { "name": sym.name, "vram": value, "size": size if size != 0 else None }

            idx = vrams.get(value)
            if idx == None:
                vrams[value] = len(funcs)
                funcs.append(func)
            elif funcs[idx]["name"].startswith("func_") or funcs[idx]["name"] == "":
                funcs[idx] = func
    
    funcs.sort(key=lambda f : f["vram"])
    
    with open("dino.syms.core.toml", "w", encoding="utf-8") as syms_toml:
        syms_toml.write("[[section]]\n")
        syms_toml.write("name = \".segment\"\n")
        syms_toml.write("rom = 0x00001000\n")
        syms_toml.write("vram = 0x{:X}\n".format(textStart))
        syms_toml.write("size = 0x{:X}\n".format(textSize))
        syms_toml.write("\n")

        syms_toml.write("functions = [\n")
        i = 0
        for func in funcs:
            size = func["size"]
            if size == None:
                print(f"Inferring size for {func['name']}")
                if i < len(funcs) - 1:
                    size = funcs[i + 1]["vram"] - func["vram"]
                else:
                    size = textEnd - func["vram"]
            
            syms_toml.write("    {{ name = \"{}\", vram = 0x{:X}, size = 0x{:X} }},\n"
                    .format(func["name"], func["vram"], size))

            i += 1
        syms_toml.write("]\n")

if __name__ == "__main__":
    main()
