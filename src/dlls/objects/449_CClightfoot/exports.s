.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CClightfoot_ctor
.dword CClightfoot_dtor

# export table
.dword CClightfoot_setup
.dword CClightfoot_control
.dword CClightfoot_update
.dword CClightfoot_print
.dword CClightfoot_free
.dword CClightfoot_get_model_flags
.dword CClightfoot_get_data_size
