.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TrickyWarp_ctor
.dword TrickyWarp_dtor

# export table
.dword TrickyWarp_setup
.dword TrickyWarp_control
.dword TrickyWarp_update
.dword TrickyWarp_print
.dword TrickyWarp_free
.dword TrickyWarp_get_model_flags
.dword TrickyWarp_get_data_size
