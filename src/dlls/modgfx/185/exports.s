.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_185_ctor
.dword dll_185_dtor

# export table
/*0*/ .dword dll_185_modgfx_Func0
