import struct

class DLLTab:
    """The full DLLS.tab describing the location of each DLL."""
    def __init__(self,
                 bank0: "tuple[int, int]",
                 bank1: "tuple[int, int]",
                 bank2: "tuple[int, int]",
                 bank3: "tuple[int, int]",
                 entries: "list[DLLTabEntry]") -> None:
        self.bank0 = bank0
        self.bank1 = bank1
        self.bank2 = bank2
        self.bank3 = bank3
        self.banks = [bank0, bank1, bank2, bank3]
        self.bank_names = ["engine", "modgfx", "projgfx", "objects"]
        self.entries = entries

    @staticmethod
    def parse(data: bytearray):
        # Read header
        header = struct.unpack_from(">IIII", data, offset=0)
        
        # Read entries
        offset = 4 * 4
        entries: "list[DLLTabEntry]" = []
        file_size = len(data)

        while offset < file_size:
            # Note: The end offset is actually the start offset of the next entry.
            # The final entry is not actually a DLL, but its start offset is used as the end
            # for the second to last entry.
            start_offset, bss_size, end_offset = struct.unpack_from(">III", data, offset)

            # DLLS.tab ends with 16 bytes of 0xF
            if end_offset == 0xFFFF_FFFF:
                break

            entry = DLLTabEntry(start_offset, end_offset, bss_size)
            entries.append(entry)
            offset += 8
        
        return DLLTab(
            bank0=(0, header[0]),
            bank1=(header[0] + 1, header[1]),
            bank2=(header[1] + 1, header[3]),
            bank3=(header[3] + 1, len(entries) - 1),
            entries=entries
        )
            

class DLLTabEntry:
    """An individual DLLS.tab entry."""
    def __init__(self, 
                 start_offset: int,
                 end_offset: int, 
                 bss_size: int) -> None:
        self.start_offset = start_offset
        self.end_offset = end_offset
        self.size = end_offset - start_offset
        self.bss_size = bss_size
