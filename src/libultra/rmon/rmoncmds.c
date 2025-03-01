// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

int NotImplemented(KKHeader *a0);

static FUNPTR dispatchTable[] = {
	__rmonLoadProgram,
	__rmonListProcesses,
	__rmonGetExeName,
	__rmonListThreads,
	__rmonThreadStatus,
	NotImplemented,
	__rmonStopThread,
	__rmonRunThread,
	NotImplemented,
	NotImplemented,
	__rmonSetFault,
	NotImplemented,
	__rmonGetRegionCount,
	__rmonGetRegions,
	__rmonGetGRegisters,
	__rmonSetGRegisters,
	__rmonGetFRegisters,
	__rmonSetFRegisters,
	__rmonReadMem,
	__rmonWriteMem,
	__rmonSetBreak,
	__rmonClearBreak,
	__rmonListBreak,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	__rmonSetComm,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	NotImplemented,
	__rmonGetSRegs,
	__rmonSetSRegs,
	__rmonGetVRegs,
	__rmonSetVRegs,
	NotImplemented
};

int NotImplemented(KKHeader *a0) {
    return -1;
}

int __rmonExecute(KKHeader* request) {
    int retval;
    KKHeader event;
    
    if (request->code > 0x34) {
	    return -1;
    }

    retval = (*dispatchTable[request->code])(request);

    if (retval < 0){
        event.code = request->code;
        event.error = retval;

        __rmonSendReply((KKHeader *const)&event, 0xc, 1);
    }

    return retval;
}
