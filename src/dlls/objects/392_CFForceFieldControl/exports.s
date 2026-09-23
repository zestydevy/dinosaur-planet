.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFForceControl_ctor
.dword CFForceControl_dtor

# export table
/*0*/ .dword CFForceControl_obj_Setup
/*1*/ .dword CFForceControl_obj_Control
/*2*/ .dword CFForceControl_obj_Update
/*3*/ .dword CFForceControl_obj_Print
/*4*/ .dword CFForceControl_obj_Free
/*5*/ .dword CFForceControl_obj_GetModelFlags
/*6*/ .dword CFForceControl_obj_GetDataSize
