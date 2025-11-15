.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_211_ctor
.dword dll_211_dtor

# export table
/*0*/ .dword dll_211_setup
/*1*/ .dword dll_211_control
/*2*/ .dword dll_211_update
/*3*/ .dword dll_211_print
/*4*/ .dword dll_211_free
/*5*/ .dword dll_211_get_model_flags
/*6*/ .dword dll_211_get_data_size
/*7*/ .dword dll_211_func_1064
/*8*/ .dword dll_211_func_1074
/*9*/ .dword dll_211_func_108C
/*10*/ .dword dll_211_func_109C
/*11*/ .dword dll_211_func_10B4
/*12*/ .dword dll_211_func_10C4
/*13*/ .dword dll_211_func_10D4
/*14*/ .dword dll_211_func_1248
/*15*/ .dword dll_211_func_1270
/*16*/ .dword dll_211_func_1284
/*17*/ .dword dll_211_func_1298
/*18*/ .dword dll_211_func_12B4
/*19*/ .dword dll_211_func_12C8
/*20*/ .dword dll_211_func_12D8
/*21*/ .dword dll_211_func_12F0
/*22*/ .dword dll_211_func_1398
/*23*/ .dword dll_211_func_1578
/*24*/ .dword dll_211_func_14E8
/*25*/ .dword dll_211_func_1550
/*26*/ .dword dll_211_func_1230
