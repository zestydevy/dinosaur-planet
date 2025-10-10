.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFMainSlideDoor_ctor
.dword CFMainSlideDoor_dtor

# export table
/*0*/ .dword CFMainSlideDoor_create
/*1*/ .dword CFMainSlideDoor_update
/*2*/ .dword CFMainSlideDoor_func_1D4
/*3*/ .dword CFMainSlideDoor_draw
/*4*/ .dword CFMainSlideDoor_destroy
/*5*/ .dword CFMainSlideDoor_get_model_flags
/*6*/ .dword CFMainSlideDoor_get_state_size
