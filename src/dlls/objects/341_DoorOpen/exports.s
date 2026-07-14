.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DoorOpen_ctor
.dword DoorOpen_dtor

# export table
/*0*/ .dword DoorOpen_setup
/*1*/ .dword DoorOpen_control
/*2*/ .dword DoorOpen_update
/*3*/ .dword DoorOpen_print
/*4*/ .dword DoorOpen_free
/*5*/ .dword DoorOpen_get_model_flags
/*6*/ .dword DoorOpen_get_data_size
