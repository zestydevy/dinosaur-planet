#!/usr/bin/env python3

import argparse
import os
import sys
import subprocess
import tempfile

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))

# Project-specific
CPP_FLAGS = [
    "-Iinclude",
    "-D_LANGUAGE_C",
    "-D_FINALROM",
    "-DF3DEX_GBI_2",
    "-D_MIPS_SZLONG=32",
    "-D__sgi",
    #"-DSCRIPT(...)={}"
    #"-D__attribute__(...)=",
    #"-D__asm__(...)=",
    #"-ffreestanding",
    "-DM2CTX",
    "-DPERMUTER",
]

def remove_big_comments(text: str):
    # Remove multiline block comments that start at the beginning of a line
    # and lines where the only content is a single line block comment.
    #
    # This greatly trims down the context file size while preserving things
    # such as struct field offset comments.
    i = 0
    while True:
        start = text.find("\n/*", i)
        if start == -1:
            break
        end = text.find("*/", start + 2)
        if end == -1:
            break

        i = end + 2
        comment = text[start + 1:end + 2]
        if "\n" in comment or text[end + 2] == "\n":
            text = text[0:start] + text[end + 2:]
            i = start
    return text

def import_c_file(in_file) -> str:
    in_file = os.path.relpath(in_file, root_dir)
    # -E = preprocess only
    # -CC = preserve comments
    # -P = dont generate #line directives
    # -dM = dump only #define directives
    cpp_command = ["gcc", "-E", "-CC", "-P", "-dD", *CPP_FLAGS, in_file]

    with tempfile.NamedTemporaryFile(suffix=".c") as tmp:
        stock_macros = subprocess.check_output(["gcc", "-E", "-P", "-dM", tmp.name], cwd=root_dir, encoding="utf-8")

    try:
        out_text = subprocess.check_output(cpp_command, cwd=root_dir, encoding="utf-8")
    except subprocess.CalledProcessError:
        print(
            "Failed to preprocess input file, when running command:\n"
            + " ".join(cpp_command),
            file=sys.stderr,
            )
        sys.exit(1)

    if not out_text:
        print("Output is empty - aborting")
        sys.exit(1)

    out_text = remove_big_comments(out_text)

    defines = {}
    source_lines = []
    for line in out_text.splitlines(keepends=True):
        if line.startswith("#define"):
            sym = line.split()[1].split("(")[0]
            defines[sym] = line
        elif line.startswith("#undef"):
            sym = line.split()[1]
            if sym in defines:
                del defines[sym]
        else:
            source_lines.append(line)

    for line in stock_macros.strip().splitlines():
        sym = line.split()[1].split("(")[0]
        if sym in defines:
            del defines[sym]

    return "".join(defines.values()) + "".join(source_lines)

def main():
    parser = argparse.ArgumentParser(
        description="""Create a context file which can be used for m2c / decomp.me"""
    )
    parser.add_argument(
        "c_file",
        help="""File from which to create context""",
    )
    args = parser.parse_args()

    output = import_c_file(args.c_file)

    with open(os.path.join(root_dir, "ctx.c"), "w", encoding="UTF-8") as f:
        f.write(output)


if __name__ == "__main__":
    main()
