.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFMainSlideDoor_ctor
.dword CFMainSlideDoor_dtor

# export table
/*0*/ .dword CFMainSlideDoor_setup
/*1*/ .dword CFMainSlideDoor_control
/*2*/ .dword CFMainSlideDoor_update
/*3*/ .dword CFMainSlideDoor_print
/*4*/ .dword CFMainSlideDoor_free
/*5*/ .dword CFMainSlideDoor_get_model_flags
/*6*/ .dword CFMainSlideDoor_get_data_size
