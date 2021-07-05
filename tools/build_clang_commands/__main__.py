#!/usr/bin/env python3
"""Asks `make` to output all commands it would run, and generates `compile_commands.json`
from them.

This file's purpose is to tell the VSCode clangd extension how to compile the files,
so that it can do error checking, completion, etc.

Note that the paths must be absolute, so you'll need to run this script yourself
if you want to use this feature.
"""

import subprocess
import json
import os

def main():
    commands = []
    cwd = os.getcwd()

    # get make to output all the commands it would run
    proc = subprocess.run(["make", "--always-make", "--dry-run"],
        capture_output=True, check=True)
    
    # look at each command
    for line in proc.stdout.decode('utf-8').splitlines():
        cmd = line.split(' ', maxsplit=1)[0]
        if cmd in ('gcc', 'mips-linux-gnu-as'):
            # if it's compiling or assembling a file, create an entry
            cmd = line.split()

            # quick hack to remove options clang doesn't support
            line = line.replace('-fno-toplevel-reorder', '')
            # and prevent warnings for GLOBAL_ASM
            line = line.replace('-Wall ', '-Wall -Wno-unknown-pragmas ')

            commands.append({
                'directory': cwd,
                'command': line,
                'file': cmd[-1],
            })
    
    # output the entries
    with open('compile_commands.json', 'wt') as file:
        file.write(json.dumps(commands, sort_keys=True, indent=4))


if __name__ == "__main__":
    main()
