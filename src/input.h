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
    OSContPad *unk0x0;
    u8 _padding[40 - sizeof(OSContPad)];
} UnkInputStruct;

extern u16 D_8008C8A4;
extern u8 D_8008C8B0;

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

extern u8 buttonQueue_[];
extern UnkInputStruct **contpad_buffer;

/**
 * If TRUE, no valid/inserted controllers were detected.
 * 
 * @see init_controller_data
 */
extern s32 gNoControllers;

#endif
