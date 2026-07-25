.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCKingEarthWalker_ctor
.dword WCKingEarthWalker_dtor

# export table
/*0*/ .dword WCKingEarthWalker_obj_Setup
/*1*/ .dword WCKingEarthWalker_obj_Control
/*2*/ .dword WCKingEarthWalker_obj_Update
/*3*/ .dword WCKingEarthWalker_obj_Print
/*4*/ .dword WCKingEarthWalker_obj_Free
/*5*/ .dword WCKingEarthWalker_obj_GetModelFlags
/*6*/ .dword WCKingEarthWalker_obj_GetDataSize
