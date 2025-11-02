.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword perchobject_ctor
.dword perchobject_dtor

# export table
/*0*/ .dword perchobject_setup
/*1*/ .dword perchobject_control
/*2*/ .dword perchobject_update
/*3*/ .dword perchobject_print
/*4*/ .dword perchobject_free
/*5*/ .dword perchobject_get_model_flags
/*6*/ .dword perchobject_get_data_size
/*7*/ .dword perchobject_land_on_perch
/*8*/ .dword perchobject_func_388
/*9*/ .dword perchobject_approach_perch
/*10*/ .dword perchobject_func_460
