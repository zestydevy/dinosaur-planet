#ifndef _SYS_GAME_INTERACTION_ARROW_H
#define _SYS_GAME_INTERACTION_ARROW_H

typedef enum {
    ARROW_Red = 1,
    ARROW_Orange = 2,
    ARROW_Yellow = 5,
    ARROW_Green = 6,
    ARROW_Blue = 4,
    ARROW_Pink = 3,
    ARROW_Black = 7,
    ARROW_8 = 8,
    ARROW_9 = 8,
    ARROW_10 = 10,
    ARROW_11 = 11
} InteractionArrowColour;

/** For use with Object->unkAF */
typedef enum {
    ARROW_FLAG_None = 0,
    ARROW_FLAG_1_Interacted = 1,            //set for 1 tick when player presses A
    ARROW_FLAG_2_Targeted = 2,              //set for 1 tick when player locked on
    ARROW_FLAG_4_Highlighted = 4,           //arrow visible, periodically set for 1 tick when player in range
    ARROW_FLAG_8_No_Targetting = 8,         //set for 1 tick when player ends lock-on (arrow floats/fades away)
    ARROW_FLAG_10_Greyed_Out = 0x10,        //grey arrow, plays "unsuccessful" sound when pressing A
    ARROW_FLAG_20_Removed = 0x20,           //arrow floats/fades away, lock-on ends, arrow doesn't appear when in range
    ARROW_FLAG_40_Unlock_Disabled = 0x40,   //player can't dismiss lock-on
    ARROW_FLAG_80 = 0x80
} InteractionArrowFlags;

#endif
