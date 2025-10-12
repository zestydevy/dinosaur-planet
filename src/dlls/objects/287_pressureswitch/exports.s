.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword pressureswitch_ctor
.dword pressureswitch_dtor

# export table
.dword pressureswitch_setup
.dword pressureswitch_control
.dword pressureswitch_update
.dword pressureswitch_print
.dword pressureswitch_free
.dword pressureswitch_get_model_flags
.dword pressureswitch_get_data_size
