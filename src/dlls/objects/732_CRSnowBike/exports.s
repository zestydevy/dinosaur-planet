.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CRSnowBike_ctor
.dword CRSnowBike_dtor

# export table
/*0*/ .dword CRSnowBike_setup
/*1*/ .dword CRSnowBike_control
/*2*/ .dword CRSnowBike_update
/*3*/ .dword CRSnowBike_print
/*4*/ .dword CRSnowBike_free
/*5*/ .dword CRSnowBike_get_model_flags
/*6*/ .dword CRSnowBike_get_data_size
/*7*/ .dword CRSnowBike_func_18FC
/*8*/ .dword CRSnowBike_func_1980
/*9*/ .dword CRSnowBike_func_1990
/*10*/ .dword CRSnowBike_func_19B4
/*11*/ .dword CRSnowBike_func_1A24
/*12*/ .dword CRSnowBike_func_1A34
/*13*/ .dword CRSnowBike_get_state
/*14*/ .dword CRSnowBike_set_state
/*15*/ .dword CRSnowBike_func_1B10
/*16*/ .dword CRSnowBike_func_1B9C
/*17*/ .dword CRSnowBike_func_1C28
/*18*/ .dword CRSnowBike_func_1C38
/*19*/ .dword CRSnowBike_func_1DB8
/*20*/ .dword CRSnowBike_func_4B30
