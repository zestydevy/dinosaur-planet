// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

static const char str_800a29d0[] = "ReadMem @ %08x for %d\n";
static const char str_800a29e8[] = "WriteMem\n";
static const char str_800a29f4[] = "Long unaligned write...\n";
static const char str_800a2a10[] = "RCP write not an integral number of words\n";
static const char str_800a2a3c[] = "ListProcesses\n";
static const char str_800a2a4c[] = "LoadProgram\n";
static const char str_800a2a5c[] = "GetExeName\n";

void __rmonWriteWordTo(u32* addr, u32 val) {
    while (__osSpRawWriteIo((u32)addr, val) != 0) { }
}

u32 __rmonReadWordAt(u32* addr) {
    u32 ret;

    if ((u32)addr >= 0x4000000 && (u32)addr <= 0x4ffffff) {
        __osSpRawReadIo((u32)addr, &ret);
        return ret;
    } else {
        return 0;
    }
}

void __rmonMemcpy(u8* dest, u8* srce, u32 count) {
    while (count--) {
	    *dest++ = *srce++;
    }
}

void __rmonCopyWords(u32* dest, u32* srce, u32 count) {
    while (count--) {
	    *dest++ = *srce++;
    }
}

void __rmon_strcpy(char *dest, char *src) {
    while (*dest++ = *src++) { }
}

int __rmonReadMem(KKHeader* req) {
    u8 *var1;
    int var2;
    int var3;
    KKReadRequest *request;
    KKBufferEvent *event;
    u8 *var4;

    request = (KKReadRequest*)req;
    event = (KKBufferEvent*)&__rmonUtilityBuffer;

    event->header.code = request->header.code;
    event->object = request->object;
    event->header.error = 0;
    
    if (request->addr == -1) {
	    return -5;
    }

    if (request->nbytes > 0x400) {
	    return -8;
    }

    if (req->method == 1) {
        if (!((request->addr < 0x4001000 || (request->addr + request->nbytes) > 0x4001fff) ? 0 : 1) && 
            !((request->addr < 0x4000000 || (request->addr + request->nbytes) > 0x4000fff) ? 0 : 1)) {

            return -5;
        }
    } else {
        if (osVirtualToPhysical((void*)request->addr) == -1) {
            return -5;
        }
    }

    var4 = (u8*)request->addr;
    event->header.length = 0x10 + request->nbytes;
    
    var3 = 0x10 + request->nbytes;

    var1 = (u8*)&var3;

    var2 = 0;

    while (var2 < 4) {
	    var2 += __osRdbSend(&var1[var2], 4 - var2, 8);
    }

    __rmonSendHeader((KKHeader *const)event, 0x10, 1);
    __rmonSendData((char *const)var4, request->nbytes);
    
    return 0;
}

int __rmonWriteMem(KKHeader* req) {
    register KKWriteRequest *request;
	KKObjectEvent event;
    int var1;
	u32 var2;
    int var3;
    u32 *var4;

    request = (KKWriteRequest*)req;

	if (req->method == 0 && osVirtualToPhysical((void*)request->writeHeader.addr) == -1) {
        return -5;
    }

	if (request->writeHeader.nbytes > 0x400) {
		return -8;
    }

	if ((request->writeHeader.addr < 0x4000000 || (request->writeHeader.addr + request->writeHeader.nbytes) > 0x4ffffff) ? 0 : 1) {
		var1 = request->writeHeader.addr & 3;

        if (var1) {
			if (request->writeHeader.nbytes != 1) {
				return -5;
			}

			var2 = __rmonReadWordAt((u32*)(request->writeHeader.addr & ~3));

			if (var1 == 1) {
				var2 = (var2 & 0xff00ffff) | (*request->buffer << 16);
            } else if (var1 == 2) {
				var2 = (var2 & 0xffff00ff) | (*request->buffer << 8);
            } else {
				var2 = (var2 & 0xffffff00) | (*request->buffer);
            }

			__rmonWriteWordTo((u32*)(request->writeHeader.addr & ~3), var2);
		} else {
			var3 = request->writeHeader.nbytes / 4;
			var4 = (u32*)request->buffer;

			if (request->writeHeader.nbytes % 4) {
				return -5;
			}

			while (var3--) {
				__rmonWriteWordTo((u32*)request->writeHeader.addr, *var4++);
				request->writeHeader.addr += 4;
			}
		}
	} else {
		__rmonMemcpy((u8*)request->writeHeader.addr, (u8*)request->buffer, request->writeHeader.nbytes);
	}

	event.header.code = request->writeHeader.header.code;
	event.header.error = 0;
	event.object = request->writeHeader.object;

	__rmonSendReply((KKHeader *const)&event, 0x10, 1);

	return 0;
}

int __rmonListProcesses(KKHeader* req) {
    KKObjectRequest *request;
	KKObjsEvent event;

    request = (KKObjectRequest*)req;

	event.object = 0;
	event.objs.number = 1;
	event.objs.objects[0] = req->method == 1 ? 0x3e9 : 0x3ea;
	event.header.code = request->header.code;
	event.header.error = 0;

	__rmonSendReply((KKHeader *const)&event, 0x18, 1);

	return 0;
}

int __rmonLoadProgram(KKHeader* req) {
    return -1;
}

int __rmonGetExeName(KKHeader* req) {
    KKObjectRequest *request;
	KKBufferEvent *event;

    request = (KKObjectRequest *)req;
    event = (KKBufferEvent *)&__rmonUtilityBuffer;

	event->header.code = request->header.code;
	event->header.error = 0;
	event->object = request->object;

	if (req->method == 1) {
        __rmon_strcpy(event->buffer, "imem");
    } else {
        __rmon_strcpy(event->buffer, "rmon");
    }

	__rmonSendReply((KKHeader *const)event, 0x18, 1);

	return 0;
}

static const char str_800a2a78[] = "GetRegionCount\n";
static const char str_800a2a88[] = "GetRegions\n";

int __rmonGetRegionCount(KKHeader* req) {
    KKObjectRequest *request;
	KKNumberEvent event;

    request = (KKObjectRequest *)req;

	event.header.code = request->header.code;
	event.header.error = 0;
	event.object = request->object;
	event.number = req->method == 1 ? 2 : 5;

	__rmonSendReply((KKHeader * const)&event, 0x14, 1);

	return 0;
}

int __rmonGetRegions(KKHeader* req) {
    KKObjectRequest *request;
	KKRegionEvent *event;
	int num;

    request = (KKObjectRequest*)req;
    event = (KKRegionEvent *)&__rmonUtilityBuffer;

    num = req->method == 1 ? 2 : 6;

    event->header.length = 0x24 + num * 0x10;
	event->header.code = request->header.code;
	event->header.error = 0;
	event->object = request->object;
	event->number = num;
	event->regions[1].vaddr = 0x4001000;
	event->regions[1].size =  0x00001000;
	event->regions[1].flags = 7;
	event->regions[1].paddr = 0x4001000;
	event->regions[0].vaddr = 0x4000000;
	event->regions[0].size =  0x1000;
	event->regions[0].flags = 3;
	event->regions[0].paddr = 0x4000000;

	if (num > 2) {
		event->regions[2].vaddr = 0x88200000;
		event->regions[2].size = 0x6130;
		event->regions[2].flags = 5;
		event->regions[2].paddr = 0;
		event->regions[3].vaddr = 4;
		event->regions[3].size =  0x200000;
		event->regions[3].flags = 3;
		event->regions[3].paddr = 0;
		event->regions[4].vaddr = 0x4002000;
		event->regions[4].size =  0x800000;
		event->regions[4].flags = 3;
		event->regions[4].paddr = 0;
		event->regions[5].vaddr = 0x88206130;
		event->regions[5].size =  0x9000;
		event->regions[5].flags = 3;
		event->regions[5].paddr = 0;
	}

	__rmonSendReply((KKHeader *const)event, event->header.length, 1);

	return 0;
}
