.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BigScorpionRobot_ctor
.dword BigScorpionRobot_dtor

# export table
/*0*/ .dword BigScorpionRobot_setup
/*1*/ .dword BigScorpionRobot_control
/*2*/ .dword BigScorpionRobot_update
/*3*/ .dword BigScorpionRobot_print
/*4*/ .dword BigScorpionRobot_free
/*5*/ .dword BigScorpionRobot_get_model_flags
/*6*/ .dword BigScorpionRobot_get_data_size
