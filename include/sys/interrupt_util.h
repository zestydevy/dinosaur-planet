#ifndef _SYS_INTERRUPT_UTIL_H
#define _SYS_INTERRUPT_UTIL_H

#include "PR/ultratypes.h"

/**
 * Whether interrupts_disable/interrupts_enable should have any effect.
 */
extern s8 gIntDisFlag;

/**
 * Disables interrupts if gIntDisFlag is set.
 * Returns the previous value of the SR_IE bit.
 */
s32 interrupts_disable();

/**
 * Restores the SR_IE flag to the given value if gIntDisFlag is set.
 */
void interrupts_enable(s32 ie);

void set_gIntDisFlag(s32 value);

#endif
