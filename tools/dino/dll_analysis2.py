from io import BytesIO
import spimdisasm
import rabbitizer

from .dll import DLL
from .dll_syms_txt import DLLSymsTxt

# Arbitrary base VRAM address to use when analyzing DLLs
DLL_VRAM_BASE = 0x81000000
__STANDARD_MIPS_GP_PROLOGUE = bytearray([
    0x3C, 0x1C, 0x00, 0x00, # lui     gp,0x0
    0x27, 0x9C, 0x00, 0x00, # addiu   gp,gp,0x0
    0x03, 0x99, 0xE0, 0x21  # addu    gp,gp,t9
])

class AnalyzedDLL:
    def __init__(self,
                 meta: DLL,
                 number: int,
                 bytes: bytes,
                 bss_size: int | None,
                 context: spimdisasm.common.Context,
                 text: spimdisasm.mips.sections.SectionText,
                 rodata: spimdisasm.mips.sections.SectionRodata | None,
                 data: spimdisasm.mips.sections.SectionData | None,
                 bss: spimdisasm.mips.sections.SectionBss | None,
                 functions: list[spimdisasm.mips.symbols.SymbolFunction],
                 function_rodata: list[spimdisasm.mips.FunctionRodataEntry]) -> None:
        self.meta = meta
        self.number = number
        self.bytes = bytes
        self.bss_size = bss_size
        self.context = context
        self.text = text
        self.rodata = rodata
        self.data = data
        self.bss = bss
        self.functions = functions
        self.function_rodata = function_rodata

def analyze_dll(dll: DLL, dll_number: int, dll_bytes: bytes, symbol_files: list[DLLSymsTxt], bss_size: int | None) -> AnalyzedDLL:
    # Setup global config for rabbitizer and spimdisasm
    __set_rabbitizer_config()
    __set_spimdisasm_config(__get_dll_gp(dll))

    # Preprocess DLL data for spimdisasm
    #
    # spimdisasm doesn't understand Dinosaur Planet's custom $gp prologue format
    if dll.reloc_table.exists and len(dll.reloc_table.gp_relocations) > 0:
        dll_bytes = __rewrite_to_standard_gp_prologues(dll, dll_bytes)

    # Create spimdisasm context
    context = __make_spimdisasm_context(dll, dll_bytes, bss_size)

    # Create spimdisasm sections
    text, rodata, data, bss = __init_sections(context, dll, dll_bytes, bss_size)

    # Infer symbols from GOT and reloc tables
    __define_symbols_from_reloc_table(dll, context.globalSegment, text, rodata, data, bss)

    # Define known symbol names
    for symbol_file in symbol_files:
        __define_known_symbols(symbol_file, context.globalSegment, text, rodata, data, bss)

    # Run analysis
    text.analyze()
    if rodata != None:
        rodata.analyze()
    if data != None:
        data.analyze()
    if bss != None:
        bss.analyze()

    # Set custom symbol name defaults
    __set_symbol_name_defaults(dll, dll_number, context.globalSegment, text, rodata, data, bss)

    # Make function list
    functions: list[spimdisasm.mips.symbols.SymbolFunction] = []
    for sym in text.symbolList:
        if sym.isFunction():
            assert(isinstance(sym, spimdisasm.mips.symbols.SymbolFunction))
            functions.append(sym)

    # Migrate .rodata
    function_rodata =  spimdisasm.mips.FunctionRodataEntry.getAllEntriesFromSections(text, rodata)

    return AnalyzedDLL(dll, dll_number, dll_bytes, bss_size, context, text, rodata, data, bss, functions, function_rodata)
    
def __set_symbol_name_defaults(
        dll: DLL,
        dll_number: int,
        global_segment: spimdisasm.common.SymbolsSegment,
        text: spimdisasm.mips.sections.SectionText,
        rodata: spimdisasm.mips.sections.SectionRodata | None,
        data: spimdisasm.mips.sections.SectionData | None,
        bss: spimdisasm.mips.sections.SectionBss | None):
    for sym in global_segment.symbols.values():
        if sym.vram < DLL_VRAM_BASE and (sym.vram & 0x8000_0000) != 0:
            sym.setNameIfUnset("IMPORT_{:X}".format(sym.vram))
        elif sym.sectionType == spimdisasm.common.FileSectionType.Text:
            if (sym.vram - DLL_VRAM_BASE) == dll.header.ctor_offset:
                sym.setNameIfUnset("dll_{}_ctor".format(dll_number))
            elif (sym.vram - DLL_VRAM_BASE) == dll.header.dtor_offset:
                sym.setNameIfUnset("dll_{}_dtor".format(dll_number))
            else:
                sym.setNameIfUnset("dll_{}_func_{:X}".format(dll_number, sym.vram - text.vram))
        elif sym.sectionType == spimdisasm.common.FileSectionType.Rodata and rodata != None:
            if sym.isJumpTable():
                sym.setNameIfUnset("jtbl_{:X}".format(sym.vram - rodata.vram))
            elif sym.isString():
                sym.setNameIfUnset("str_{:X}".format(sym.vram - rodata.vram))
            else:
                sym.setNameIfUnset("_rodata_{:X}".format(sym.vram - rodata.vram))
        elif sym.sectionType == spimdisasm.common.FileSectionType.Data and data != None:
            sym.setNameIfUnset("_data_{:X}".format(sym.vram - data.vram))
        elif sym.sectionType == spimdisasm.common.FileSectionType.Bss and bss != None:
            sym.setNameIfUnset("_bss_{:X}".format(sym.vram - bss.vram))

def __define_known_symbols(symbols_file: DLLSymsTxt,
                           global_segment: spimdisasm.common.SymbolsSegment,
                           text: spimdisasm.mips.sections.SectionText,
                           rodata: spimdisasm.mips.sections.SectionRodata | None,
                           data: spimdisasm.mips.sections.SectionData | None,
                           bss: spimdisasm.mips.sections.SectionBss | None):
    
    for address, name in symbols_file.absolute.items():
        sym = global_segment.addSymbol(address)
        sym.name = name

    text_symbols = symbols_file.sections.get(".text")
    if text_symbols != None:
        for address, name in text_symbols.syms.items():
            sym = text.addSymbol(DLL_VRAM_BASE + text_symbols.offset + address)
            sym.name = name

    if rodata != None:
        rodata_symbols = symbols_file.sections.get(".rodata")
        if rodata_symbols != None:
            for address, name in rodata_symbols.syms.items():
                sym = rodata.addSymbol(DLL_VRAM_BASE + rodata_symbols.offset + address)
                sym.name = name
    
    if data != None:
        data_symbols = symbols_file.sections.get(".data")
        if data_symbols != None:
            for address, name in data_symbols.syms.items():
                sym = data.addSymbol(DLL_VRAM_BASE + data_symbols.offset + address)
                sym.name = name
    
    if bss != None:
        bss_symbols = symbols_file.sections.get(".bss")
        if bss_symbols != None:
            for address, name in bss_symbols.syms.items():
                sym = bss.addSymbol(DLL_VRAM_BASE + bss_symbols.offset + address)
                sym.name = name

def __define_symbols_from_reloc_table(
        dll: DLL,
        global_segment: spimdisasm.common.SymbolsSegment,
        text: spimdisasm.mips.sections.SectionText,
        rodata: spimdisasm.mips.sections.SectionRodata | None,
        data: spimdisasm.mips.sections.SectionData | None,
        bss: spimdisasm.mips.sections.SectionBss | None):
    if not dll.reloc_table.exists:
        return
    
    # Known functions
    for gp_reloc in dll.reloc_table.gp_relocations:
        text.addSymbol(text.vram + gp_reloc)

    # Known .data variables
    if data != None:
        for data_reloc in dll.reloc_table.data_relocations:
            data_ptr = data.words[data_reloc // 4]
            data.addSymbol(data.vram + data_ptr)
    
    # Known global variables
    for e in dll.reloc_table.global_offset_table[4:]:
        if (e & 0x80000000) == 0:
            var_vram = DLL_VRAM_BASE + e
            if bss != None and var_vram >= bss.vram and var_vram < bss.vramEnd:
                bss.addSymbol(var_vram)
            elif data != None and var_vram >= data.vram and var_vram < data.vramEnd:
                data.addSymbol(var_vram)
            elif rodata != None and var_vram >= rodata.vram and var_vram < rodata.vramEnd:
                rodata.addSymbol(var_vram)
            elif var_vram >= text.vram and var_vram < text.vramEnd:
                text.addSymbol(var_vram)
            else:
                global_segment.addSymbol(var_vram)

def __init_sections(context: spimdisasm.common.Context, dll: DLL, dll_bytes: bytes, bss_size: int | None):
    text: spimdisasm.mips.sections.SectionText
    rodata: spimdisasm.mips.sections.SectionRodata | None = None
    data: spimdisasm.mips.sections.SectionData | None = None
    bss: spimdisasm.mips.sections.SectionBss | None = None

    text = spimdisasm.mips.sections.SectionText(
        context, 
        vromStart=dll.header.size,
        vromEnd=dll.header.size + dll.get_text_size(),
        vram=DLL_VRAM_BASE,
        filename="",
        array_of_bytes=dll_bytes,
        segmentVromStart=0,
        overlayCategory=None)
    text.setCommentOffset(text.vromStart)

    if dll.has_rodata():
        rodata_offset = dll.header.rodata_offset + dll.reloc_table.get_size()
        rodata = spimdisasm.mips.sections.SectionRodata(
            context,
            vromStart=rodata_offset,
            vromEnd=rodata_offset + dll.get_rodata_size(),
            vram=DLL_VRAM_BASE + (rodata_offset - dll.header.size),
            filename="",
            array_of_bytes=dll_bytes,
            segmentVromStart=0,
            overlayCategory=None)
        rodata.setCommentOffset(rodata.vromStart)

    if dll.has_data():
        data = spimdisasm.mips.sections.SectionData(
            context,
            vromStart=dll.header.data_offset,
            vromEnd=dll.header.data_offset + dll.get_data_size(),
            vram=DLL_VRAM_BASE + (dll.header.data_offset - dll.header.size),
            filename="",
            array_of_bytes=dll_bytes,
            segmentVromStart=0,
            overlayCategory=None)
        data.setCommentOffset(data.vromStart)
    
    if bss_size != None and bss_size > 0:
        bss_offset = dll.get_bss_offset()
        bss = spimdisasm.mips.sections.SectionBss(
            context,
            vromStart=bss_offset,
            vromEnd=bss_offset,
            bssVramStart=DLL_VRAM_BASE + (bss_offset - dll.header.size),
            bssVramEnd=DLL_VRAM_BASE + (bss_offset - dll.header.size + bss_size),
            filename="",
            segmentVromStart=0,
            overlayCategory=None)
        bss.setCommentOffset(bss.vromStart)

    return text, rodata, data, bss

def __make_spimdisasm_context(dll: DLL, dll_bytes: bytes, bss_size: int | None) -> spimdisasm.common.Context:
    if bss_size == None:
        bss_size = 0
    
    context = spimdisasm.common.Context()

    # Set ROM/VRAM ranges
    context.changeGlobalSegmentRanges(
        vromStart=0,
        vromEnd=len(dll_bytes),
        vramStart=DLL_VRAM_BASE,
        vramEnd=DLL_VRAM_BASE + (dll.get_bss_offset() - dll.header.size + bss_size))
    
    # Set up global offset table
    got: list[spimdisasm.common.GotEntry] = []
    for i, e in enumerate(dll.reloc_table.global_offset_table):
        if (e & 0x80000000) != 0:
            # Import
            is_local = False
            s = context.globalSegment.addSymbol(e)
            s.isGot = True # spimdisasm will emit %gp_rel without this set!
        else:
            if i < 4:
                # Section symbol
                is_local = True
            elif e >= 0x1_0000 and (e & 0xFFFF) == 0:
                # Hi address of local symbol (probably, this is somewhat heuristic)
                is_local = True
            else:
                # Normal global symbol
                is_local = False

            e += DLL_VRAM_BASE
        
        got.append(spimdisasm.common.GotEntry(e, isGlobal=not is_local))
    
    context.setupGotTable(__get_dll_gp(dll), got)

    return context

def __get_dll_gp(dll: DLL) -> int:
    return DLL_VRAM_BASE + (dll.header.rodata_offset - dll.header.size)

def __rewrite_to_standard_gp_prologues(dll: DLL, dll_bytes: bytes) -> bytes:
    io = BytesIO(dll_bytes)
    
    for gp_reloc in dll.reloc_table.gp_relocations:
        io.seek(dll.header.size + gp_reloc)
        io.write(__STANDARD_MIPS_GP_PROLOGUE)

    return io.getvalue()

def __set_spimdisasm_config(gp: int):
    spimdisasm.common.GlobalConfig.ABI = spimdisasm.common.Abi.O32
    spimdisasm.common.GlobalConfig.COMPILER = spimdisasm.common.Compiler.IDO
    spimdisasm.common.GlobalConfig.ENDIAN = spimdisasm.common.InputEndian.BIG
    spimdisasm.common.GlobalConfig.ARCHLEVEL = spimdisasm.common.ArchLevel.MIPS3
    spimdisasm.common.GlobalConfig.GP_VALUE = gp # Should already have the base VRAM applied!
    spimdisasm.common.GlobalConfig.PIC = True
    spimdisasm.common.GlobalConfig.EMIT_CPLOAD = False
    spimdisasm.common.GlobalConfig.ASM_NM_LABEL = ""
    spimdisasm.common.GlobalConfig.SYMBOL_FINDER_FILTER_ADDRESSES_ADDR_LOW = DLL_VRAM_BASE
    spimdisasm.common.GlobalConfig.SYMBOL_FINDER_FILTER_ADDRESSES_ADDR_HIGH = 0xB0000000 # Filter out direct ROM address
    spimdisasm.common.GlobalConfig.RODATA_STRING_GUESSER_LEVEL = 4 # 3 can detect empty strings, 4 can detect whitespace strings

def __set_rabbitizer_config():
    rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.O32
    rabbitizer.config.regNames_fprAbiNames = rabbitizer.Abi.O32
    rabbitizer.config.regNames_userFpcCsr = False
    rabbitizer.config.regNames_vr4300Cop0NamedRegisters = False
    rabbitizer.config.pseudos_pseudoMove = False
    rabbitizer.config.toolchainTweaks_treatJAsUnconditionalBranch = False
    rabbitizer.config.toolchainTweaks_gnuMode = True # Required for emitting matching asm
