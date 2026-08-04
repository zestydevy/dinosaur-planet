.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword newstars_ctor
.dword newstars_dtor

# export table
/*0*/ .dword newstars_Init
/*1*/ .dword newstars_Print
/*2*/ .dword newstars_Free
