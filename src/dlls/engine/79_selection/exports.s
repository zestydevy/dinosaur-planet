.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword selection_ctor
.dword selection_dtor

# export table
/*0*/ .dword selection_update1
/*1*/ .dword selection_update2
/*2*/ .dword selection_draw
/*3*/ .dword selection_set_selection_state
