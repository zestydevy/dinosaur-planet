.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TumbleweedBush_ctor
.dword TumbleweedBush_dtor

# export table
/*0*/ .dword TumbleweedBush_setup
/*1*/ .dword TumbleweedBush_control
/*2*/ .dword TumbleweedBush_update
/*3*/ .dword TumbleweedBush_print
/*4*/ .dword TumbleweedBush_free
/*5*/ .dword TumbleweedBush_get_model_flags
/*6*/ .dword TumbleweedBush_get_data_size
/*7*/ .dword TumbleweedBush_remove_tumbleweed
