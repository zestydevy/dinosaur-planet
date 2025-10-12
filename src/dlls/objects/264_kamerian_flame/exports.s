.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword kamerian_flame_ctor
.dword kamerian_flame_dtor

# export table
.dword kamerian_flame_setup
.dword kamerian_flame_control
.dword kamerian_flame_update
.dword kamerian_flame_print
.dword kamerian_flame_free
.dword kamerian_flame_get_model_flags
.dword kamerian_flame_get_data_size
