.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFLevelControl_ctor
.dword CFLevelControl_dtor

# export table
/*0*/ .dword CFLevelControl_obj_Setup
/*1*/ .dword CFLevelControl_obj_Control
/*2*/ .dword CFLevelControl_obj_Update
/*3*/ .dword CFLevelControl_obj_Print
/*4*/ .dword CFLevelControl_obj_Free
/*5*/ .dword CFLevelControl_obj_GetModelFlags
/*6*/ .dword CFLevelControl_obj_GetDataSize
