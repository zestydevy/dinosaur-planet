.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_168_ctor
.dword dll_168_dtor

# export table
/*0*/ .dword dll_168_modgfx_Func0
