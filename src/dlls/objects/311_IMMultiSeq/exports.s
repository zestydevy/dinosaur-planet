.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword IMMultiSeq_ctor
.dword IMMultiSeq_dtor

# export table
/*0*/ .dword IMMultiSeq_setup
/*1*/ .dword IMMultiSeq_control
/*2*/ .dword IMMultiSeq_update
/*3*/ .dword IMMultiSeq_print
/*4*/ .dword IMMultiSeq_free
/*5*/ .dword IMMultiSeq_get_model_flags
/*6*/ .dword IMMultiSeq_get_data_size
