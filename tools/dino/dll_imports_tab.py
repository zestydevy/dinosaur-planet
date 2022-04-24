import struct

class DLLImportsTab:
    """The full DLLIMPORTSTAB file."""
    def __init__(self, imports: "list[int]") -> None:
        self.imports = imports
        """A mapping of import index to base executable pointer. Each pointer
        refers to non-DLL VRAM (e.g. >=0x80000000)"""

    @staticmethod
    def parse(data: bytearray) -> "DLLImportsTab":
        imports: "list[int]" = []

        for i in range(0, len(data), 4):
            ptr = struct.unpack_from(">I", data, offset=i)[0]
            imports.append(ptr)

        return DLLImportsTab(imports)
