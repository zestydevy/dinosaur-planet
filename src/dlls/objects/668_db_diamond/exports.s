.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBdiamond_ctor
.dword DBdiamond_dtor

# export table
.dword DBdiamond_create
.dword DBdiamond_update
.dword DBdiamond_func_204
.dword DBdiamond_draw
.dword DBdiamond_destroy
.dword DBdiamond_func_2BC
.dword DBdiamond_get_state_size
