.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ChukaChuck_ctor
.dword ChukaChuck_dtor

# export table
/*0*/ .dword ChukaChuck_setup
/*1*/ .dword ChukaChuck_control
/*2*/ .dword ChukaChuck_update
/*3*/ .dword ChukaChuck_print
/*4*/ .dword ChukaChuck_free
/*5*/ .dword ChukaChuck_get_model_flags
/*6*/ .dword ChukaChuck_get_data_size
