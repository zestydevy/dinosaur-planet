.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCDockDoor_ctor
.dword CCDockDoor_dtor

# export table
/*0*/ .dword CCDockDoor_obj_Setup
/*1*/ .dword CCDockDoor_obj_Control
/*2*/ .dword CCDockDoor_obj_Update
/*3*/ .dword CCDockDoor_obj_Print
/*4*/ .dword CCDockDoor_obj_Free
/*5*/ .dword CCDockDoor_obj_GetModelFlags
/*6*/ .dword CCDockDoor_obj_GetDataSize
