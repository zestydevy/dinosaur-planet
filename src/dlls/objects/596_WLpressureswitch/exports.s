.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLpressureswitch_ctor
.dword WLpressureswitch_dtor

# export table
.dword WLpressureswitch_setup
.dword WLpressureswitch_control
.dword WLpressureswitch_update
.dword WLpressureswitch_print
.dword WLpressureswitch_free
.dword WLpressureswitch_get_model_flags
.dword WLpressureswitch_get_data_size
