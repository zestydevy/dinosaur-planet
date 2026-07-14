.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword PlacedFood_ctor
.dword PlacedFood_dtor

# export table
/*0*/ .dword PlacedFood_setup
/*1*/ .dword PlacedFood_control
/*2*/ .dword PlacedFood_update
/*3*/ .dword PlacedFood_print
/*4*/ .dword PlacedFood_free
/*5*/ .dword PlacedFood_get_model_flags
/*6*/ .dword PlacedFood_get_data_size
/*7*/ .dword PlacedFood_get_unload_timer_value
