.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MAGICMaker_ctor
.dword MAGICMaker_dtor

# export table
.dword MAGICMaker_setup
.dword MAGICMaker_control
.dword MAGICMaker_update
.dword MAGICMaker_print
.dword MAGICMaker_free
.dword MAGICMaker_get_model_flags
.dword MAGICMaker_get_data_size
