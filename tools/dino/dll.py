import struct

class DLL:
    """A Dinosaur Planet DLL"""
    def __init__(self,
                 number: str,
                 size_aligned: int,
                 header: "DLLHeader",
                 reloc_table: "DLLRelocationTable") -> None:
        self.number = number
        self.size_aligned = size_aligned
        """The total size of this DLL (in bytes), 16-byte aligned"""
        self.header = header
        self.reloc_table = reloc_table

    def has_data(self) -> bool:
        """Returns whether this DLL has a .data section"""
        return self.header.data_offset != 0xFFFF_FFFF
    
    def has_rodata(self) -> bool:
        """Returns whether this DLL has a .rodata section"""
        return self.header.rodata_offset != 0xFFFF_FFFF

    def get_text_size(self) -> int:
        """Calculates the size (in bytes) of this DLL's .text section"""
        start = self.header.size

        if self.has_rodata():
            end = self.header.rodata_offset
        elif self.has_data():
            end = self.header.data_offset
        else:
            return ((self.size_aligned - start) // 16) * 16
        
        return end - start
    
    def get_rodata_size(self) -> int:
        """Calculates the size (in bytes) of this DLL's .rodata section, 
        excluding the GOT and relocation tables"""
        if not self.has_rodata():
            return 0

        start = self.header.rodata_offset + self.reloc_table.get_size()

        if self.has_data():
            end = self.header.data_offset
        else:
            return ((self.size_aligned - start) // 16) * 16

        return end - start
    
    def get_data_size(self) -> int:
        """Calculates the size (in bytes) of this DLL's .data section"""
        if not self.has_data():
            return 0
        
        start = self.header.data_offset
        end = self.size_aligned

        return ((end - start) // 16) * 16
    
    def get_size(self) -> int:
        """Calculates the total size (in bytes, unaligned) of this DLL"""
        return self.header.size + \
            self.get_text_size() + \
            self.get_rodata_size() + \
            self.reloc_table.get_size() + \
            self.get_data_size()

    def get_bss_offset(self) -> int:
        """Calculates the BSS offset (relative to start of header)"""
        return self.get_size()

    @staticmethod
    def parse(data: bytes, number: str):
        header = DLLHeader.parse(data)
        reloc_table = DLLRelocationTable.parse(data, header)
        dll = DLL(number, len(data), header, reloc_table)

        return dll

class DLLHeader:
    """The header section (including exports)"""
    def __init__(self, 
                 size: int,
                 data_offset: int,
                 rodata_offset: int,
                 export_count: int,
                 ctor_offset: int,
                 dtor_offset: int,
                 export_offsets: "list[int]") -> None:
        self.size = size
        """Header size in bytes"""
        self.data_offset = data_offset
        """DATA offset (relative to start of header or 0xFFFFFFFF if section is not present)"""
        self.rodata_offset = rodata_offset
        """RODATA offset (relative to start of header or 0xFFFFFFFF if section is not present)"""
        self.export_count = export_count
        """Number of exports"""
        self.ctor_offset = ctor_offset
        """Constructor offset (relative to end of header)"""
        self.dtor_offset = dtor_offset
        """Destructor offset (relative to end of header)"""
        self.export_offsets = export_offsets
        """List of exports (the offsets they specify)"""

    @staticmethod
    def parse(data: bytes) -> "DLLHeader":
        """Given a DLL file, parses and returns the header"""
        header_size = struct.unpack_from(">I", data, offset=0x0)[0]
        data_offsets = struct.unpack_from(">II", data, offset=0x4)
        export_count = struct.unpack_from(">H", data, offset=0xC)[0]
        ctor_dtor = struct.unpack_from(">II", data, offset=0x10)
        export_offsets = struct.unpack_from(">" + ("I" * export_count), data, offset=0x1C)
        
        return DLLHeader(
            size=header_size,
            data_offset=data_offsets[0],
            rodata_offset=data_offsets[1],
            export_count=export_count,
            ctor_offset=ctor_dtor[0],
            dtor_offset=ctor_dtor[1],
            export_offsets=list(export_offsets)
        )

class DLLRelocationTable:
    """The relocation table (including global offset table)"""
    def __init__(self,
                 exists: bool,
                 global_offset_table: "list[int]",
                 gp_relocations: "list[int]",
                 data_relocations: "list[int]") -> None:
        self.exists = exists
        self.global_offset_table = global_offset_table
        self.gp_relocations = gp_relocations
        self.data_relocations = data_relocations

    def get_size(self) -> int:
        """Calculates the size of the relocation table in bytes"""
        if not self.exists:
            return 0
        
        # +4 to include table section end markers
        return len(self.global_offset_table) * 4 + 4 \
            + len(self.gp_relocations) * 4 + 4 \
            + len(self.data_relocations) * 4 + 4

    @staticmethod
    def parse(data: bytes, header: DLLHeader) -> "DLLRelocationTable":
        """Given a DLL file, parses and returns the relocation section"""
        if header.rodata_offset == 0xFFFF_FFFF:
            # No relocation table
            return DLLRelocationTable(False, [], [], [])
        
        offset = header.rodata_offset
        global_offset_table: "list[int]" = []
        while offset < len(data) and (value := struct.unpack_from(">I", data, offset)[0]) != 0xFFFF_FFFE:
            global_offset_table.append(value)
            offset += 0x4
        
        offset += 0x4
        gp_relocations: "list[int]" = []
        while offset < len(data) and (value := struct.unpack_from(">I", data, offset)[0]) != 0xFFFF_FFFD:
            gp_relocations.append(value)
            offset += 0x4
        
        offset += 0x4
        data_relocations: "list[int]" = []
        while offset < len(data) and (value := struct.unpack_from(">I", data, offset)[0]) != 0xFFFF_FFFF:
            data_relocations.append(value)
            offset += 0x4
        
        return DLLRelocationTable(True, global_offset_table, gp_relocations, data_relocations)
