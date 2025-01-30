.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword task_ctor
.dword task_dtor

# export table
.dword task_load_recently_completed
.dword task_mark_task_completed
.dword task_get_num_recently_completed
.dword task_func_390
.dword task_func_3F8
.dword task_get_completion_percentage
