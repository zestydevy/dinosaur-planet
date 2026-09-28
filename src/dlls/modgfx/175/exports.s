.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_175_ctor
.dword dll_175_dtor

# export table
/*0*/ .dword dll_175_modgfx_Func0
