#ifndef _INPUT_H_
#define _INPUT_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"
#include "variables.h"

#define CONTROLLER_THREAD_ID 0x62
// NOTE: This size is NOT CONFIRMED YET
#define CONTROLLER_THREAD_STACKSIZE 1152
// The length of gContInterruptBuffer
#define CONT_INTERRUPT_BUFFER_LENGTH 2
#define CONTROLLER_MESG_BUFFER_2_LENGTH 8
#define CONTROLLER_MESG_BUFFER_3_LENGTH 1

typedef struct _UnkInputStruct {
    OSContPad unk0x0[MAXCONTROLLERS];
    u16 unk0x18[MAXCONTROLLERS];
    u16 unk0x20[MAXCONTROLLERS];
} UnkInputStruct;

extern u16 D_8008C8A4;
extern u8 gIgnoreJoystick;

/**
 * The message queue associated with SI controller interrupts.
 * 
 * This is the same message queue that is passed to osContInit.
 */
extern OSMesgQueue gContInterruptQueue;
extern OSMesgQueue gControllerMesgQueue2;
extern OSMesgQueue gControllerMesgQueue3;
extern OSMesg gContInterruptBuffer[CONT_INTERRUPT_BUFFER_LENGTH];
extern OSMesg gControllerMesgQueue2Buffer[CONTROLLER_MESG_BUFFER_2_LENGTH];
extern OSMesg gControllerMesgQueue3Buffer[CONTROLLER_MESG_BUFFER_3_LENGTH];

/**
 * Message sent to the gContInterruptQueue when an SI interrupt occurs.
 */
extern OSMesg gContInterruptMessage;

/**
 * The status and type of each controller (SI device).
 */
extern OSContStatus gContStatuses[4];

extern OSThread gControllerThread;
extern u8 gControllerThreadStack[CONTROLLER_THREAD_STACKSIZE];

extern int D_800A8608;
extern s16 D_800A8618;

extern OSContPad gContPads[MAXCONTROLLERS];

extern u8 buttonQueue_;
extern UnkInputStruct *contpad_buffer[2];

extern UnkInputStruct D_800A7DC0[MAXCONTROLLERS];
extern UnkInputStruct D_800A7E60[MAXCONTROLLERS];

extern u8 D_800A7DB1;
extern u8 D_800A7DB2[2];

/**
 * If TRUE, no controllers are inserted.
 * 
 * @see init_controller_data
 */
extern s32 gNoControllers;

/**
 * Initializes SI settings and controller globals.
 * 
 * @returns The index of the last inserted controller.
 * For example, if one controller is inserted, 0 will be returned.
 * If no controllers are inserted, -1 will be returned.
 */
s32 init_controller_data();

void start_controller_thread(OSSched *scheduler);

#endif
