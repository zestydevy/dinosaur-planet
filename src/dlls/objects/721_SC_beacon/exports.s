.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCbeacon_ctor
.dword SCbeacon_dtor

# export table
/*0*/ .dword SCbeacon_setup
/*1*/ .dword SCbeacon_control
/*2*/ .dword SCbeacon_update
/*3*/ .dword SCbeacon_print
/*4*/ .dword SCbeacon_free
/*5*/ .dword SCbeacon_get_model_flags
/*6*/ .dword SCbeacon_get_data_size
/*7*/ .dword SCbeacon_handle_kyte_flame_seqs
/*8*/ .dword SCbeacon_func_7D4
/*9*/ .dword SCbeacon_func_7EC
/*10*/ .dword SCbeacon_func_804
