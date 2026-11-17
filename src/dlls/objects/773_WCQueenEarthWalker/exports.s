.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCqueenearthwalker_ctor
.dword WCqueenearthwalker_dtor

# export table
/*0*/ .dword WCqueenearthwalker_obj_Setup
/*1*/ .dword WCqueenearthwalker_obj_Control
/*2*/ .dword WCqueenearthwalker_obj_Update
/*3*/ .dword WCqueenearthwalker_obj_Print
/*4*/ .dword WCqueenearthwalker_obj_Free
/*5*/ .dword WCqueenearthwalker_obj_GetModelFlags
/*6*/ .dword WCqueenearthwalker_obj_GetDataSize
