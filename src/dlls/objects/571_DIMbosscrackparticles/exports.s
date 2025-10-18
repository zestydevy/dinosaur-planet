.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMbosscrackparticles_ctor
.dword DIMbosscrackparticles_dtor

# export table
/*0*/ .dword DIMbosscrackparticles_setup
/*1*/ .dword DIMbosscrackparticles_control
/*2*/ .dword DIMbosscrackparticles_update
/*3*/ .dword DIMbosscrackparticles_print
/*4*/ .dword DIMbosscrackparticles_free
/*5*/ .dword DIMbosscrackparticles_get_model_flags
/*6*/ .dword DIMbosscrackparticles_get_data_size
