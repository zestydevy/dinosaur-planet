.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLpressureswitch_ctor
.dword WLpressureswitch_dtor

# export table
.dword WLpressureswitch_create
.dword WLpressureswitch_update
.dword WLpressureswitch_func_598
.dword WLpressureswitch_draw
.dword WLpressureswitch_destroy
.dword WLpressureswitch_get_model_flags
.dword WLpressureswitch_get_state_size
