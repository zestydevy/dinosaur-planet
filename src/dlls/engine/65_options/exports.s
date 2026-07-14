.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword options_ctor
.dword options_dtor

# export table
/*0*/ .dword options_update1
/*1*/ .dword options_update2
/*2*/ .dword options_draw
