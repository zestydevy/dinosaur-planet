.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBStealerWorm_ctor
.dword DBStealerWorm_dtor

# export table
/*0*/ .dword DBStealerWorm_obj_Setup
/*1*/ .dword DBStealerWorm_obj_Control
/*2*/ .dword DBStealerWorm_obj_Update
/*3*/ .dword DBStealerWorm_obj_Print
/*4*/ .dword DBStealerWorm_obj_Free
/*5*/ .dword DBStealerWorm_obj_GetModelFlags
/*6*/ .dword DBStealerWorm_obj_GetDataSize
/*7*/ .dword DBStealerWorm_GetAnimState
/*8*/ .dword DBStealerWorm_ReceiveMessage
