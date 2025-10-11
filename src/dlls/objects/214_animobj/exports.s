.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword animobj_ctor
.dword animobj_dtor

# export table
.dword animobj_setup
.dword animobj_control
.dword animobj_update
.dword animobj_print
.dword animobj_free
.dword animobj_get_model_flags
.dword animobj_get_data_size
