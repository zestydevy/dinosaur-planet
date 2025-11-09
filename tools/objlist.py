#!/usr/bin/env python3
import argparse
from io import BufferedReader
import itertools
import os
from pathlib import Path
import struct
from typing import Iterable, TypedDict

SCRIPT_DIR = Path(os.path.dirname(os.path.realpath(__file__)))
BIN_ASSETS_DIR = Path("bin/assets")

class Obj(TypedDict):
    id: int
    tabidx: int
    dll_num: int
    dll_id: int
    group: int
    name: str
    folder: str

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

def print_objects(objects: Iterable[Obj], dll_ids: bool):
    print("{:>5} {:>6} {:>6} {:>6} {}".format(
            "id", "tabidx", "dll", "group", "folder", "name"))
    for obj in objects:
        if dll_ids:
            id_str = "0x{:X}".format(obj["dll_id"])
            print("{:>5} {:>6} {:>6} {:>6} {:>6} {}".format(
                obj["id"], obj["tabidx"], id_str, obj["group"], obj["folder"], obj["name"]))
        else:
            print("{:>5} {:>6} {:>6} {:>6} {:>6} {}".format(
                obj["id"], obj["tabidx"], obj["dll_num"], obj["group"], obj["folder"], obj["name"]))

def list_objects(objects: list[Obj], sort_key: str, dll_ids: bool):
    objects.sort(key=lambda o: o[sort_key])
    print_objects(objects, dll_ids)

def list_grouped(objects: list[Obj], group_key: str, dll_ids: bool):
    objects.sort(key=lambda o: o[group_key])
    for key, group in itertools.groupby(objects, lambda o: o[group_key]):
        objs = list(group)
        objs.sort(key=lambda o: o["id"])
        print_objects(objs, dll_ids)
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
        objects_bin.seek(offset + 0x6f, os.SEEK_SET)
        folder = struct.unpack_from(">B", objects_bin.read(1))[0]

        if dll_id >= 0x8000:
            dll_num = (dll_id - 0x8000) + 209
        elif dll_id >= 0x2000:
            dll_num = (dll_id - 0x2000) + 185 + 1
        elif dll_id >= 0x1000:
            dll_num = (dll_id - 0x1000) + 103 + 1
        else:
            dll_num = dll_id

        name = str_bytes[:str_bytes.index(0)].decode("utf-8")

        objects.append({
            "id": id,
            "tabidx": tabidx,
            "dll_num": dll_num,
            "dll_id": dll_id,
            "group": obj_group,
            "name": name,
            "folder": folder
        })

        tabidx += 1
    
    return objects

def main():
    parser = argparse.ArgumentParser(description="Lists Dinosaur Planet object definitions.")
    parser.add_argument("--base-dir", type=str, dest="base_dir", help="The root of the project.", default=str(SCRIPT_DIR.joinpath("..")))
    parser.add_argument("--by", type=str, choices=["dll", "group", "tabidx", "id", "folder"], help="Group/sort by.")
    parser.add_argument("--id", type=str, help="Filter by object ID.")
    parser.add_argument("--tabidx", type=str, help="Filter by object tab index.")
    parser.add_argument("--dll", type=str, help="Filter by DLL.")
    parser.add_argument("--group", type=str, help="Filter by object group.")
    parser.add_argument("--name", type=str, help="Filter by object name.")
    parser.add_argument("--folder", type=str, help="Filter by folder.")
    parser.add_argument("--dll-id", action="store_true", default=False, dest="dll_id", 
                        help="Use DLL IDs instead of tab indexes. Affects filter inputs and display.")
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
    
    if args.id != None:
        id = int(args.id, base=0)
        objects = [o for o in objects if o["id"] == id]
    if args.tabidx != None:
        tabidx = int(args.tabidx, base=0)
        objects = [o for o in objects if o["tabidx"] == tabidx]
    if args.dll != None:
        dll = int(args.dll, base=0)
        if args.dll_id:
            objects = [o for o in objects if o["dll_id"] == dll]
        else:
            objects = [o for o in objects if o["dll_num"] == dll]
    if args.group != None:
        group = int(args.group, base=0)
        objects = [o for o in objects if o["group"] == group]
    if args.folder != None:
        folder = int(args.folder, base=0)
        objects = [o for o in objects if o["folder"] == folder]
    if args.name != None:
        name = args.name.lower()
        objects = [o for o in objects if name in o["name"].lower()]

    if args.by == "dll":
        list_grouped(objects, group_key="dll_num", dll_ids=args.dll_id)
    elif args.by == "group":
        list_grouped(objects, group_key="group", dll_ids=args.dll_id)
    elif args.by == "folder":
        list_grouped(objects, group_key="folder", dll_ids=args.dll_id)
    elif args.by == "tabidx":
        list_objects(objects, sort_key="tabidx", dll_ids=args.dll_id)
    else:
        list_objects(objects, sort_key="id", dll_ids=args.dll_id)

if __name__ == "__main__":
    main()