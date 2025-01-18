.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_cannon_claw_ctor
.dword dll_cannon_claw_dtor

# export table
.dword dll_cannon_claw_func_18
.dword dll_cannon_claw_func_4C
.dword dll_cannon_claw_func_200
.dword dll_cannon_claw_func_20C
.dword dll_cannon_claw_func_26C
.dword dll_cannon_claw_func_27C
.dword dll_cannon_claw_func_28C
