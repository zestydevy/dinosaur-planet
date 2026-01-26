.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword amseq_ctor
.dword amseq_dtor

# export table
/*0*/ .dword amseq_set
/*1*/ .dword amseq_free
/*2*/ .dword amseq_is_set
/*3*/ .dword amseq_set_focus_obj
/*4*/ .dword amseq_play
/*5*/ .dword amseq_play_ex
/*6*/ .dword amseq_stop
/*7*/ .dword amseq_get_no
/*8*/ .dword amseq_get_volume_option
/*9*/ .dword amseq_set_volume_option
/*10*/ .dword amseq_func_1090
/*11*/ .dword amseq_func_109C
/*12*/ .dword amseq_get_volume
/*13*/ .dword amseq_set_volume
/*14*/ .dword amseq_func_1150
/*15*/ .dword amseq_func_1160
/*16*/ .dword amseq_func_1170
/*17*/ .dword amseq_func_345C
/*18*/ .dword amseq_tick
/*19*/ .dword amseq_func_1244
/*20*/ .dword amseq_func_1250
/*21*/ .dword amseq_func_125C
/*22*/ .dword amseq_func_1268
/*23*/ .dword amseq_func_1274
/*24*/ .dword amseq_func_1284
/*25*/ .dword amseq_func_1294
/*26*/ .dword amseq_func_12A4
/*27*/ .dword amseq_func_12B4
/*28*/ .dword amseq_func_12C4
/*29*/ .dword amseq_func_12D4
/*30*/ .dword amseq_func_12E4
/*31*/ .dword amseq_func_12F8
/*32*/ .dword amseq_func_130C
/*33*/ .dword amseq_func_1320
/*34*/ .dword amseq_func_1334
/*35*/ .dword amseq_func_1348
