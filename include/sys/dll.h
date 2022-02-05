/** DLL system
 */
#ifndef _SYS_DLL_H
#define _SYS_DLL_H

#include "PR/ultratypes.h"
enum DLL_ID {
    DLL_UI        = 0x01,
    DLL_CAMERA    = 0x02,
    DLL_ANIM      = 0x03,
    DLL_RACE      = 0x04,
    DLL_AMSEQ     = 0x05,
    DLL_AMSFX     = 0x06,
    DLL_SKY       = 0x07,
    DLL_NEWCLOUDS = 0x09,
    DLL_NEWSTARS  = 0x0A,
    DLL_NEWLFX    = 0x0B,
    DLL_MINIC     = 0x0C,
    DLL_EXPGFX    = 0x0D,
    DLL_MODGFX    = 0x0E,
    DLL_PROJGFX   = 0x0F,
    DLL_SCREENS   = 0x14,
    DLL_TEXT      = 0x15,
    DLL_SUBTITLES = 0x16,
    DLL_WATERFX   = 0x18,
    DLL_CURVES    = 0x1A,
    DLL_GPLAY     = 0x1D,
    DLL_SAVEGAME  = 0x1F,
    DLL_MINIMAP   = 0x3B,
    DLL_LINK      = 0x4A
};

typedef union {
    /* 0x04 */ void (*asVoid)(void);
    /* 0x04 */ s32 (*asVoidS32)(void); //HACK
    /* 0x04 */ void (*withOneArg)(s32);
    /* 0x04 */ void (*withTwoArgs)(s32, s32);
    /* 0x04 */ void (*withThreeArgs)(s32, s32, s32);
    /* 0x04 */ void (*withFourArgs)(s32, s32, s32, s32);
    /* 0x04 */ void (*withFiveArgs)(s32, s32, s32, s32, u16);
} DLLFuncs;
//eventually each DLL should be its own class with the appropriate signatures.
//for now, this works to match DLL calls.

typedef struct DLLInstance {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ DLLFuncs func[1]; //set to 1 to shut compiler up
} DLLInstance;

typedef struct
{
	s32 offset;
	s32 bssSize;
} DLLTabEntry;

typedef struct
{
///*0000*/	s32 bank0;
///*0004*/	s32 bank1;
///*0008*/	s32 reserved;
///*000C*/	s32 bank2;
/*0010*/	DLLTabEntry entries[1];
} DLLTab;

typedef struct
{
/*0000*/	s32 bank0;
/*0004*/	s32 bank1;
/*0008*/	s32 reserved;
/*000C*/	s32 bank2;
} DLLTabHeader;

typedef struct
{
/*0000*/	u32 id;
/*0004*/	s32 refCount;
/*0008*/	u32 * exports;
/*000C*/	u32 * end;
} DLLInst;

typedef void (*DLLFunc)(u32 a);

typedef struct
{
	u32 code;
	u32 data;
	s32 rodata;
	u16 exportCount;
	DLLFunc ctor;
	DLLFunc dtor;
	u32 exports[1]; // Exports table begins here
} DLLFile;

extern void ***gDLL_57;
extern struct DLLInstance
    **D_8008C970,
    **gDLL_1C,
    **gDLL_Camera,
    **gDLL_ANIM,
    **gDLL_Sky,
    **gDLL_08,
    **gDLL_newclouds,
    **gDLL_newstars,
    **gDLL_minic,
    **gDLL_UI,
    **gDLL_Race,
    **gDLL_AMSEQ,
    **gDLL_AMSEQ2,
    **gDLL_AMSFX,
    **gDLL_newlfx,
    **gDLL_39,
    **gDLL_3A,
    **gDLL_expgfx,
    **gDLL_modgfx,
    **gDLL_projgfx,
    **gDLL_10,
    **gDLL_11,
    **gDLL_12,
    **gDLL_SCREENS,
    **gDLL_text,
    **gDLL_subtitles,
    **gDLL_17,
    **gDLL_waterfx,
    **gDLL_19,
    **gDLL_CURVES,
    **gDLL_Link,
    **gDLL_4B,
    **gDLL_1B,
    **gDLL_gplay,
    **gDLL_38,
    **gDLL_1E,
    **gDLL_savegame,
    **gDLL_4C,
    **gDLL_20,
    **gDLL_21,
    **gDLL_3B,
    **gDLL_36;

extern DLLInst* gLoadedDLLList;
extern s32 gLoadedDLLCount;
extern DLLTab * gFile_DLLS_TAB;

/**
 * Returns the ID of the DLL that the given program counter is executing within, 
 * or -1 if the PC is not within a DLL.
 * 
 * start and end will also be set to pointers to the start and end 
 * of the DLL's body respectively.
 */
u32 find_executing_dll(u32 pc, u32 **start, u32 **end);

#endif //_SYS_DLL_H
