.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHspore_ctor
.dword SHspore_dtor

# export table
.dword SHspore_create
.dword SHspore_update
.dword SHspore_func_B88
.dword SHspore_draw
.dword SHspore_destroy
.dword SHspore_func_BF4
.dword SHspore_get_state_size
