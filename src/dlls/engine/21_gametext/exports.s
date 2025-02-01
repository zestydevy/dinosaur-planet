.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword gametext_ctor
.dword gametext_dtor

# export table
.dword gametext_bank_count
.dword gametext_chunk_count
.dword gametext_set_bank
.dword gametext_curr_bank
.dword gametext_get_chunk
.dword gametext_get_text
