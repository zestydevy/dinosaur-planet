.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword gameover_ctor
.dword gameover_dtor

# export table
/*0*/ .dword gameover_update1
/*1*/ .dword gameover_update2
/*2*/ .dword gameover_draw
