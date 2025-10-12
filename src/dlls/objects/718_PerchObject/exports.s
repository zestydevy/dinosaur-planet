.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword perchObject_ctor
.dword perchObject_dtor

# export table
/*0*/ .dword perchObject_setup
/*1*/ .dword perchObject_control
/*2*/ .dword perchObject_update
/*3*/ .dword perchObject_print
/*4*/ .dword perchObject_free
/*5*/ .dword perchObject_get_model_flags
/*6*/ .dword perchObject_get_data_size
/*7*/ .dword perchObject_land_on_perch
/*8*/ .dword perchObject_func_388
/*9*/ .dword perchObject_approach_perch
/*10*/ .dword perchObject_func_460
