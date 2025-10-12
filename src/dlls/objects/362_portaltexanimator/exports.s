.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword portaltexanimator_ctor
.dword portaltexanimator_dtor

# export table
.dword portaltexanimator_setup
.dword portaltexanimator_control
.dword portaltexanimator_update
.dword portaltexanimator_print
.dword portaltexanimator_free
.dword portaltexanimator_get_model_flags
.dword portaltexanimator_get_data_size
