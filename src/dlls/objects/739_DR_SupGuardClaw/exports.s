.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DR_NPC_ctor
.dword DR_NPC_dtor

# export table
/*0*/ .dword DR_NPC_setup
/*1*/ .dword DR_NPC_control
/*2*/ .dword DR_NPC_update
/*3*/ .dword DR_NPC_print
/*4*/ .dword DR_NPC_free
/*5*/ .dword DR_NPC_get_model_flags
/*6*/ .dword DR_NPC_get_data_size
