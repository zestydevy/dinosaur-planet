#ifndef _TYPES_H_
#define _TYPES_H_

#include "ultra64.h"
#include "dll_def.h"
#include "game/objects/object.h"

typedef union {
    void (*asVoid)(void);
    s32 (*asVoidS32)(void);
    void (*withOneArg)(s32);
    void (*withTwoArgs)(s32, s32);
    void (*withThreeArgs)(s32, s32, s32);
    void (*withFourArgs)(s32, s32, s32, s32);
    void (*withFiveArgs)(s32, s32, s32, s32, u16);
    void (*withSixArgs)(s32, s32, s32, s32, s32, s32);
    void (*withSevenArgs)(s32, s32, s32, s32, s32, s32, s32);
} UnknownDLLFunc;

// "Unknown" DLL interface to help match code calling DLLs where the exact
// DLL being called has not been determined 
DLL_INTERFACE_BEGIN(Unknown)
    /* 0x04 */ UnknownDLLFunc func[1];
DLL_INTERFACE_END()

typedef struct {
/*0000*/ u16 unk0;
/*0002*/ u16 unk2;
/*0004*/ u16 unk4;
/*0006*/ u16 unk6;
/*0008*/ u16 unk8;
/*000A*/ u16 unka;
/*000C*/ u16 unkc;
/*000E*/ s16 unke;
/*0010*/ u16 unk10;
/*0012*/ union {
             u16 asShort;
             u8 asByte;
         } unk12;
/*0014*/ u8 unk14;
/*0015*/ u8 unk15;
/*0016*/ u8 unk16;
/*0017*/ u8 unk17;
/*0018*/ u8 unk18;
/*0019*/ u8 unk19;
/*001A*/ u8 unk1a;
/*001B*/ u8 unk1b;
/*001C*/ u8 unk1c;
/*001D*/ u8 unk1d;
/*001E*/ u8 unk1e;
/*001F*/ u8 unk1f;
/*0020*/ u8 unk20;
/*0021*/ u8 unk21;
/*0022*/ u8 unk22;
/*0023*/ u8 unk23;
} NewLfxStruct;

// TODO: copied from DKR, might be wrong
/* Size: 10 bytes */
typedef struct Vertex {
/* 0x00 */ s16 x;
/* 0x02 */ s16 y;
/* 0x04 */ s16 z;
/* 0x06 */ u8  r;
/* 0x07 */ u8  g;
/* 0x08 */ u8  b;
/* 0x09 */ u8  a;
} Vertex;

// TODO: copied from DKR, might be wrong
/* Size: 4 bytes */
typedef struct TexCoords {
    union {
      struct {
        s16 u, v;
      };
      u32 texCoords; // For convenience?
    };
} TexCoords;

// TODO: copied from DKR, might be wrong
/* Size: 0x10 bytes */
typedef struct Triangle {
    union {
        struct {
            /* 0x00 */ u8 flags; // 0x40 = Draw backface, 0x00 = Cull backface
            /* 0x01 */ u8 vi0;          // First vertex index
            /* 0x02 */ u8 vi1;          // Second vertex index
            /* 0x03 */ u8 vi2;          // Third vertex index
        };
    /* 0x00 */ u32 vertices; // For convenience?
               u8 verticesArray[4];
    };
/* 0x04 */ TexCoords uv0;   // Texture coordinates for the first vertex
/* 0x08 */ TexCoords uv1;   // Texture coordinates for the second vertex
/* 0x0C */ TexCoords uv2;   // Texture coordinates for the third vertex
} Triangle;

#endif
