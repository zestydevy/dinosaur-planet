#ifndef _INPUT_H_
#define _INPUT_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"
#include "variables.h"

#define CONTROLLER_THREAD_ID 0x62
// NOTE: This size is NOT CONFIRMED YET
#define CONTROLLER_THREAD_STACKSIZE 1152
#define CONTROLLER_MESG_QUEUE_LENGTH 8
#define CONTROLLER_MESG_QUEUE_3_LENGTH 1

typedef struct _UnkInputStruct {
    u16 unk0x0;
    s8 unk0x2;
    s8 unk0x3;
    u16 unk0x4;
} UnkInputStruct;

extern u16 D_8008C8A4;
extern u8 D_8008C8B0;

extern OSMesgQueue gControllerMesgQueue2;
extern OSMesgQueue gControllerMesgQueue;
extern OSMesgQueue gControllerMesgQueue3;
extern OSMesg gControllerMesgQueue_Array[CONTROLLER_MESG_QUEUE_LENGTH];
extern OSMesg gControllerMesgQueue3_Array[CONTROLLER_MESG_QUEUE_3_LENGTH];

extern OSThread gControllerThread;
extern u8 gControllerThreadStack[CONTROLLER_THREAD_STACKSIZE];

extern int D_800A8608;
extern s16 D_800A8618;

extern UnkInputStruct D_800a7f88[MAXCONTROLLERS];

#endif
