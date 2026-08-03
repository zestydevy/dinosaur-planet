.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword newfog_ctor
.dword newfog_dtor

# export table
/*0*/ .dword newfog_Func_A0
/*1*/ .dword newfog_Func_464
/*2*/ .dword newfog_Func_5AC
/*3*/ .dword newfog_Func_1234
/*4*/ .dword newfog_Func_139C
/*5*/ .dword newfog_Func_13AC
/*6*/ .dword newfog_Func_1764
/*7*/ .dword newfog_GetColor
/*8*/ .dword newfog_GetPosition
/*9*/ .dword newfog_Func_18C0
/*10*/ .dword newfog_Func_14B0
/*11*/ .dword newfog_Func_1608
/*12*/ .dword newfog_Func_18C8
