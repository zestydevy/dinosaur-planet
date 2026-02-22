#ifndef _DLLS_469_H
#define _DLLS_469_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef struct {
    s16 testStartRadius;        //Test begins when player inside this radius
    s16 musicPlayTimer;         //Plays a music track after a delay
    s16 delayTimer;             //Short pause between actions while the cup minigame is active
    s16 musWhispersVol;         //Volume of whispering music track
    s16 musWhipersVolSpeed;     //Rate of change of whispering's volume
    s16 musCorridorVol;         //Volume of corridor music track
    s16 musCorridorVolSpeed;    //Rate of change of corridor music's volume
    s16 modGfxCircle;           //For removing modGfx circle mesh when test begins
    s16 unk10;                  //Can be queried through export, but seems to stay at 0?
    s16 shuffles;               //How many times to shuffle the cups at the beginning of a round
    s16 choiceTimeout;          //Timer for player to select a cup
    s16 substate;               //Secondary state value for the cup minigame
    s16 choiceWasCorrect;       //-1 when player has yet to choose, TRUE/FALSE after choice
    u8 spiritCup;               //The index of the cup that currently holds the Krazoa Spirit
    u8 state;                   //Main state value for the shrine
    u8 seqValue;                //Value managed by the animCallback function
    u8 musicStarted;            //Music started via timer
} ECSHshrine_Data;

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    s16 testStartRadius;        //Test begins when player inside this radius (stored multiplied by 256)
} ECSHshrine_Setup;

typedef enum {
    ECShrine_STATE_Waiting = 0,
    ECShrine_STATE_Test_Start = 1,
    ECShrine_STATE_Cup_Game_Start = 2,
    ECShrine_STATE_Cups_Round_1 = 3,
    ECShrine_STATE_Cups_Round_2 = 4,
    ECShrine_STATE_Cups_Round_3 = 5,
    ECShrine_STATE_Test_Successful = 6,
    ECShrine_STATE_Grant_Spirit = 7,
    ECShrine_STATE_Warp_Away = 8,
    ECShrine_STATE_Test_Failure = 9,
    ECShrine_STATE_Finished = 10
} ECSHshrine_States;

typedef enum {
    Cup_STATE_Unknown = -1,
    Cup_STATE_Stopped = 0,
    Cup_STATE_Moving = 1,
    Cup_STATE_Shuffle = 2,
    Cup_STATE_Underground = 3,
    Cup_STATE_Move_Finished = 4,
    Cup_STATE_Await_Choice = 5,
    Cup_STATE_Rise_Up = 6,
    Cup_STATE_Sink_Down = 7,
    Cup_STATE_Round_Start = 8,
    Cup_STATE_Round_End = 9
} ECSHshrine_Cup_States;

DLL_INTERFACE(DLL_469_ECSHshrine) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*7*/ void (*func7)(s16 *arg0);
/*8*/ void (*get_cup_coords)(u8 cupIndex, f32* x, f32* z); //Exports the x and z coords of a particular cup index.
/*9*/ void (*get_minigame_state)(s32* cupSubstate, u8* cupWithSpirit); //Exports the cup minigame's substate value, and the index of the cup currently holding the Krazoa Spirit.
/*10*/ void (*set_cup_coords)(u8 cupIndex, f32 x, f32 z); //Sets the x and z coords of a particular cup.
/*11*/ void (*choose_cup)(u8 chosenCupIndex); //Used when player touches a cup.
};

#endif // _DLLS_469_H
