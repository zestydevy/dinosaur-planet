.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCcollectables_ctor
.dword SCcollectables_dtor

# export table
/*0*/ .dword SCcollectables_setup
/*1*/ .dword SCcollectables_control
/*2*/ .dword SCcollectables_update
/*3*/ .dword SCcollectables_print
/*4*/ .dword SCcollectables_free
/*5*/ .dword SCcollectables_get_model_flags
/*6*/ .dword SCcollectables_get_data_size
/*7*/ .dword SCcollectables_start_falling
/*8*/ .dword SCcollectables_copy_object_position
