.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFPowerBase_ctor
.dword CFPowerBase_dtor

# export table
/*0*/ .dword CFPowerBase_obj_Setup
/*1*/ .dword CFPowerBase_obj_Control
/*2*/ .dword CFPowerBase_obj_Update
/*3*/ .dword CFPowerBase_obj_Print
/*4*/ .dword CFPowerBase_obj_Free
/*5*/ .dword CFPowerBase_obj_GetModelFlags
/*6*/ .dword CFPowerBase_obj_GetDataSize
