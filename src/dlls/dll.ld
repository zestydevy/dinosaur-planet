/** 
 * Base linker script for DLLs.
 * 
 * The specifics of this script aren't important since the resulting
 * ELF is converted to a Dinosaur Planet .dll with elf2dll. What matters
 * is that the following sections are present (if not empty, in any order):
 * - .text
 * - .rodata
 * - .data
 * - .bss
 * - .exports
 * - .rel.text
 * - .rel.rodata
 * - .rel.data
 * - .rel.exports
 */

SECTIONS
{
    . = 0x0;
    .text : {
        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    .data : {
        *(.data)
        *(.data.*)
    }

    .bss : {
        *(.bss)
        *(.bss.*)
    }

    /DISCARD/ : {
        *(.MIPS.abiflags)
    }

    /*========================================= */
    /* Stabs debugging sections.                */
    /*========================================= */
    .stab            0 : { *(.stab) }
    .stabstr         0 : { *(.stabstr) }
    .stab.excl       0 : { *(.stab.excl) }
    .stab.exclstr    0 : { *(.stab.exclstr) }
    .stab.index      0 : { *(.stab.index) }
    .stab.indexstr   0 : { *(.stab.indexstr) }
    .comment         0 : { *(.comment) }

    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0. */

    /* DWARF 1 */
    .debug           0 : { *(.debug) }
    .line            0 : { *(.line) }

    /* GNU DWARF 1 extensions */
    .debug_srcinfo   0 : { *(.debug_srcinfo) }
    .debug_sfnames   0 : { *(.debug_sfnames) }

    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges   0 : { *(.debug_aranges) }
    .debug_pubnames  0 : { *(.debug_pubnames) }

    /* DWARF 2 */
    .debug_info      0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev    0 : { *(.debug_abbrev) }
    .debug_line      0 : { *(.debug_line) }
    .debug_frame     0 : { *(.debug_frame) }
    .debug_str       0 : { *(.debug_str) }
    .debug_loc       0 : { *(.debug_loc) }
    .debug_macinfo   0 : { *(.debug_macinfo) }

    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }

    /* These must appear regardless of . */
    .gptab.sdata       : { *(.gptab.data) *(.gptab.sdata) }
    .gptab.sbss        : { *(.gptab.bss) *(.gptab.sbss) }
}
