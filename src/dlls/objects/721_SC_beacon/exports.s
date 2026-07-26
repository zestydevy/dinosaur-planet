.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCbeacon_ctor
.dword SCbeacon_dtor

# export table
/*0*/ .dword SCbeacon_obj_Setup
/*1*/ .dword SCbeacon_obj_Control
/*2*/ .dword SCbeacon_obj_Update
/*3*/ .dword SCbeacon_obj_Print
/*4*/ .dword SCbeacon_obj_Free
/*5*/ .dword SCbeacon_obj_GetModelFlags
/*6*/ .dword SCbeacon_obj_GetDataSize
/*7*/ .dword SCbeacon_KyteTarget_Interact
/*8*/ .dword SCbeacon_KyteTarget_Func_7D4
/*9*/ .dword SCbeacon_KyteTarget_Approach
/*10*/ .dword SCbeacon_KyteTarget_Func_804
