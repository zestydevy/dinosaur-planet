.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword skylight_ctor
.dword skylight_dtor

# export table
/*0*/ .dword skylight_Init
/*1*/ .dword skylight_DoAction
/*2*/ .dword skylight_Tick
/*3*/ .dword skylight_Func3
/*4*/ .dword skylight_Func4
