.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword RopeBaddie_ctor
.dword RopeBaddie_dtor

# export table
/*0*/ .dword RopeBaddie_obj_Setup
/*1*/ .dword RopeBaddie_obj_Control
/*2*/ .dword RopeBaddie_obj_Update
/*3*/ .dword RopeBaddie_obj_Print
/*4*/ .dword RopeBaddie_obj_Free
/*5*/ .dword RopeBaddie_obj_GetModelFlags
/*6*/ .dword RopeBaddie_obj_GetDataSize
