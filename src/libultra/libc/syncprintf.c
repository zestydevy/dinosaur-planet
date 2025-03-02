// @DECOMP_OPT_FLAGS=-O3
#include <PR/os.h>
#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PR/rcp.h>
#include "libc/xstdio.h"
#include "libc/stdarg.h"

extern s32 __kmc_pt_mode;

static void* proutSyncPrintf(void* str, const char* buf, size_t n) {
    u32 sent = 0;

    while (sent < n) {
        sent += __osRdbSend((u8*)(buf + sent), n - sent, RDB_TYPE_GtoH_PRINT);
    }
    return (void*)1;
}

static volatile unsigned int* stat = (unsigned*)0xbff08004;
static volatile unsigned int* wport = (unsigned*)0xbff08000;
static volatile unsigned int* piok = (unsigned*)PHYS_TO_K1(PI_STATUS_REG);

static void rmonPutchar(char c) {
    while (*piok & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
    }

    while (!(*stat & 4)) {
    }
    
    *wport = c;
}

static void* kmc_proutSyncPrintf(void* str, const char* buf, int n) {
    int i;
    char c;
    char* p;
    char* q;
    char xbuf[128];
    static int column = 0;

    p = xbuf;

    for (i = 0; i < n; i++) {
        c = *buf++;

        switch (c) {
            case '\n':
                *p++ = '\n';
                column = 0;
                break;
            case '\t':
                do {
                    *p++ = ' ';
                } while (++column % 8);
                break;
            default:
                column++;
                *p++ = c;
                break;
        }

        if (c == '\n' || (p - xbuf) > 100) {
            rmonPutchar((p - xbuf) - 1);

            q = xbuf;
            while (q != p) {
                rmonPutchar(*q++);
            }
            p = xbuf;
        }
    }
    if (p != xbuf) {
        rmonPutchar((p - xbuf) - 1);

        q = xbuf;
        while (q != p) {
            rmonPutchar(*q++);
        }
    }
    return (void*)1;
}

void osSyncPrintf(const char* fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    if (__kmc_pt_mode) {
        ans = _Printf((outfun*)kmc_proutSyncPrintf, NULL, fmt, ap);
    } else {
        ans = _Printf(proutSyncPrintf, NULL, fmt, ap);
    }
    va_end(ap);
}

void rmonPrintf(const char* fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    if (__kmc_pt_mode) {
        ans = _Printf((outfun*)kmc_proutSyncPrintf, NULL, fmt, ap);
    }
    va_end(ap);
}
