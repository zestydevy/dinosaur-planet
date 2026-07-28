.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWGeyser_ctor
.dword NWGeyser_dtor

# export table
/*0*/ .dword NWGeyser_obj_Setup
/*1*/ .dword NWGeyser_obj_Control
/*2*/ .dword NWGeyser_obj_Update
/*3*/ .dword NWGeyser_obj_Print
/*4*/ .dword NWGeyser_obj_Free
/*5*/ .dword NWGeyser_obj_GetModelFlags
/*6*/ .dword NWGeyser_obj_GetDataSize
