#ifndef _GAME_LIGHTACTIONS_H
#define _GAME_LIGHTACTIONS_H

/** Indices for LACTIONS.bin (has 0x400 entries) */
typedef enum {
    LIGHTACTION_0 = 0x0, 

    LIGHTACTION_59 = 0x59, //SC_beacon: warm glow 
    LIGHTACTION_5A = 0x5A, //SC_beacon: warm glow (remove)

    LIGHTACTION_27B = 0x27B //last nonzero entry
} LightActionIndices;

#endif // _GAME_LIGHTACTIONS_H
