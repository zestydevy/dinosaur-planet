.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Transporter_ctor
.dword Transporter_dtor

# export table
/*0*/ .dword Transporter_obj_Setup
/*1*/ .dword Transporter_obj_Control
/*2*/ .dword Transporter_obj_Update
/*3*/ .dword Transporter_obj_Print
/*4*/ .dword Transporter_obj_Free
/*5*/ .dword Transporter_obj_GetModelFlags
/*6*/ .dword Transporter_obj_GetDataSize
