.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Caictua_ctor
.dword Caictua_dtor

# export table
/*0*/ .dword Caictua_obj_Setup
/*1*/ .dword Caictua_obj_Control
/*2*/ .dword Caictua_obj_Update
/*3*/ .dword Caictua_obj_Print
/*4*/ .dword Caictua_obj_Free
/*5*/ .dword Caictua_obj_GetModelFlags
/*6*/ .dword Caictua_obj_GetDataSize
/*7*/ .dword Caictua_GetAnimState
/*8*/ .dword Caictua_ReceiveMessage
