.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFExplodePieces_ctor
.dword CFExplodePieces_dtor

# export table
/*0*/ .dword CFExplodePieces_obj_Setup
/*1*/ .dword CFExplodePieces_obj_Control
/*2*/ .dword CFExplodePieces_obj_Update
/*3*/ .dword CFExplodePieces_obj_Print
/*4*/ .dword CFExplodePieces_obj_Free
/*5*/ .dword CFExplodePieces_obj_GetModelFlags
/*6*/ .dword CFExplodePieces_obj_GetDataSize
/*7*/ .dword CFExplodePieces_GetState
