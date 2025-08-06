/** DLL system
 */
#ifndef _SYS_DLL_H
#define _SYS_DLL_H

#include "PR/ultratypes.h"
#include "macros.h"

#define MAX_LOADED_DLLS 128
#define DLL_NONE -1

typedef struct {
	s32 offset;
	s32 bssSize;
} DLLTabEntry;

typedef struct {
/*0000*/	s32 bank1;
/*0004*/	s32 bank2;
/*0008*/	s32 reserved;
/*000C*/	s32 bank3;
} DLLTabHeader;

typedef struct {
/*0000*/	DLLTabHeader header;
/*0010*/	DLLTabEntry entries[1];
} DLLTab;

typedef struct {
/*0000*/	s32 id;
/*0004*/	s32 refCount;
/*0008*/	u32 *vtblPtr;
/*000C*/	void *end;
} DLLState;

struct DLLFile;

typedef void (*DLLFunc)(struct DLLFile *self);

typedef struct DLLFile
{
	u32 code;
	u32 data;
	s32 rodata;
	u16 exportCount;
	DLLFunc ctor;
	DLLFunc dtor;
	// Exports table begins here
	// After relocation, the exports table becomes a vtable
} DLLFile;

#define DLL_FILE_TO_EXPORTS(dllFile) ((u32*)((u32)dllFile + sizeof(DLLFile)))
#define DLL_EXPORTS_TO_FILE(exports) ((DLLFile*)((u32)exports - sizeof(DLLFile)))

// Note: A DLL interface IS a pointer to a DLL state vtblPtr field (DLLState.vtblPtr)
#define DLL_INTERFACE_TO_STATE(interfacePtr) ((DLLState*)((u32)interfacePtr - OFFSETOF(DLLState, vtblPtr)))

extern DLLState *gLoadedDLLList;
extern s32 gLoadedDLLCount;

extern u32 *gFile_DLLSIMPORTTAB;
extern DLLTab *gFile_DLLS_TAB;
extern s32 gDLLCount;

void init_dll_system();
/**
 * Returns the ID of the DLL that the given program counter is executing within, 
 * or -1 if the PC is not within a DLL.
 * 
 * start and end will also be set to pointers to the start and end 
 * of the DLL's body respectively.
 */
s32 find_executing_dll(u32 pc, void **start, void **end);
void replace_loaded_dll_list(DLLState list[], s32 count);
DLLState *get_loaded_dlls(s32 *outLoadedDLLCount);
void *dll_load_deferred(u16 id, u16 exportCount);
/**
 * Loads a DLL by ID and returns a pointer to its loaded interface.
 */
void *dll_load(u16 id, u16 exportCount, s32 runConstructor);
void dll_load_from_bytes(u16 id, void *dllBytes, s32 dllBytesSize, s32 bssSize);
s32 dll_unload(void *dllInterfacePtr);
s32 dll_throw_fault();

#endif //_SYS_DLL_H
