.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MMshrine_ctor
.dword MMshrine_dtor

# export table
.dword MMshrine_setup
.dword MMshrine_control
.dword MMshrine_update
.dword MMshrine_print
.dword MMshrine_free
.dword MMshrine_get_model_flags
.dword MMshrine_get_data_size
