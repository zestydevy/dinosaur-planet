.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KamerianBoss_ctor
.dword KamerianBoss_dtor

# export table
/*0*/ .dword KamerianBoss_setup
/*1*/ .dword KamerianBoss_control
/*2*/ .dword KamerianBoss_update
/*3*/ .dword KamerianBoss_print
/*4*/ .dword KamerianBoss_free
/*5*/ .dword KamerianBoss_get_model_flags
/*6*/ .dword KamerianBoss_get_data_size
