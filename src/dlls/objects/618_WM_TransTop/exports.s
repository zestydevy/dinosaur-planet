.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WM_TransTop_ctor
.dword WM_TransTop_dtor

# export table
/*0*/ .dword WM_TransTop_setup
/*1*/ .dword WM_TransTop_control
/*2*/ .dword WM_TransTop_update
/*3*/ .dword WM_TransTop_print
/*4*/ .dword WM_TransTop_free
/*5*/ .dword WM_TransTop_get_model_flags
/*6*/ .dword WM_TransTop_get_data_size
