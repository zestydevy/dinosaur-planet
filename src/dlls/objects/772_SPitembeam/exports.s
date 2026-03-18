.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SPitembeam_ctor
.dword SPitembeam_dtor

# export table
/*0*/ .dword SPitembeam_setup
/*1*/ .dword SPitembeam_control
/*2*/ .dword SPitembeam_update
/*3*/ .dword SPitembeam_print
/*4*/ .dword SPitembeam_free
/*5*/ .dword SPitembeam_get_model_flags
/*6*/ .dword SPitembeam_get_data_size
