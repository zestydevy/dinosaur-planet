.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword kamerian_flame_ctor
.dword kamerian_flame_dtor

# export table
.dword kamerian_flame_create
.dword kamerian_flame_update
.dword kamerian_flame_func_518
.dword kamerian_flame_draw
.dword kamerian_flame_destroy
.dword kamerian_flame_func_668
.dword kamerian_flame_get_state_size
