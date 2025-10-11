.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword krazoatext_ctor
.dword krazoatext_dtor

# export table
.dword krazoatext_setup
.dword krazoatext_control
.dword krazoatext_update
.dword krazoatext_print
.dword krazoatext_free
.dword krazoatext_get_model_flags
.dword krazoatext_get_state_size
.dword krazoatext_print_text
