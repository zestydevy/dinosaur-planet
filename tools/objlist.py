#!/usr/bin/env python3
import argparse
from io import BufferedReader
import itertools
import os
from pathlib import Path
import struct
from typing import TypedDict

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

class Obj(TypedDict):
    id: int
    tabidx: int
    dll_num: int
    group: int
    name: str

def emit_header(objects: list[Obj]):
    dir = SCRIPT_DIR.joinpath("objh")
    dir.mkdir(exist_ok=True)

    objects.sort(key=lambda o: o["id"])
    with open(dir.joinpath("object_id.h"), "w", encoding="utf-8") as objects_md:
        objects_md.write("#ifndef _OBJECT_IDS_H\n")
        objects_md.write("#define _OBJECT_IDS_H\n")
        objects_md.write("\n")
        objects_md.write("enum ObjID {\n")
        for obj in objects:
            objects_md.write("    OBJ_{} = {},\n".format(
                obj["name"], obj["id"]))
        objects_md.write("};\n")
        objects_md.write("\n")
        objects_md.write("#endif\n")

def emit_markdown(objects: list[Obj]):
    dir = SCRIPT_DIR.joinpath("objmd")
    dir.mkdir(exist_ok=True)

    objects.sort(key=lambda o: o["id"])
    with open(dir.joinpath("objects.md"), "w", encoding="utf-8") as objects_md:
        objects_md.write("|ID|Tab Idx|DLL #|Group|Name|\n")
        objects_md.write("|--|-------|-----|-----|----|\n")
        for obj in objects:
            objects_md.write("|{}|{}|{}|{}|{}|\n".format(
                obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))
    
    objects.sort(key=lambda o: o["dll_num"])
    with open(dir.joinpath("objects_by_dll.md"), "w", encoding="utf-8") as objects_md:
        for key, group in itertools.groupby(objects, lambda o: o["dll_num"]):
            objs = list(group)
            objs.sort(key=lambda o: o["id"])
            objects_md.write("### DLL {}\n".format(key))
            objects_md.write("|ID|Tab Idx|DLL #|Group|Name|\n")
            objects_md.write("|--|-------|-----|-----|----|\n")
            for obj in objs:
                objects_md.write("|{}|{}|{}|{}|{}|\n".format(
                    obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))
            objects_md.write("\n")
    
    objects.sort(key=lambda o: o["group"])
    with open(dir.joinpath("objects_by_group.md"), "w", encoding="utf-8") as objects_md:
        for key, group in itertools.groupby(objects, lambda o: o["group"]):
            objs = list(group)
            objs.sort(key=lambda o: o["id"])
            objects_md.write("### Group {}\n".format(key))
            objects_md.write("|ID|Tab Idx|DLL #|Group|Name|\n")
            objects_md.write("|--|-------|-----|-----|----|\n")
            for obj in objs:
                objects_md.write("|{}|{}|{}|{}|{}|\n".format(
                    obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))
            objects_md.write("\n")

def list_objects(objects: list[Obj]):
    objects.sort(key=lambda o: o["id"])
    print("{:>5} {:>6} {:>5} {:>6} {}".format(
            "id", "tabidx", "dll", "group", "name"))
    for obj in objects:
        print("{:>5} {:>6} {:>5} {:>6} {}".format(
            obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))

def list_objects_by_tabidx(objects: list[Obj]):
    objects.sort(key=lambda o: o["tabidx"])
    print("{:>5} {:>6} {:>5} {:>6} {}".format(
            "id", "tabidx", "dll", "group", "name"))
    for obj in objects:
        print("{:>5} {:>6} {:>5} {:>6} {}".format(
            obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))

def list_objects_by_dll(objects: list[Obj]):
    objects.sort(key=lambda o: o["dll_num"])
    for key, group in itertools.groupby(objects, lambda o: o["dll_num"]):
        objs = list(group)
        objs.sort(key=lambda o: o["id"])
        print("{:>5} {:>6} {:>5} {:>6} {}".format(
            "id", "tabidx", "dll", "group", "name"))
        for obj in objs:
            print("{:>5} {:>6} {:>5} {:>6} {}".format(
                obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))
        print()

def list_objects_by_group(objects: list[Obj]):
    objects.sort(key=lambda o: o["group"])
    for key, group in itertools.groupby(objects, lambda o: o["group"]):
        objs = list(group)
        objs.sort(key=lambda o: o["id"])
        print("{:>5} {:>6} {:>5} {:>6} {}".format(
            "id", "tabidx", "dll", "group", "name"))
        for obj in objs:
            print("{:>5} {:>6} {:>5} {:>6} {}".format(
                obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["name"]))
        print()

def read_objects(objects_bin: BufferedReader,
         objects_tab: BufferedReader,
         objects_idx: BufferedReader) -> list[Obj]:
    tab_to_id: dict[int, int] = {}
    neg1_ids: list[int] = []
    objects: list[Obj] = []

    id = 0

    while True:
        data = objects_idx.read(2)
        if len(data) < 2:
            break

        tabidx = struct.unpack_from(">h", data)[0]
        if tabidx == -1:
            neg1_ids.append(id)
        else:
            tab_to_id[tabidx] = id
        id += 1
    
    tabidx = 0
    while True:
        data = objects_tab.read(4)
        if len(data) < 4:
            break

        offset = struct.unpack_from(">I", data)[0]

        objects_bin.seek(offset + 0x5f, os.SEEK_SET)
        str_bytes = objects_bin.read(16)

        if len(str_bytes) == 0:
            break
        id = tab_to_id[tabidx]

        objects_bin.seek(offset + 0x58, os.SEEK_SET)
        dll_id = struct.unpack_from(">H", objects_bin.read(2))[0]
        obj_group = struct.unpack_from(">h", objects_bin.read(2))[0]

        if dll_id >= 0x8000:
            dll_id = (dll_id - 0x8000) + 209
        elif dll_id >= 0x2000:
            dll_id = (dll_id - 0x2000) + 185 + 1
        elif dll_id >= 0x1000:
            dll_id = (dll_id - 0x1000) + 103 + 1

        name = str_bytes[:str_bytes.index(0)].decode("utf-8")

        objects.append({
            "id": id,
            "tabidx": tabidx,
            "dll_num": dll_id,
            "group": obj_group,
            "name": name
        })

        tabidx += 1
    
    return objects

def main():
    parser = argparse.ArgumentParser(description="Lists Dinosaur Planet object definitions.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("--by", type=str, choices=["dll", "group", "tabidx"], help="Group/sort by.")
    parser.add_argument("--markdown", action="store_true", default=False, help="Generate Markdown tables.")
    parser.add_argument("--header", action="store_true", default=False, help="Generate C header files.")

    args = parser.parse_args()

    # Do all path lookups from the base directory
    os.chdir(Path(args.base_dir).resolve())

    with open(BIN_ASSETS_DIR.joinpath("OBJECTS.bin"), "rb") as objects_bin, \
         open(BIN_ASSETS_DIR.joinpath("OBJECTS_tab.bin"), "rb") as objects_tab, \
         open(BIN_ASSETS_DIR.joinpath("OBJINDEX.bin"), "rb") as objects_idx:
        objects = read_objects(objects_bin, objects_tab, objects_idx)
    
    if args.markdown:
        emit_markdown(objects)
        return

    if args.header:
        emit_header(objects)
        return

    if args.by == "dll":
        list_objects_by_dll(objects)
    elif args.by == "group":
        list_objects_by_group(objects)
    elif args.by == "tabidx":
        list_objects_by_tabidx(objects)
    else:
        list_objects(objects)

if __name__ == "__main__":
    main()