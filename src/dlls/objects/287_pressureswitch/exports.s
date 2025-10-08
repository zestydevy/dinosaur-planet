.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword pressureswitch_ctor
.dword pressureswitch_dtor

# export table
.dword pressureswitch_create
.dword pressureswitch_update
.dword pressureswitch_func_524
.dword pressureswitch_draw
.dword pressureswitch_destroy
.dword pressureswitch_func_604
.dword pressureswitch_get_state_size
