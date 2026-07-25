.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIMSnowBallGenerator_ctor
.dword DIMSnowBallGenerator_dtor

# export table
/*0*/ .dword DIMSnowBallGenerator_obj_Setup
/*1*/ .dword DIMSnowBallGenerator_obj_Control
/*2*/ .dword DIMSnowBallGenerator_obj_Update
/*3*/ .dword DIMSnowBallGenerator_obj_Print
/*4*/ .dword DIMSnowBallGenerator_obj_Free
/*5*/ .dword DIMSnowBallGenerator_obj_GetModelFlags
/*6*/ .dword DIMSnowBallGenerator_obj_GetDataSize
