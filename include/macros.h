#ifndef _MACROS_H
#define _MACROS_H

#define ALIGNED(x) __attribute__((aligned(x)))
#define ARRAYCOUNT(a) (sizeof(a) / sizeof(a[0]))
#define OFFSETOF(type, field) ((size_t)&(((type*)0)->field))
#define STUBBED_PRINTF(x) ((void)(x))
#define UNUSED __attribute__((unused))
#define STACKSIZE(x) (x / sizeof(u64))

#ifdef __sgi
#define __attribute__(x)
#endif

// A few systems in the game use an array as a cache table. This gives you the asset ID
#define ASSETCACHE_ID(x)    ((x << 1) + 0)
// A few systems in the game use an array as a cache table. This gives you the pointer to the asset
#define ASSETCACHE_PTR(x)   ((x << 1) + 1)

#endif
