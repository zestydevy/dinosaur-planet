.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWMultiSeq_ctor
.dword NWMultiSeq_dtor

# export table
/*0*/ .dword NWMultiSeq_setup
/*1*/ .dword NWMultiSeq_control
/*2*/ .dword NWMultiSeq_update
/*3*/ .dword NWMultiSeq_print
/*4*/ .dword NWMultiSeq_free
/*5*/ .dword NWMultiSeq_get_model_flags
/*6*/ .dword NWMultiSeq_get_data_size
