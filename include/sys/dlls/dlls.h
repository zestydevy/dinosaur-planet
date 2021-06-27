#ifndef _DLLS_H_
#define _DLLS_H_

enum DLL_ID {
    DLL_ANIM      = 0x03,
    DLL_RACE      = 0x04,
    DLL_AMSEQ     = 0x05,
    DLL_AMSFX     = 0x06,
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
    DLL_SAVEGAME  = 0x1F
};

typedef struct
{
	s32 offset;
	s32 bssSize;
} DLLTabEntry;


typedef struct
{
/*0000*/	s32 bank0;
/*0004*/	s32 bank1;
/*0008*/	s32 reserved;
/*000C*/	s32 bank2;
/*0010*/	DLLTabEntry * entries;
} DLLTab;

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
	u32 * reserved;
} DLLFile;

extern DLLInst (*gLoadedDLLList)[];
extern s32 gLoadedDLLCount;
extern DLLTab * gFile_DLLS_TAB;

extern struct DllInstance
    **D_8008C970, 
    **D_8008C974, 
    **D_8008C978, 
    **gDLL_ANIM,
    **D_8008C980, 
    **D_8008C984, 
    **gDLL_newclouds, 
    **gDLL_newstars,
    **gDLL_minic, 
    **D_8008C994, 
    **gDLL_Race, 
    **gDLL_AMSEQ,
    **gDLL_AMSEQ2, 
    **gDLL_AMSFX, 
    **gDLL_newlfx, 
    **D_8008C9AC,
    **D_8008C9B0, 
    **gDLL_expgfx, 
    **gDLL_modgfx, 
    **gDLL_projgfx,
    **D_8008C9C0, 
    **D_8008C9C4, 
    **D_8008C9C8, 
    **gDLL_SCREENS,
    **gDLL_text, 
    **gDLL_subtitles, 
    **D_8008C9D8, 
    **gDLL_waterfx,
    **D_8008C9E0, 
    **gDLL_CURVES, 
    **D_8008C9E8, 
    **D_8008C9EC,
    **D_8008C9F0, 
    **gDLL_gplay, 
    **D_8008C9F8, 
    **D_8008C9FC,
    **gDLL_savegame, 
    **D_8008CA04, 
    **D_8008CA08, 
    **D_8008CA0C,
    **D_8008CA10, 
    **D_8008CA14;

#endif //_DLLS_H_
