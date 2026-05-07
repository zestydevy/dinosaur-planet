.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword waterfx_ctor
.dword waterfx_dtor

# export table
.dword waterfx_tick
.dword waterfx_func_564
.dword waterfx_print
.dword waterfx_spawn_splash
.dword waterfx_spawn_circular_ripple
.dword waterfx_spawn_movement_ripple
.dword waterfx_init
.dword waterfx_set_circular_ripple_scale
