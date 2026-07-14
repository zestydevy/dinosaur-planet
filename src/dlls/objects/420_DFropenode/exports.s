.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_420_ctor
.dword dll_420_dtor

# export table
/*0*/ .dword dll_420_setup
/*1*/ .dword dll_420_control
/*2*/ .dword dll_420_update
/*3*/ .dword dll_420_print
/*4*/ .dword dll_420_free
/*5*/ .dword dll_420_get_model_flags
/*6*/ .dword dll_420_get_data_size
/*7*/ .dword dll_420_func_DFC
/*8*/ .dword dll_420_func_E28
/*9*/ .dword dll_420_func_F04
/*10*/ .dword dll_420_func_FDC
/*11*/ .dword dll_420_func_1098
/*12*/ .dword dll_420_func_149C
/*13*/ .dword dll_420_func_14AC
/*14*/ .dword dll_420_func_14F0
/*15*/ .dword dll_420_func_1514
/*16*/ .dword dll_420_func_148C
