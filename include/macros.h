#ifndef _MACROS_H
#define _MACROS_H

#define ALIGNED(x) __attribute__((aligned(x)))
#define ARRAYCOUNT(a) (sizeof(a) / sizeof(a[0]))
#define OFFSETOF(type, field) ((size_t)&(((type*)0)->field))
#define STUBBED_PRINTF(x) ((void)(x))
#define UNUSED __attribute__((unused))

#ifdef __sgi
#define __attribute__(x)
#endif

#endif
