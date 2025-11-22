.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CRCloudRace_ctor
.dword CRCloudRace_dtor

# export table
/*0*/ .dword CRCloudRace_setup
/*1*/ .dword CRCloudRace_control
/*2*/ .dword CRCloudRace_update
/*3*/ .dword CRCloudRace_print
/*4*/ .dword CRCloudRace_free
/*5*/ .dword CRCloudRace_get_model_flags
/*6*/ .dword CRCloudRace_get_data_size
