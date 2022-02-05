#ifndef _LIBULTRA_OS_SIINT_H
#define _LIBULTRA_OS_SIINT_H

#include <PR/os_internal.h>
#include <PR/rcp.h>

void __osSiGetAccess(void);
void __osSiRelAccess(void);
int __osSiDeviceBusy(void);
void __osSiCreateAccessQueue(void);

#endif // _LIBULTRA_OS_SIINT_H
