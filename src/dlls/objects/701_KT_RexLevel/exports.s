.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KT_RexLevel_ctor
.dword KT_RexLevel_dtor

# export table
.dword KT_RexLevel_setup
.dword KT_RexLevel_control
.dword KT_RexLevel_update
.dword KT_RexLevel_print
.dword KT_RexLevel_free
.dword KT_RexLevel_get_model_flags
.dword KT_RexLevel_get_data_size
