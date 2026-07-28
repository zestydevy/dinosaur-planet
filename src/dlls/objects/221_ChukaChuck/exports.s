.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ChukaChuck_ctor
.dword ChukaChuck_dtor

# export table
/*0*/ .dword ChukaChuck_obj_Setup
/*1*/ .dword ChukaChuck_obj_Control
/*2*/ .dword ChukaChuck_obj_Update
/*3*/ .dword ChukaChuck_obj_Print
/*4*/ .dword ChukaChuck_obj_Free
/*5*/ .dword ChukaChuck_obj_GetModelFlags
/*6*/ .dword ChukaChuck_obj_GetDataSize
