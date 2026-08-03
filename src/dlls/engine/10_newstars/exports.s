.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword newstars_ctor
.dword newstars_dtor

# export table
/*0*/ .dword newstars_Func_18
/*1*/ .dword newstars_Func_A1C
/*2*/ .dword newstars_Func_1118
