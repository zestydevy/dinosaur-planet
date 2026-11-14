.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword LFXEmitter_ctor
.dword LFXEmitter_dtor

# export table
/*0*/ .dword LFXEmitter_obj_Setup
/*1*/ .dword LFXEmitter_obj_Control
/*2*/ .dword LFXEmitter_obj_Update
/*3*/ .dword LFXEmitter_obj_Print
/*4*/ .dword LFXEmitter_obj_Free
/*5*/ .dword LFXEmitter_obj_GetModelFlags
/*6*/ .dword LFXEmitter_obj_GetDataSize
/*7*/ .dword LFXEmitter_GetActionSlot
/*8*/ .dword LFXEmitter_Func_6DC
