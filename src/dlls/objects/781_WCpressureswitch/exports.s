.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCpressureswitch_ctor
.dword WCpressureswitch_dtor

# export table
.dword WCpressureswitch_setup
.dword WCpressureswitch_control
.dword WCpressureswitch_update
.dword WCpressureswitch_print
.dword WCpressureswitch_free
.dword WCpressureswitch_get_model_flags
.dword WCpressureswitch_get_state_size
