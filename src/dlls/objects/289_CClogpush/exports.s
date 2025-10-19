.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CClogpush_ctor
.dword CClogpush_dtor

# export table
/*0*/ .dword CClogpush_setup
/*1*/ .dword CClogpush_control
/*2*/ .dword CClogpush_update
/*3*/ .dword CClogpush_print
/*4*/ .dword CClogpush_free
/*5*/ .dword CClogpush_get_model_flags
/*6*/ .dword CClogpush_get_data_size
