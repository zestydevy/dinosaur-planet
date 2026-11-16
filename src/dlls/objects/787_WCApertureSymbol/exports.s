.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCApertureSymbol_ctor
.dword WCApertureSymbol_dtor

# export table
/*0*/ .dword WCApertureSymbol_obj_Setup
/*1*/ .dword WCApertureSymbol_obj_Control
/*2*/ .dword WCApertureSymbol_obj_Update
/*3*/ .dword WCApertureSymbol_obj_Print
/*4*/ .dword WCApertureSymbol_obj_Free
/*5*/ .dword WCApertureSymbol_obj_GetModelFlags
/*6*/ .dword WCApertureSymbol_obj_GetDataSize
