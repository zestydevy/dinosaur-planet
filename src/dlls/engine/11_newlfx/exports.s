.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword newlfx_ctor
.dword newlfx_dtor

# export table
/*0*/ .dword newlfx_DoAction
/*1*/ .dword newlfx_Free
/*2*/ .dword newlfx_Tick
/*3*/ .dword newlfx_Func_828
/*4*/ .dword newlfx_FindClosestLight
/*5*/ .dword newlfx_Func_9D8
/*6*/ .dword newlfx_Func_BB8
