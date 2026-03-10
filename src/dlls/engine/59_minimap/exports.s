.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword minimap_ctor
.dword minimap_dtor

# export table
/*0*/ .dword minimap_toggle
/*1*/ .dword minimap_print
