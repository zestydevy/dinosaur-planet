.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DRExplodeDoor_ctor
.dword DRExplodeDoor_dtor

# export table
/*0*/ .dword DRExplodeDoor_setup
/*1*/ .dword DRExplodeDoor_control
/*2*/ .dword DRExplodeDoor_update
/*3*/ .dword DRExplodeDoor_print
/*4*/ .dword DRExplodeDoor_free
/*5*/ .dword DRExplodeDoor_get_model_flags
/*6*/ .dword DRExplodeDoor_get_data_size
