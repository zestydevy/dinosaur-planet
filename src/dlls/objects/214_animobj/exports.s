.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword animobj_ctor
.dword animobj_dtor

# export table
.dword animobj_create
.dword animobj_update
.dword animobj_func_318
.dword animobj_draw
.dword animobj_destroy
.dword animobj_func_484
.dword animobj_get_state_size
