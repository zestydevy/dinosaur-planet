from collections import OrderedDict
from typing import TextIO

class DLLsTxt:
    """
    Parser for src/dlls/dlls.txt

    Provides mappings for DLL numbers to their src directory relative
    to the src/dlls directory.
    """
    
    def __init__(self, 
                 lines: "list[tuple[str, int | None]]", 
                 path_map: "OrderedDict[int, str]"):
        self.__lines = lines
        self.path_map = path_map

    def set(self, number: int, path: str):
        self.path_map[number] = path
        line = f"{number} = {path}\n"

        # Best effort attempt at inserting the line while keeping the existing sort order
        idx = 0
        last_real_idx = 0
        for (_, line_dll) in self.__lines:
            if line_dll != None:
                if line_dll == number:
                    self.__lines[idx] = (line, number)
                    return
                elif line_dll > number:
                    self.__lines.insert(last_real_idx + 1, (line, number))
                    return

                last_real_idx = idx
                
            idx += 1
        
        self.__lines.append((line, number))

    def save(self, file: TextIO):
        for (line, _) in self.__lines:
            file.write(line)

    @staticmethod
    def parse(file: TextIO):
        path_map: "OrderedDict[int, str]" = {}
        lines: "list[tuple[str, int | None]]" = []

        for line in file.readlines():
            stripped = line.lstrip()
            if len(stripped) == 0 or stripped.isspace() or stripped.startswith("#"):
                lines.append((line, None))
                continue

            pair = stripped.split("=")
            if len(pair) == 2:
                number = int(pair[0].rstrip())
                path = pair[1].strip()
                path_map[number] = path
                lines.append((line, number))
            else:
                lines.append((line, None))
        
        return DLLsTxt(lines, path_map)

