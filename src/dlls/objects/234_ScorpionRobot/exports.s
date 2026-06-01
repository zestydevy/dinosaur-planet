.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ScorpionRobot_ctor
.dword ScorpionRobot_dtor

# export table
/*0*/ .dword ScorpionRobot_setup
/*1*/ .dword ScorpionRobot_control
/*2*/ .dword ScorpionRobot_update
/*3*/ .dword ScorpionRobot_print
/*4*/ .dword ScorpionRobot_free
/*5*/ .dword ScorpionRobot_get_model_flags
/*6*/ .dword ScorpionRobot_get_data_size
/*7*/ .dword ScorpionRobot_func_A8C
