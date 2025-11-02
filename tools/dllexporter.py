import argparse
from dataclasses import dataclass
from dino.dlls_txt import DLLsTxt 
import os
from pathlib import Path
import sys
import re
from collections.abc import Callable

ROOT_PATH = Path(os.path.dirname(os.path.realpath(__file__))).parent
DLLS_PATH = ROOT_PATH.joinpath("src", "dlls")

DLL_NAME_OVERWRITES = {
    "1": "UI",
    "3": "animation"
}

DLL_BASE_EXPORTS = {
    "DLL": 0,
    "DLL_IModgfx": 1,
    "DLL_IObject": 7,
}

@dataclass
class DllExport:
    """
    This class encapsulate all data related to an exported DLL function
    """
    original_name: str # The name of the exported func
    index: int # The export index
    dll_id: str
    dll_name: str
    is_matched: bool
    return_type: str | None
    parameters: str | None

    def name(self) -> str:
        """
        Returns the name of the func without any dll related prefix like dll_, dll name, dll id or any leading _
        """
        prefixes = ["dll_", self.dll_id, self.dll_name, "_"]
        name = self.original_name
        for prefix in prefixes:
            if str.startswith(name, prefix):
                name = name[len(prefix):]

        return name
    
    def is_documented(self) -> bool:
        """
        An export is considered "documented" if it does not start with func_xxx
        """
        return not str.startswith(self.name(), "func_")
    
    def is_unknown(self) -> bool:
        return self.return_type is None or self.parameters is None

type ExportWriter = Callable[[DllExport], str]

def main():    
    parser = argparse.ArgumentParser(description="Generate the DLL interface for any given DLL.")
    parser.add_argument("id", help="The DLL ID to generate interface for.")
    parser.add_argument("--by-name", action="store_true", help="If given the exports will use the export name instead of index.")
    parser.add_argument("--no-interaction", action="store_true", help="If given the user won't be asked for confirmation on overwrites.")
    parser.add_argument("-b", "--base", default="DLL", help="If DLL header has no interface described yet the given base will be used to infer interface exports. Defaults to DLL. Will be ignored if DLL header has interface.")
    args = parser.parse_args()
    exports = read_exports(args.id)
    assert len(exports) > 0, "At least one export required"
    first_export = exports[0]

    export_writer: ExportWriter = export_func_by_index
    if args.by_name:
        export_writer = export_func_by_name
    export_writer = unknown_func_exporter(export_writer)
    export_writer = return_type_formatting_exporter(export_writer)

    dll_name = first_export.dll_id
    if first_export.dll_id != first_export.dll_name:
        dll_name = f"{first_export.dll_id}_{first_export.dll_name}"

    header_file = find_dll_header_file(args.id, dll_name)
    if header_file is None:
        print(f"Could not find header file for dll {args.id}, aborting")
        sys.exit(1)

    print(f"Reading .h file for DLL {first_export.dll_id}")
    existing_interface_def = None
    with open(header_file, "r", encoding="utf-8") as h_file:
        content = h_file.read()
        match = re.compile("DLL_INTERFACE_BASE\\((.*?)\\);\n([^}]+)};", re.VERBOSE | re.MULTILINE).search(content)
        base = None
        if match:
            base = match.group(1)
            existing_interface_def = match.group(2)

    if not existing_interface_def or not base:
        print("No DLL exports found in .h file")
        if args.base not in DLL_BASE_EXPORTS:
            print(f"Unknown DLL base '{args.base}' given")
        else:
            print(generate_interface_and_exports(dll_name, exports, export_writer, args.base))
        sys.exit(1)

    if base not in DLL_BASE_EXPORTS:
        print(f"Unknown DLL base '{base}' found")
        sys.exit(1)

    export_idx = DLL_BASE_EXPORTS[base]

    existing_interface_def = str.strip(existing_interface_def)
    out_def = existing_interface_def
    for line in str.splitlines(existing_interface_def):
        if len(exports) == export_idx:
            print("Too many exports in header file?")
            sys.exit(1)

        export = exports[export_idx]
        export_name = f"func{export.index}"
        if args.by_name or export.is_documented():
            export_name = export.name()

        # skip lines that do not contain the current export
        if not export_name in line:
            continue

        # Remove /* xx */ comments
        line = re.sub("/\\*.*?\\*/", "", line, 0)
        # Remove everything after //
        line = re.sub("//.*$", "", line, 0)
        # Remove trailing / leading whitespace
        line = str.strip(line)

        expected_export = export_writer(export)
        if expected_export in line:
            print(f"Found export {export.name()}")
        else:
            print(f"Found unexpected export definition")
            print(f"Expected:    {expected_export}")
            print(f"Found:       {line}")
            if args.no_interaction or ask_user_confirmation("Replace export?"):
                out_def = str.replace(out_def, line, expected_export)
        export_idx += 1

    if export_idx != len(exports):
        print(f"Warning: Expected to find {len(exports)} but only found {export_idx}")
        sys.exit(1)

    if out_def == existing_interface_def:
        print("No changes detected")
        sys.exit(0)

    print("Changes detected!")
    print("New DLL exports:")
    print(out_def)
    
    if args.no_interaction or ask_user_confirmation("Replace exports?"):
        with open(header_file, "r", encoding="utf-8") as h_file:
            content = h_file.read()
        content = str.replace(content, existing_interface_def, out_def)
        with open(header_file, "w", encoding="utf-8") as h_file:
            h_file.write(content)

def ask_user_confirmation(question: str) -> bool:
    while True:
        answer = str.lower(input(f"{question} (y/n/q) "))
        if "q" in answer:
            sys.exit(0)
        if "y" in answer:
            return True
        if "n" in answer:
            return False
        print("Invalid answer given. One of y/n/q is required")

def find_dll_header_file(dll_id: str, dll_name: str) -> Path | None:
    header_files = list(ROOT_PATH.glob(f"include/**/{dll_name}.h", case_sensitive=False))
    if len(header_files) == 0:
        # header file might be dll id?
        header_files = list(ROOT_PATH.glob(f"include/**/{dll_id}.h", case_sensitive=False))
        if len(header_files) == 0:
            return None
    return header_files[0]

def unknown_func_exporter(decorated: ExportWriter) -> ExportWriter:
    def writer(export: DllExport) -> str:
        if export.is_unknown():
            return f"UnknownDLLFunc func{export.index}"
        return decorated(export)
    
    return writer

def return_type_formatting_exporter(decorated: ExportWriter) -> ExportWriter:
    def writer(export: DllExport) -> str:
        out = decorated(export)
        out = str.replace(out, "* (*", "*(*")
        return out
    
    return writer

def export_func_by_index(export: DllExport) -> str:
    if export.is_documented():
        return export_func_by_name(export)

    return f"{export.return_type} (*func{export.index})({export.parameters});"

def export_func_by_name(export: DllExport) -> str:
    return f"{export.return_type} (*{export.name()})({export.parameters});"

def generate_interface_and_exports(dll_name: str, exports: list[DllExport], export_writer: ExportWriter, base: str) -> str:
    assert base in DLL_BASE_EXPORTS, f"Unknown base {base}"
    export_idx = DLL_BASE_EXPORTS[base]
    exports = exports[export_idx:]
    print("Inferred exports / interface:")
    interface = """
DLL_INTERFACE(DLL_%s) {
/*:*/ DLL_INTERFACE_BASE(%s);
""" % (dll_name, base)
    
    for export in exports:
        interface += f"/*{export.index}*/ {export_writer(export)}\n"
    interface += "};"

    return interface

def read_exports(dll_id: str) -> list[DllExport]:
    dlls_txt_path = DLLS_PATH.joinpath("dlls.txt")
    assert dlls_txt_path.exists(), f"Missing dlls.txt file at {dlls_txt_path.absolute()}"
    
    with open(dlls_txt_path, "r", encoding="utf-8") as dlls_txt_file:
        dlls_txt = DLLsTxt.parse(dlls_txt_file)

    # Ensure the DLL isn't already set up
    dll_dir = dlls_txt.path_map.get(int(dll_id), None)

    if dll_dir == None:
        print(f"DLL {dll_id} not found")
        sys.exit(1)

    dll_dir = DLLS_PATH.joinpath(dll_dir)
    
    if dll_id in DLL_NAME_OVERWRITES:
        dll_name = DLL_NAME_OVERWRITES[dll_id]
    else:
        underscore_idx = str.find(dll_dir.name, "_")
        # if the dir has an underscore it's gonna be <dll_id>_name, otherwise just use the id as name
        if underscore_idx != -1:
            dll_name = dll_dir.name[underscore_idx + 1:]
        else:
            dll_name = dll_id

    exports_file_path = dll_dir.joinpath("exports.s")
    exports_in_file: list[str] = []
    print(f"Reading exports for DLL {dll_id}")
    with open(exports_file_path, "r", encoding="utf-8") as exports_file:
        for line in exports_file.readlines():
            if ".dword" not in line:
                continue

            line = str.strip(line)
            # Skip con-/destructor
            if str.endswith(line, "_dtor") or str.endswith(line, "_ctor"):
                continue
            # Line contains ".dword dll_x_func_y"
            dword_idx = str.find(line, ".dword")
            assert dword_idx != -1, "dword should exist in line"
            exports_in_file.append(line[dword_idx + 7:])


    print(f"Found {len(exports_in_file)} exports")

    print(f"Reading .c file for DLL {dll_id}")
    c_file = read_dll_c_file(dll_dir)
    if c_file is None or c_file == "":
        print(f"Failed to read DLL .c file, aborting")
        sys.exit(1)
    
    print(f"Finding exports in .c file")
    exports: list[DllExport] = []
    for i, export in enumerate(exports_in_file):
        global_asm_pragma = f'#pragma GLOBAL_ASM("asm/nonmatchings/dlls/{dll_dir}/{export}.s")'
        is_matched = global_asm_pragma not in c_file

        signature = find_function_signature(c_file, export)
        (return_type, parameters) = (None, None)
        if signature:
            (return_type, parameters) = (signature['return_type'], signature['parameters'])
            pointer_idx = str.find(return_type, "*")
            if pointer_idx != -1 and " " not in return_type:
                return_type = f"{return_type[:pointer_idx]} *"

            parameters = " ".join(parameters.split())

        exports.append(DllExport(export, i, dll_id, dll_name, is_matched, return_type, parameters))
    return exports

def read_dll_c_file(dll_dir: Path):
    c_files = list(Path.glob(dll_dir, "*.c"))
    if len(c_files) > 1:
        return None

    if len(c_files) == 0:
        return None

    with open(c_files[0], "r", encoding="utf-8") as c_file:
        return c_file.read()
    
    return None

def find_function_signature(code, function_name):
    """Find a function signature in C code for a given function name."""
    regex = get_simple_function_signature_regex(function_name)
    match = regex.search(code)
    
    if match:
        return_type = match.group(1).strip()
        func_name = match.group(2)
        parameters = match.group(3).strip()
        
        return {
            'return_type': return_type,
            'function_name': func_name,
            'parameters': parameters,
        }
    return None

def get_simple_function_signature_regex(function_name):
    """
    Simpler version that works for most common function signatures.
    May fail with complex nested parentheses in parameters.
    """
    pattern = r"""
        ^\s*                          # Start of line, optional whitespace
        ([\w\s\*]+?)                  # Return type - group 1
        \s*                           # Whitespace
        (""" + re.escape(function_name) + r""")  # Function name - group 2
        \s*                           # Optional whitespace
        \(                            # Opening parenthesis
        ([^)]*)                       # Parameters (everything until closing paren) - group 3
        \)                            # Closing parenthesis
    """
    return re.compile(pattern, re.VERBOSE | re.MULTILINE)

if __name__ == "__main__":
    main()