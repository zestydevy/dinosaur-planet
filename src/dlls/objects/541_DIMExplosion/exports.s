.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMExplosion_ctor
.dword DIMExplosion_dtor

# export table
/*0*/ .dword DIMExplosion_setup
/*1*/ .dword DIMExplosion_control
/*2*/ .dword DIMExplosion_update
/*3*/ .dword DIMExplosion_print
/*4*/ .dword DIMExplosion_free
/*5*/ .dword DIMExplosion_get_model_flags
/*6*/ .dword DIMExplosion_get_data_size
