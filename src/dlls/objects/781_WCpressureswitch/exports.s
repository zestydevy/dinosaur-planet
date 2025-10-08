.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCpressureswitch_ctor
.dword WCpressureswitch_dtor

# export table
.dword WCpressureswitch_create
.dword WCpressureswitch_update
.dword WCpressureswitch_func_4B4
.dword WCpressureswitch_draw
.dword WCpressureswitch_destroy
.dword WCpressureswitch_func_594
.dword WCpressureswitch_get_state_size
